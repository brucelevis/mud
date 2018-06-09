//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>
#ifndef MUD_CPP_20
#include <map>
#endif

#ifdef MUD_MODULES
module mud.procgen;
#else
#include <infra/Vector.h>
#include <procgen/Wfc/Wfc.h>
// @kludge : not sure why this fixes a compilation error when using MSVC modules :/
#ifdef MUD_CPP_20
#include <srlz/Serial.h>
#endif
#endif

#include <json11.hpp>
using json = json11::Json;


namespace mud
{
	PatternHash hash_from_pattern(const ColorPattern& pattern, size_t palette_size)
	{
		/*CHECK_LT_F(std::pow((double)palette_size, (double)pattern.size()),
				   std::pow(2.0, sizeof(PatternHash) * 8),
				   "Too large palette (it is %lu) or too large pattern size (it's %.0f)",
				   palette_size, std::sqrt(pattern.size()));*/
		PatternHash result = 0;
		size_t power = 1;
		for(size_t i = 0; i < pattern.size(); ++i)
		{
			result += pattern[pattern.size() - 1 - i] * power;
			power *= palette_size;
		}
		return result;
	}

	ColorPattern pattern_from_hash(const PatternHash hash, int n, size_t palette_size)
	{
		uint64_t residue = hash;
		size_t power = size_t(std::pow(palette_size, n * n));
		ColorPattern result(n * n);

		for(size_t i = 0; i < result.size(); ++i)
		{
			power /= palette_size;
			size_t count = 0;

			while(residue >= power)
			{
				residue -= power;
				count++;
			}

			result[i] = static_cast<ColorIndex>(count);
		}

		return result;
	}

	template<typename Fun>
	ColorPattern make_pattern(size_t n, const Fun& fun)
	{
		ColorPattern result(n * n);
		for(size_t dy = 0; dy < n; ++dy)
			for(size_t dx = 0; dx < n; ++dx)
				result[dy * n + dx] = fun(dx, dy);
		return result;
	};

	Patternset::Patternset(int n, const PatternPrevalence& hashed_patterns, const ColorPalette& palette, PatternHash foundation_pattern)
		: m_propagator(hashed_patterns.size(), 2 * n - 1, 2 * n - 1, {})
	{
		UNUSED(foundation_pattern);
		m_n = n;
		
		for(const auto& it : hashed_patterns)
		{
			//if(it.first == foundation_pattern)
			//	m_foundation = m_patterns.size();

			m_patterns.push_back(pattern_from_hash(it.first, n, palette.size()));
			m_weights.push_back(it.second);
		}

		const auto agrees = [&](const ColorPattern& p1, const ColorPattern& p2, int dx, int dy)
		{
			int xmin = dx < 0 ? 0 : dx, xmax = dx < 0 ? dx + n : n;
			int ymin = dy < 0 ? 0 : dy, ymax = dy < 0 ? dy + n : n;
			for(int y = ymin; y < ymax; ++y)
				for(int x = xmin; x < xmax; ++x)
					if(p1[x + n * y] != p2[x - dx + n * (y - dy)])
						return false;
			return true;
		};

		size_t longest_propagator = 0;
		size_t sum_propagator = 0;

		for(uint16_t t = 0; t < hashed_patterns.size(); ++t)
			for(int x = 0; x < 2 * n - 1; ++x)
				for(int y = 0; y < 2 * n - 1; ++y)
				{
					std::vector<PatternIndex>& patterns = m_propagator.at(x, y)[t];
					for(uint16_t t2 = 0; t2 < hashed_patterns.size(); ++t2)
						if(agrees(m_patterns[t], m_patterns[t2], x - n + 1, y - n + 1))
							patterns.push_back(t2);

					patterns.shrink_to_fit();
					longest_propagator = max(longest_propagator, patterns.size());
					sum_propagator += patterns.size();
				}

		//LOG_F(INFO, "propagator length: mean/max/sum: %.1f, %lu, %lu",
		//    (double)sum_propagator / _propagator.size(), longest_propagator, sum_propagator);
	}

	bool on_boundary(int n, Wave& wave, int x, int y, int z)
	{
		return !wave.m_periodic && (x + n > wave.m_width || y + n > wave.m_height || z + n > wave.m_depth);
	};

