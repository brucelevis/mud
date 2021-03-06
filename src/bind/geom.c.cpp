#include <type/Api.h>
#include <math/Api.h>
#include <geom/Api.h>

#ifdef MUD_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#define DECL EMSCRIPTEN_KEEPALIVE
#else
#define DECL
#endif


extern "C" {
	
	// Aabb
	mud::Type* DECL mud_Aabb__type() {
		return &mud::type<mud::Aabb>();
	}
	mud::Aabb* DECL mud_Aabb__construct_0() {
		return new mud::Aabb();
	}
	mud::Aabb* DECL mud_Aabb__construct_2(const mud::vec3* center, const mud::vec3* extents) {
		return new mud::Aabb(*center, *extents);
	}
	mud::vec3* DECL mud_Aabb__get_center(mud::Aabb* self) {
		return &self->m_center;
	}
	void DECL mud_Aabb__set_center(mud::Aabb* self, mud::vec3* value) {
		self->m_center = *value;
	}
	mud::vec3* DECL mud_Aabb__get_extents(mud::Aabb* self) {
		return &self->m_extents;
	}
	void DECL mud_Aabb__set_extents(mud::Aabb* self, mud::vec3* value) {
		self->m_extents = *value;
	}
	bool DECL mud_Aabb__get_empty(mud::Aabb* self) {
		return self->m_empty;
	}
	void DECL mud_Aabb__set_empty(mud::Aabb* self, bool value) {
		self->m_empty = value;
	}
	void DECL mud_Aabb__destroy(mud::Aabb* self) {
		delete self;
	}
	// Distribution
	mud::Type* DECL mud_Distribution__type() {
		return &mud::type<mud::Distribution>();
	}
	void DECL mud_Distribution__destroy(mud::Distribution* self) {
		delete self;
	}
	// Face3
	mud::Type* DECL mud_Face3__type() {
		return &mud::type<mud::Face3>();
	}
	mud::Face3* DECL mud_Face3__construct_0() {
		return new mud::Face3();
	}
	void DECL mud_Face3__destroy(mud::Face3* self) {
		delete self;
	}
	// MeshPacker
	mud::Type* DECL mud_MeshPacker__type() {
		return &mud::type<mud::MeshPacker>();
	}
	mud::MeshPacker* DECL mud_MeshPacker__construct_0() {
		return new mud::MeshPacker();
	}
	void DECL mud_MeshPacker__destroy(mud::MeshPacker* self) {
		delete self;
	}
	// Plane
	mud::Type* DECL mud_Plane__type() {
		return &mud::type<mud::Plane>();
	}
	mud::Plane* DECL mud_Plane__construct_0() {
		return new mud::Plane();
	}
	mud::vec3* DECL mud_Plane__get_normal(mud::Plane* self) {
		return &self->m_normal;
	}
	void DECL mud_Plane__set_normal(mud::Plane* self, mud::vec3* value) {
		self->m_normal = *value;
	}
	float DECL mud_Plane__get_distance(mud::Plane* self) {
		return self->m_distance;
	}
	void DECL mud_Plane__set_distance(mud::Plane* self, float value) {
		self->m_distance = value;
	}
	void DECL mud_Plane__destroy(mud::Plane* self) {
		delete self;
	}
	// Plane3
	mud::Type* DECL mud_Plane3__type() {
		return &mud::type<mud::Plane3>();
	}
	mud::Plane3* DECL mud_Plane3__construct_0() {
		return new mud::Plane3();
	}
	mud::vec3* DECL mud_Plane3__get_origin(mud::Plane3* self) {
		return &self->m_origin;
	}
	void DECL mud_Plane3__set_origin(mud::Plane3* self, mud::vec3* value) {
		self->m_origin = *value;
	}
	mud::vec3* DECL mud_Plane3__get_a(mud::Plane3* self) {
		return &self->m_a;
	}
	void DECL mud_Plane3__set_a(mud::Plane3* self, mud::vec3* value) {
		self->m_a = *value;
	}
	mud::vec3* DECL mud_Plane3__get_b(mud::Plane3* self) {
		return &self->m_b;
	}
	void DECL mud_Plane3__set_b(mud::Plane3* self, mud::vec3* value) {
		self->m_b = *value;
	}
	void DECL mud_Plane3__destroy(mud::Plane3* self) {
		delete self;
	}
	// Ray
	mud::Type* DECL mud_Ray__type() {
		return &mud::type<mud::Ray>();
	}
	mud::Ray* DECL mud_Ray__construct_0() {
		return new mud::Ray();
	}
	mud::vec3* DECL mud_Ray__get_start(mud::Ray* self) {
		return &self->m_start;
	}
	void DECL mud_Ray__set_start(mud::Ray* self, mud::vec3* value) {
		self->m_start = *value;
	}
	mud::vec3* DECL mud_Ray__get_end(mud::Ray* self) {
		return &self->m_end;
	}
	void DECL mud_Ray__set_end(mud::Ray* self, mud::vec3* value) {
		self->m_end = *value;
	}
	mud::vec3* DECL mud_Ray__get_dir(mud::Ray* self) {
		return &self->m_dir;
	}
	void DECL mud_Ray__set_dir(mud::Ray* self, mud::vec3* value) {
		self->m_dir = *value;
	}
	mud::vec3* DECL mud_Ray__get_inv_dir(mud::Ray* self) {
		return &self->m_inv_dir;
	}
	void DECL mud_Ray__set_inv_dir(mud::Ray* self, mud::vec3* value) {
		self->m_inv_dir = *value;
	}
	void DECL mud_Ray__destroy(mud::Ray* self) {
		delete self;
	}
	// Segment
	mud::Type* DECL mud_Segment__type() {
		return &mud::type<mud::Segment>();
	}
	mud::Segment* DECL mud_Segment__construct_0() {
		return new mud::Segment();
	}
	mud::vec3* DECL mud_Segment__get_start(mud::Segment* self) {
		return &self->m_start;
	}
	void DECL mud_Segment__set_start(mud::Segment* self, mud::vec3* value) {
		self->m_start = *value;
	}
	mud::vec3* DECL mud_Segment__get_end(mud::Segment* self) {
		return &self->m_end;
	}
	void DECL mud_Segment__set_end(mud::Segment* self, mud::vec3* value) {
		self->m_end = *value;
	}
	void DECL mud_Segment__destroy(mud::Segment* self) {
		delete self;
	}
	// Shape
	mud::Type* DECL mud_Shape__type() {
		return &mud::type<mud::Shape>();
	}
	mud::Type* DECL mud_Shape__get_type(mud::Shape* self) {
		return &self->m_type;
	}
	void DECL mud_Shape__destroy(mud::Shape* self) {
		delete self;
	}
	// ShapeVar
	mud::Type* DECL mud_ShapeVar__type() {
		return &mud::type<mud::ShapeVar>();
	}
	mud::ShapeVar* DECL mud_ShapeVar__construct_0() {
		return new mud::ShapeVar();
	}
	mud::ShapeVar* DECL mud_ShapeVar__construct_1(const mud::Shape* shape) {
		return new mud::ShapeVar(*shape);
	}
	mud::Shape* DECL mud_ShapeVar__get_shape(mud::ShapeVar* self) {
		return &self->shape();
	}
	void DECL mud_ShapeVar__destroy(mud::ShapeVar* self) {
		delete self;
	}
	// Symbol
	mud::Type* DECL mud_Symbol__type() {
		return &mud::type<mud::Symbol>();
	}
	mud::Symbol* DECL mud_Symbol__construct_0() {
		return new mud::Symbol();
	}
	mud::Symbol* DECL mud_Symbol__construct_1(mud::Colour* fill) {
		return new mud::Symbol(*fill);
	}
	mud::Symbol* DECL mud_Symbol__construct_2(mud::Colour* fill, mud::Colour* outline) {
		return new mud::Symbol(*fill, *outline);
	}
	mud::Symbol* DECL mud_Symbol__construct_3(mud::Colour* fill, mud::Colour* outline, bool overlay) {
		return new mud::Symbol(*fill, *outline, overlay);
	}
	mud::Symbol* DECL mud_Symbol__construct_4(mud::Colour* fill, mud::Colour* outline, bool overlay, bool double_sided) {
		return new mud::Symbol(*fill, *outline, overlay, double_sided);
	}
	mud::Symbol* DECL mud_Symbol__construct_5(mud::Colour* fill, mud::Colour* outline, bool overlay, bool double_sided, mud::SymbolDetail detail) {
		return new mud::Symbol(*fill, *outline, overlay, double_sided, detail);
	}
	mud::Colour* DECL mud_Symbol__get_outline(mud::Symbol* self) {
		return &self->m_outline;
	}
	void DECL mud_Symbol__set_outline(mud::Symbol* self, mud::Colour* value) {
		self->m_outline = *value;
	}
	mud::Colour* DECL mud_Symbol__get_fill(mud::Symbol* self) {
		return &self->m_fill;
	}
	void DECL mud_Symbol__set_fill(mud::Symbol* self, mud::Colour* value) {
		self->m_fill = *value;
	}
	bool DECL mud_Symbol__get_overlay(mud::Symbol* self) {
		return self->m_overlay;
	}
	void DECL mud_Symbol__set_overlay(mud::Symbol* self, bool value) {
		self->m_overlay = value;
	}
	bool DECL mud_Symbol__get_double_sided(mud::Symbol* self) {
		return self->m_double_sided;
	}
	void DECL mud_Symbol__set_double_sided(mud::Symbol* self, bool value) {
		self->m_double_sided = value;
	}
	mud::SymbolDetail DECL mud_Symbol__get_detail(mud::Symbol* self) {
		return self->m_detail;
	}
	void DECL mud_Symbol__set_detail(mud::Symbol* self, mud::SymbolDetail value) {
		self->m_detail = value;
	}
	const char* DECL mud_Symbol__get_image(mud::Symbol* self) {
		return self->m_image;
	}
	void DECL mud_Symbol__set_image(mud::Symbol* self, const char* value) {
		self->m_image = value;
	}
	mud::Image256* DECL mud_Symbol__get_image256(mud::Symbol* self) {
		return self->m_image256;
	}
	void DECL mud_Symbol__set_image256(mud::Symbol* self, mud::Image256* value) {
		self->m_image256 = value;
	}
	const char* DECL mud_Symbol__get_program(mud::Symbol* self) {
		return self->m_program;
	}
	void DECL mud_Symbol__set_program(mud::Symbol* self, const char* value) {
		self->m_program = value;
	}
	void DECL mud_Symbol__destroy(mud::Symbol* self) {
		delete self;
	}
	// Arc
	mud::Type* DECL mud_Arc__type() {
		return &mud::type<mud::Arc>();
	}
	mud::Arc* DECL mud_Arc__construct_0() {
		return new mud::Arc();
	}
	mud::Arc* DECL mud_Arc__construct_3(float radius, float start, float end) {
		return new mud::Arc(radius, start, end);
	}
	float DECL mud_Arc__get_radius(mud::Arc* self) {
		return self->m_radius;
	}
	void DECL mud_Arc__set_radius(mud::Arc* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_Arc__get_start(mud::Arc* self) {
		return self->m_start;
	}
	void DECL mud_Arc__set_start(mud::Arc* self, float value) {
		self->m_start = value;
	}
	float DECL mud_Arc__get_end(mud::Arc* self) {
		return self->m_end;
	}
	void DECL mud_Arc__set_end(mud::Arc* self, float value) {
		self->m_end = value;
	}
	void DECL mud_Arc__destroy(mud::Arc* self) {
		delete self;
	}
	// ArcLine
	mud::Type* DECL mud_ArcLine__type() {
		return &mud::type<mud::ArcLine>();
	}
	mud::ArcLine* DECL mud_ArcLine__construct_0() {
		return new mud::ArcLine();
	}
	mud::ArcLine* DECL mud_ArcLine__construct_3(const mud::vec3* start, const mud::vec3* middle, const mud::vec3* end) {
		return new mud::ArcLine(*start, *middle, *end);
	}
	mud::ArcLine* DECL mud_ArcLine__construct_4(const mud::vec3* center, const mud::vec3* start, const mud::vec3* middle, const mud::vec3* end) {
		return new mud::ArcLine(*center, *start, *middle, *end);
	}
	mud::vec3* DECL mud_ArcLine__get_start(mud::ArcLine* self) {
		return &self->m_start;
	}
	void DECL mud_ArcLine__set_start(mud::ArcLine* self, mud::vec3* value) {
		self->m_start = *value;
	}
	mud::vec3* DECL mud_ArcLine__get_middle(mud::ArcLine* self) {
		return &self->m_middle;
	}
	void DECL mud_ArcLine__set_middle(mud::ArcLine* self, mud::vec3* value) {
		self->m_middle = *value;
	}
	mud::vec3* DECL mud_ArcLine__get_end(mud::ArcLine* self) {
		return &self->m_end;
	}
	void DECL mud_ArcLine__set_end(mud::ArcLine* self, mud::vec3* value) {
		self->m_end = *value;
	}
	void DECL mud_ArcLine__destroy(mud::ArcLine* self) {
		delete self;
	}
	// Box
	mud::Type* DECL mud_Box__type() {
		return &mud::type<mud::Box>();
	}
	mud::Box* DECL mud_Box__construct_0() {
		return new mud::Box();
	}
	void DECL mud_Box__destroy(mud::Box* self) {
		delete self;
	}
	// Capsule
	mud::Type* DECL mud_Capsule__type() {
		return &mud::type<mud::Capsule>();
	}
	mud::Capsule* DECL mud_Capsule__construct_0() {
		return new mud::Capsule();
	}
	mud::Capsule* DECL mud_Capsule__construct_2(float radius, float height) {
		return new mud::Capsule(radius, height);
	}
	mud::Capsule* DECL mud_Capsule__construct_3(float radius, float height, mud::Axis axis) {
		return new mud::Capsule(radius, height, axis);
	}
	float DECL mud_Capsule__get_radius(mud::Capsule* self) {
		return self->m_radius;
	}
	void DECL mud_Capsule__set_radius(mud::Capsule* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_Capsule__get_height(mud::Capsule* self) {
		return self->m_height;
	}
	void DECL mud_Capsule__set_height(mud::Capsule* self, float value) {
		self->m_height = value;
	}
	mud::Axis DECL mud_Capsule__get_axis(mud::Capsule* self) {
		return self->m_axis;
	}
	void DECL mud_Capsule__set_axis(mud::Capsule* self, mud::Axis value) {
		self->m_axis = value;
	}
	void DECL mud_Capsule__destroy(mud::Capsule* self) {
		delete self;
	}
	// Circle
	mud::Type* DECL mud_Circle__type() {
		return &mud::type<mud::Circle>();
	}
	mud::Circle* DECL mud_Circle__construct_0() {
		return new mud::Circle();
	}
	mud::Circle* DECL mud_Circle__construct_1(float radius) {
		return new mud::Circle(radius);
	}
	mud::Circle* DECL mud_Circle__construct_2(float radius, mud::Axis axis) {
		return new mud::Circle(radius, axis);
	}
	float DECL mud_Circle__get_radius(mud::Circle* self) {
		return self->m_radius;
	}
	void DECL mud_Circle__set_radius(mud::Circle* self, float value) {
		self->m_radius = value;
	}
	mud::Axis DECL mud_Circle__get_axis(mud::Circle* self) {
		return self->m_axis;
	}
	void DECL mud_Circle__set_axis(mud::Circle* self, mud::Axis value) {
		self->m_axis = value;
	}
	void DECL mud_Circle__destroy(mud::Circle* self) {
		delete self;
	}
	// ConvexHull
	mud::Type* DECL mud_ConvexHull__type() {
		return &mud::type<mud::ConvexHull>();
	}
	mud::ConvexHull* DECL mud_ConvexHull__construct_0() {
		return new mud::ConvexHull();
	}
	mud::ConvexHull* DECL mud_ConvexHull__construct_1(const stl::vector<mud::vec3>* vertices) {
		return new mud::ConvexHull(*vertices);
	}
	void DECL mud_ConvexHull__destroy(mud::ConvexHull* self) {
		delete self;
	}
	// Cube
	mud::Type* DECL mud_Cube__type() {
		return &mud::type<mud::Cube>();
	}
	mud::Cube* DECL mud_Cube__construct_0() {
		return new mud::Cube();
	}
	mud::Cube* DECL mud_Cube__construct_1(const mud::vec3* extents) {
		return new mud::Cube(*extents);
	}
	mud::Cube* DECL mud_Cube__construct_2(const mud::vec3* center, const mud::vec3* extents) {
		return new mud::Cube(*center, *extents);
	}
	mud::vec3* DECL mud_Cube__get_extents(mud::Cube* self) {
		return &self->m_extents;
	}
	void DECL mud_Cube__set_extents(mud::Cube* self, mud::vec3* value) {
		self->m_extents = *value;
	}
	void DECL mud_Cube__destroy(mud::Cube* self) {
		delete self;
	}
	// Cylinder
	mud::Type* DECL mud_Cylinder__type() {
		return &mud::type<mud::Cylinder>();
	}
	mud::Cylinder* DECL mud_Cylinder__construct_0() {
		return new mud::Cylinder();
	}
	mud::Cylinder* DECL mud_Cylinder__construct_2(float radius, float height) {
		return new mud::Cylinder(radius, height);
	}
	mud::Cylinder* DECL mud_Cylinder__construct_3(float radius, float height, mud::Axis axis) {
		return new mud::Cylinder(radius, height, axis);
	}
	float DECL mud_Cylinder__get_radius(mud::Cylinder* self) {
		return self->m_radius;
	}
	void DECL mud_Cylinder__set_radius(mud::Cylinder* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_Cylinder__get_height(mud::Cylinder* self) {
		return self->m_height;
	}
	void DECL mud_Cylinder__set_height(mud::Cylinder* self, float value) {
		self->m_height = value;
	}
	mud::Axis DECL mud_Cylinder__get_axis(mud::Cylinder* self) {
		return self->m_axis;
	}
	void DECL mud_Cylinder__set_axis(mud::Cylinder* self, mud::Axis value) {
		self->m_axis = value;
	}
	void DECL mud_Cylinder__destroy(mud::Cylinder* self) {
		delete self;
	}
	// Ellipsis
	mud::Type* DECL mud_Ellipsis__type() {
		return &mud::type<mud::Ellipsis>();
	}
	mud::Ellipsis* DECL mud_Ellipsis__construct_0() {
		return new mud::Ellipsis();
	}
	mud::Ellipsis* DECL mud_Ellipsis__construct_1(mud::vec2* radius) {
		return new mud::Ellipsis(*radius);
	}
	mud::Ellipsis* DECL mud_Ellipsis__construct_2(mud::vec2* radius, mud::Axis axis) {
		return new mud::Ellipsis(*radius, axis);
	}
	mud::vec2* DECL mud_Ellipsis__get_radius(mud::Ellipsis* self) {
		return &self->m_radius;
	}
	void DECL mud_Ellipsis__set_radius(mud::Ellipsis* self, mud::vec2* value) {
		self->m_radius = *value;
	}
	mud::Axis DECL mud_Ellipsis__get_axis(mud::Ellipsis* self) {
		return self->m_axis;
	}
	void DECL mud_Ellipsis__set_axis(mud::Ellipsis* self, mud::Axis value) {
		self->m_axis = value;
	}
	void DECL mud_Ellipsis__destroy(mud::Ellipsis* self) {
		delete self;
	}
	// Geometry
	mud::Type* DECL mud_Geometry__type() {
		return &mud::type<mud::Geometry>();
	}
	mud::Geometry* DECL mud_Geometry__construct_0() {
		return new mud::Geometry();
	}
	void DECL mud_Geometry__destroy(mud::Geometry* self) {
		delete self;
	}
	// Grid2
	mud::Type* DECL mud_Grid2__type() {
		return &mud::type<mud::Grid2>();
	}
	mud::Grid2* DECL mud_Grid2__construct_0() {
		return new mud::Grid2();
	}
	mud::Grid2* DECL mud_Grid2__construct_1(const mud::vec2* size) {
		return new mud::Grid2(*size);
	}
	mud::Grid2* DECL mud_Grid2__construct_2(const mud::vec2* size, const mud::vec2* space) {
		return new mud::Grid2(*size, *space);
	}
	mud::vec2* DECL mud_Grid2__get_size(mud::Grid2* self) {
		return &self->m_size;
	}
	void DECL mud_Grid2__set_size(mud::Grid2* self, mud::vec2* value) {
		self->m_size = *value;
	}
	mud::vec2* DECL mud_Grid2__get_space(mud::Grid2* self) {
		return &self->m_space;
	}
	void DECL mud_Grid2__set_space(mud::Grid2* self, mud::vec2* value) {
		self->m_space = *value;
	}
	void DECL mud_Grid2__destroy(mud::Grid2* self) {
		delete self;
	}
	// Grid3
	mud::Type* DECL mud_Grid3__type() {
		return &mud::type<mud::Grid3>();
	}
	mud::Grid3* DECL mud_Grid3__construct_0() {
		return new mud::Grid3();
	}
	mud::Grid3* DECL mud_Grid3__construct_1(const mud::uvec2* size) {
		return new mud::Grid3(*size);
	}
	mud::Grid3* DECL mud_Grid3__construct_2(const mud::uvec2* size, const stl::vector<mud::vec3>* points) {
		return new mud::Grid3(*size, *points);
	}
	mud::uvec2* DECL mud_Grid3__get_size(mud::Grid3* self) {
		return &self->m_size;
	}
	void DECL mud_Grid3__set_size(mud::Grid3* self, mud::uvec2* value) {
		self->m_size = *value;
	}
	void DECL mud_Grid3__destroy(mud::Grid3* self) {
		delete self;
	}
	// Line
	mud::Type* DECL mud_Line__type() {
		return &mud::type<mud::Line>();
	}
	mud::Line* DECL mud_Line__construct_0() {
		return new mud::Line();
	}
	mud::Line* DECL mud_Line__construct_2(const mud::vec3* start, const mud::vec3* end) {
		return new mud::Line(*start, *end);
	}
	mud::vec3* DECL mud_Line__get_start(mud::Line* self) {
		return &self->m_start;
	}
	void DECL mud_Line__set_start(mud::Line* self, mud::vec3* value) {
		self->m_start = *value;
	}
	mud::vec3* DECL mud_Line__get_end(mud::Line* self) {
		return &self->m_end;
	}
	void DECL mud_Line__set_end(mud::Line* self, mud::vec3* value) {
		self->m_end = *value;
	}
	void DECL mud_Line__destroy(mud::Line* self) {
		delete self;
	}
	// Points
	mud::Type* DECL mud_Points__type() {
		return &mud::type<mud::Points>();
	}
	mud::Points* DECL mud_Points__construct_0() {
		return new mud::Points();
	}
	mud::Points* DECL mud_Points__construct_1(const stl::vector<mud::vec3>* points) {
		return new mud::Points(*points);
	}
	void DECL mud_Points__destroy(mud::Points* self) {
		delete self;
	}
	// Poisson
	mud::Type* DECL mud_Poisson__type() {
		return &mud::type<mud::Poisson>();
	}
	mud::Poisson* DECL mud_Poisson__construct_2(mud::vec2* size, float maxRadius) {
		return new mud::Poisson(*size, maxRadius);
	}
	bool DECL mud_Poisson_addPoint_2(mud::Poisson* self, float radius, mud::vec3* point) {
		return self->addPoint(radius, *point);
	}
	void DECL mud_Poisson__destroy(mud::Poisson* self) {
		delete self;
	}
	// Polygon
	mud::Type* DECL mud_Polygon__type() {
		return &mud::type<mud::Polygon>();
	}
	mud::Polygon* DECL mud_Polygon__construct_0() {
		return new mud::Polygon();
	}
	mud::Polygon* DECL mud_Polygon__construct_1(stl::vector<mud::vec3>* vertices) {
		return new mud::Polygon(*vertices);
	}
	void DECL mud_Polygon__destroy(mud::Polygon* self) {
		delete self;
	}
	// Quad
	mud::Type* DECL mud_Quad__type() {
		return &mud::type<mud::Quad>();
	}
	mud::Quad* DECL mud_Quad__construct_0() {
		return new mud::Quad();
	}
	mud::Quad* DECL mud_Quad__construct_4(const mud::vec3* a, const mud::vec3* b, const mud::vec3* c, const mud::vec3* d) {
		return new mud::Quad(*a, *b, *c, *d);
	}
	void DECL mud_Quad__destroy(mud::Quad* self) {
		delete self;
	}
	// Rect
	mud::Type* DECL mud_Rect__type() {
		return &mud::type<mud::Rect>();
	}
	mud::Rect* DECL mud_Rect__construct_0() {
		return new mud::Rect();
	}
	mud::Rect* DECL mud_Rect__construct_2(const mud::vec2* position, const mud::vec2* size) {
		return new mud::Rect(*position, *size);
	}
	mud::Rect* DECL mud_Rect__construct_4(float x, float y, float w, float h) {
		return new mud::Rect(x, y, w, h);
	}
	mud::vec2* DECL mud_Rect__get_position(mud::Rect* self) {
		return &self->m_position;
	}
	void DECL mud_Rect__set_position(mud::Rect* self, mud::vec2* value) {
		self->m_position = *value;
	}
	mud::vec2* DECL mud_Rect__get_size(mud::Rect* self) {
		return &self->m_size;
	}
	void DECL mud_Rect__set_size(mud::Rect* self, mud::vec2* value) {
		self->m_size = *value;
	}
	void DECL mud_Rect__destroy(mud::Rect* self) {
		delete self;
	}
	// Ring
	mud::Type* DECL mud_Ring__type() {
		return &mud::type<mud::Ring>();
	}
	mud::Ring* DECL mud_Ring__construct_0() {
		return new mud::Ring();
	}
	mud::Ring* DECL mud_Ring__construct_3(float radius, float min, float max) {
		return new mud::Ring(radius, min, max);
	}
	float DECL mud_Ring__get_radius(mud::Ring* self) {
		return self->m_radius;
	}
	void DECL mud_Ring__set_radius(mud::Ring* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_Ring__get_min(mud::Ring* self) {
		return self->m_min;
	}
	void DECL mud_Ring__set_min(mud::Ring* self, float value) {
		self->m_min = value;
	}
	float DECL mud_Ring__get_max(mud::Ring* self) {
		return self->m_max;
	}
	void DECL mud_Ring__set_max(mud::Ring* self, float value) {
		self->m_max = value;
	}
	void DECL mud_Ring__destroy(mud::Ring* self) {
		delete self;
	}
	// Sphere
	mud::Type* DECL mud_Sphere__type() {
		return &mud::type<mud::Sphere>();
	}
	mud::Sphere* DECL mud_Sphere__construct_0() {
		return new mud::Sphere();
	}
	mud::Sphere* DECL mud_Sphere__construct_1(float radius) {
		return new mud::Sphere(radius);
	}
	mud::Sphere* DECL mud_Sphere__construct_2(const mud::vec3* center, float radius) {
		return new mud::Sphere(*center, radius);
	}
	float DECL mud_Sphere__get_radius(mud::Sphere* self) {
		return self->m_radius;
	}
	void DECL mud_Sphere__set_radius(mud::Sphere* self, float value) {
		self->m_radius = value;
	}
	void DECL mud_Sphere__destroy(mud::Sphere* self) {
		delete self;
	}
	// SphereRing
	mud::Type* DECL mud_SphereRing__type() {
		return &mud::type<mud::SphereRing>();
	}
	mud::SphereRing* DECL mud_SphereRing__construct_0() {
		return new mud::SphereRing();
	}
	mud::SphereRing* DECL mud_SphereRing__construct_3(float radius, float min, float max) {
		return new mud::SphereRing(radius, min, max);
	}
	float DECL mud_SphereRing__get_radius(mud::SphereRing* self) {
		return self->m_radius;
	}
	void DECL mud_SphereRing__set_radius(mud::SphereRing* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_SphereRing__get_min(mud::SphereRing* self) {
		return self->m_min;
	}
	void DECL mud_SphereRing__set_min(mud::SphereRing* self, float value) {
		self->m_min = value;
	}
	float DECL mud_SphereRing__get_max(mud::SphereRing* self) {
		return self->m_max;
	}
	void DECL mud_SphereRing__set_max(mud::SphereRing* self, float value) {
		self->m_max = value;
	}
	void DECL mud_SphereRing__destroy(mud::SphereRing* self) {
		delete self;
	}
	// Spheroid
	mud::Type* DECL mud_Spheroid__type() {
		return &mud::type<mud::Spheroid>();
	}
	mud::Spheroid* DECL mud_Spheroid__construct_0() {
		return new mud::Spheroid();
	}
	mud::Spheroid* DECL mud_Spheroid__construct_1(float radius) {
		return new mud::Spheroid(radius);
	}
	mud::Spheroid* DECL mud_Spheroid__construct_2(const mud::vec3* center, float radius) {
		return new mud::Spheroid(*center, radius);
	}
	float DECL mud_Spheroid__get_radius(mud::Spheroid* self) {
		return self->m_radius;
	}
	void DECL mud_Spheroid__set_radius(mud::Spheroid* self, float value) {
		self->m_radius = value;
	}
	void DECL mud_Spheroid__destroy(mud::Spheroid* self) {
		delete self;
	}
	// Torus
	mud::Type* DECL mud_Torus__type() {
		return &mud::type<mud::Torus>();
	}
	mud::Torus* DECL mud_Torus__construct_0() {
		return new mud::Torus();
	}
	mud::Torus* DECL mud_Torus__construct_2(float radius, float solid_radius) {
		return new mud::Torus(radius, solid_radius);
	}
	mud::Torus* DECL mud_Torus__construct_3(float radius, float solid_radius, mud::Axis axis) {
		return new mud::Torus(radius, solid_radius, axis);
	}
	float DECL mud_Torus__get_radius(mud::Torus* self) {
		return self->m_radius;
	}
	void DECL mud_Torus__set_radius(mud::Torus* self, float value) {
		self->m_radius = value;
	}
	float DECL mud_Torus__get_solid_radius(mud::Torus* self) {
		return self->m_solid_radius;
	}
	void DECL mud_Torus__set_solid_radius(mud::Torus* self, float value) {
		self->m_solid_radius = value;
	}
	mud::Axis DECL mud_Torus__get_axis(mud::Torus* self) {
		return self->m_axis;
	}
	void DECL mud_Torus__set_axis(mud::Torus* self, mud::Axis value) {
		self->m_axis = value;
	}
	void DECL mud_Torus__destroy(mud::Torus* self) {
		delete self;
	}
	// Triangle
	mud::Type* DECL mud_Triangle__type() {
		return &mud::type<mud::Triangle>();
	}
	mud::Triangle* DECL mud_Triangle__construct_0() {
		return new mud::Triangle();
	}
	mud::Triangle* DECL mud_Triangle__construct_1(const mud::vec2* size) {
		return new mud::Triangle(*size);
	}
	mud::vec2* DECL mud_Triangle__get_size(mud::Triangle* self) {
		return &self->m_size;
	}
	void DECL mud_Triangle__set_size(mud::Triangle* self, mud::vec2* value) {
		self->m_size = *value;
	}
	void DECL mud_Triangle__destroy(mud::Triangle* self) {
		delete self;
	}
	mud::Ray* DECL mud_to_ray_2(const mud::vec3* pos, const mud::vec3* dir) {
		static mud::Ray temp;
		return (temp = mud::to_ray(*pos, *dir), &temp);
	}
	mud::Ray* DECL mud_to_ray_3(const mud::vec3* pos, const mud::vec3* dir, float distance) {
		static mud::Ray temp;
		return (temp = mud::to_ray(*pos, *dir, distance), &temp);
	}
	mud::Segment* DECL mud_to_segment_1(const mud::Ray* ray) {
		static mud::Segment temp;
		return (temp = mud::to_segment(*ray), &temp);
	}
	// DrawMode
	mud::DrawMode DECL mud_DrawMode_OUTLINE() {
		return mud::OUTLINE;
	}
	mud::DrawMode DECL mud_DrawMode_PLAIN() {
		return mud::PLAIN;
	}
	// PrimitiveType
	mud::PrimitiveType DECL mud_PrimitiveType_Points() {
		return mud::PrimitiveType::Points;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_Lines() {
		return mud::PrimitiveType::Lines;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_LineLoop() {
		return mud::PrimitiveType::LineLoop;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_LineStrip() {
		return mud::PrimitiveType::LineStrip;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_Triangles() {
		return mud::PrimitiveType::Triangles;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_TriangleStrip() {
		return mud::PrimitiveType::TriangleStrip;
	}
	mud::PrimitiveType DECL mud_PrimitiveType_TriangleFan() {
		return mud::PrimitiveType::TriangleFan;
	}
	// SymbolDetail
	mud::SymbolDetail DECL mud_SymbolDetail_Lowest() {
		return mud::SymbolDetail::Lowest;
	}
	mud::SymbolDetail DECL mud_SymbolDetail_Low() {
		return mud::SymbolDetail::Low;
	}
	mud::SymbolDetail DECL mud_SymbolDetail_Medium() {
		return mud::SymbolDetail::Medium;
	}
	mud::SymbolDetail DECL mud_SymbolDetail_High() {
		return mud::SymbolDetail::High;
	}
	mud::SymbolDetail DECL mud_SymbolDetail_Highest() {
		return mud::SymbolDetail::Highest;
	}
	
}