	void propagate_overlapping(Patternset& tileset, Wave& wave)
	{
		int n = tileset.m_n;

		while(wave.m_changes.size() > 0)
		{
			uvec3 changed = vector_pop(wave.m_changes);
				
			for(int dx = -n + 1; dx < n; ++dx) for(int dy = -n + 1; dy < n; ++dy) for(int dz = -n + 1; dz < n; ++dz)
			{
				ivec3 coord = ivec3(changed.x + dx, changed.y + dy, changed.z + dz);

				int sx = coord.x;
				if(sx < 0) sx += wave.m_width;
				else if(sx >= wave.m_width) sx -= wave.m_width;

				int sy = coord.y;
				if(sy < 0) sy += wave.m_height;
				else if(sy >= wave.m_height) sy -= wave.m_height;

				int sz = coord.z;
				if(sz < 0) sz += wave.m_depth;
				else if(sz >= wave.m_depth) sz -= wave.m_depth;

				if(!wave.m_periodic && (sx + n > wave.m_width || sy + n > wave.m_height || sz + n > wave.m_depth))
					continue;
		
				for(size_t t2 = 0; t2 < wave.m_states.size(); ++t2)
				{
					if(!wave.m_wave.at(sx, sy, sz)[t2])
						continue;

					bool can_pattern_fit = false;

					const std::vector<PatternIndex>& prop = tileset.m_propagator.at(n - 1 - dx, n - 1 - dy, n - 1 - dz)[t2];
					for(const auto& t3 : prop) {
						if(wave.m_wave.at(changed.x, changed.y, changed.z)[t3]) {
							can_pattern_fit = true;
							break;
						}
					}

					if(!can_pattern_fit) {
						wave.m_wave.at(sx, sy, sz)[t2] = false;
						wave.m_changes.push_back(uvec3(sx, sy, sz));
					}
				}
			}
		}
	}

	PalettedImage load_paletted_image(const std::string& path)
	{
		int width, height, comp;
		RGBA* rgba = nullptr; // reinterpret_cast<RGBA*>(stbi_load(path.c_str(), &width, &height, &comp, 4));

		const size_t num_pixels = width * height;

		// Fix issues with stbi_load:
		if(comp == 1)
		{
			// input was greyscale - set alpha:
			for(RGBA* pixel = rgba; pixel < rgba + num_pixels; ++rgba)
				pixel->a = pixel->r;
		}
		else
		{
			if(comp == 3)
			{
				for(RGBA* pixel = rgba; pixel < rgba + num_pixels; ++rgba)
					pixel->a = 255;
			}
			for(RGBA* pixel = rgba; pixel < rgba + num_pixels; ++rgba)
			{
				if(pixel->a == 0)
					*pixel = RGBA{ 0,0,0,0 };
			}
		}

		std::vector<RGBA> palette;
		std::vector<ColorIndex> data;

		for(size_t pixel_idx = 0; pixel_idx < num_pixels; ++pixel_idx)
		{
			const RGBA color = rgba[pixel_idx];
			const uint8_t color_idx = uint8_t(std::find(palette.begin(), palette.end(), color) - palette.begin());
			if(color_idx == palette.size())
			{
				//CHECK_LT_F(palette.size(), MAX_COLORS, "Too many colors in image");
				palette.push_back(color);
			}
			data.push_back(color_idx);
		}

		//stbi_image_free(rgba);

		return PalettedImage{
			static_cast<size_t>(width),
			static_cast<size_t>(height),
			data, palette
		};
	}

	// n = side of the pattern, e.g. 3.
	PatternPrevalence extract_patterns(const PalettedImage& sample, int n, bool periodic_in, size_t symmetry, PatternHash* out_lowest_pattern)
	{
#ifndef MUD_MODULES // @todo clang bug
		const auto pattern_from_sample = [&](size_t x, size_t y) {
			return make_pattern(n, [&](size_t dx, size_t dy) { return sample.at_wrapped(x + dx, y + dy); });
		};
		const auto rotate = [&](const ColorPattern& p) { return make_pattern(n, [&](size_t x, size_t y) { return p[n - 1 - y + x * n]; }); };
		const auto reflect = [&](const ColorPattern& p) { return make_pattern(n, [&](size_t x, size_t y) { return p[n - 1 - x + y * n]; }); };

		PatternPrevalence patterns;

		size_t h = sample.height - n + 1U;
		size_t w = sample.height - n + 1U;
		for(size_t y = 0; y < (periodic_in ? sample.height : h); ++y)
			for(size_t x = 0; y < (periodic_in ? sample.width : w); ++x)
			{
				std::array<ColorPattern, 8> ps;
				ps[0] = pattern_from_sample(x, y);
				ps[1] = reflect(ps[0]);
				ps[2] = rotate(ps[0]);
				ps[3] = reflect(ps[2]);
				ps[4] = rotate(ps[2]);
				ps[5] = reflect(ps[4]);
				ps[6] = rotate(ps[4]);
				ps[7] = reflect(ps[6]);

				for(size_t k = 0; k < symmetry; ++k)
				{
					auto hash = hash_from_pattern(ps[k], sample.palette.size());
					patterns[hash] += 1;
					if(out_lowest_pattern && y == sample.height - 1)
						*out_lowest_pattern = hash;
				}
			}

		return patterns;
#endif
	}

	void run_overlapping(const std::string& image, size_t symmetry, int n, uint16_t width, uint16_t height, uint16_t depth, bool periodic)
	{
		const PalettedImage sample_image = load_paletted_image(image);
		const PatternPrevalence hashed_patterns = extract_patterns(sample_image, n, false, symmetry, nullptr);
		Patternset tileset = { n, hashed_patterns, sample_image.palette, kInvalidHash };

#ifndef MUD_MODULES // @todo clang bug
		Wave wave(uint16_t(hashed_patterns.size()), width, height, depth, periodic); // resize each to model._num_patterns
		wave.m_propagate = [&](Wave& wave) { propagate_overlapping(tileset, wave); };
		wave.m_valid_coord = [&](int x, int y, int z) { return !on_boundary(n, wave, x, y, z); };
		//wave.m_states = tileset.m_weights;

		wave.solve(0);
#endif
	}

	using WfcImage = array_3d<RGBA>;

	struct ProgressGif
	{
		const bool   kGifSeparatePalette = true;
		const size_t kGifInterval = 16; // Save an image every X iterations
		const int    kGifDelayCentiSec = 1;
		const int    kGifEndPauseCentiSec = 200;
		const size_t kUpscale = 4; // Upscale images before saving

		
		WfcImage upsample(const WfcImage& image)
		{
			WfcImage result(image.m_x * kUpscale, image.m_y * kUpscale, {});
			for(size_t y = 0; y < result.m_x; ++y)
				for(size_t x = 0; x < result.m_y; ++x)
					result.at(x, y) = image.at(x / kUpscale, y / kUpscale);
			return result;
		}
	};

	OverlapGraphics overlap_graphics(const Patternset& tileset, const Wave& wave)
	{
		OverlapGraphics result(wave.m_width, wave.m_height, {});
		for(size_t y = 0; y < wave.m_height; ++y)
			for(size_t x = 0; x < wave.m_width; ++x)
			{
				auto& tile_contributors = result.at(x, y);

				for(int dy = 0; dy < tileset.m_n; ++dy)
					for(int dx = 0; dx < tileset.m_n; ++dx)
					{
						int sx = x - dx;
						if(sx < 0) sx += wave.m_width;

						int sy = y - dy;
						if(sy < 0) sy += wave.m_height;

						if(!wave.m_valid_coord(sx, sy, 1))
							continue;

						for(size_t t = 0; t < wave.m_states.size(); ++t)
							if(wave.m_wave.at(sx, sy)[t])
								tile_contributors.push_back(tileset.m_patterns[t][dx + dy * tileset.m_n]);
					}
			}
		return result;
	}

	WfcImage overlap_image(const Patternset& tileset, const Wave& wave, const ColorPalette& palette)
	{
		OverlapGraphics graphics = overlap_graphics(tileset, wave);
		WfcImage result(graphics.m_y, graphics.m_x, 1); //{0, 0, 0, 0});

		for(size_t y = 0; y < graphics.m_x; ++y)
			for(size_t x = 0; x < graphics.m_y; ++x)
			{
				const auto& tile_contributors = graphics.at(x, y);
				if(tile_contributors.empty())
				{
					result.at(x, y) = { 0, 0, 0, 255 };
				}
				else if(tile_contributors.size() == 1)
				{
					result.at(x, y) = palette[tile_contributors[0]];
				}
				else
				{
					RGBA& colour = result.at(x, y);
					colour = { 0, 0, 0, 0 };
					for(const auto tile : tile_contributors)
					{
						colour.r += palette[tile].r;
						colour.g += palette[tile].g;
						colour.b += palette[tile].b;
						colour.a += palette[tile].a;
					}
					colour.r /= uint8_t(tile_contributors.size());
					colour.g /= uint8_t(tile_contributors.size());
					colour.b /= uint8_t(tile_contributors.size());
					colour.a /= uint8_t(tile_contributors.size());
				}
			}

		return result;
	}
}