#include <map>

#define LAMBDAS 0
#define MULTI_FUNC 1

namespace mud
{
namespace clgen
{
	inline vector<string> quote(const vector<string>& strings) { return transform<string>(strings, [](const string& s) { return "\"" + s + "\""; }); }
	inline string comma(const vector<string>& strings) { return join(strings, ", "); }

	string replace_any(const string& original, span<string> tokens, string with)
	{
		string result = original;
		for(const string& token : tokens)
			result = replace(result, token, with);
		return result;
	}

	string orflags(const string& type, span<bool> flags, span<string> labels, string none = "")
	{
		vector<string> vec;
		for(size_t i = 0; i < flags.size(); ++i)
			if(flags[i]) vec.push_back(labels[i]);
		if(vec.size() == 0) return none;
		else if(vec.size() == 1) return vec[0];
		else return type + "(" + join(vec, "|") + ")";
	}

	//string bind_prefix = "emscripten_bind_";
	static string bind_prefix = "";

	string clean_name(const string& name) { return replace(replace_any(name, { " ", "::", "<", "*" }, "_"), ">", ""); };
	string name(const CLPrimitive& parent, const string& name) { return bind_prefix + clean_name(parent.m_name) + "_" + name; };
	string name(const CLPrimitive& p) { return bind_prefix + clean_name(p.m_name); };
	string name(const CLPrimitive& p, size_t i) { return  name(p) + "_" + to_string(i); };

	string id(const CLPrimitive& parent, const string& name) { return bind_prefix + clean_name(parent.m_id) + "_" + name; };
	string id(const CLPrimitive& p) { return bind_prefix + clean_name(p.m_id); };
	string id(const CLPrimitive& p, size_t i) { return  id(p) + "_" + to_string(i); };

	const CLType& real_type(const CLType& t) { return t.m_type_kind == CLTypeKind::Alias ? real_type(*((CLAlias&)t).m_target) : t; }

	vector<string> location(const CLPrimitive& e)
	{
		if(e.m_parent->m_id == "") return {};
		else return split(e.m_parent->m_id, "::");
	}
	
	string namespace_begin(CLPrimitive& n)
	{
		if(n.m_name != "")
			return namespace_begin(*n.m_parent) + "namespace " + n.m_name + " {\n";
		else
			return "";
	}

	string namespace_end(CLPrimitive& n)
	{
		if(n.m_name != "")
			return namespace_end(*n.m_parent) + "}\n";
		else
			return "";
	}

	string clnamespace(const CLPrimitive& e)
	{
		if(e.m_parent->m_id == "")
			return "&namspc({})";
		else
			return "&namspc({ " + comma(quote(location(e))) + " })";
	}

	string type_get(const CLType& c) { return "type<" + c.m_id + ">()"; }
	string type_get(const CLQualType& t) { return "type<" + t.m_type_name + ">()"; }
	//string type_get(const CLQualType& t) { return type_get(*t.m_type); }

	string base_offset_get(const CLType& c, const CLType& base) { return "base_offset<" + c.m_id + ", " + base.m_id + ">()"; }

	string cast(const string& t, string name)
	{
		return string(t.back() == '*' ? "" : "*") + "static_cast<" + t + string(t.back() == '*' ? "" : "*") + ">(" + name + ")";
	}

	string cast(const CLQualType& t, string name)
	{
		if(t.iscstring()) return "static_cast<const char*>(" + name + ")";
		return string(t.pointer() ? "" : "*") + "static_cast<" + t.m_type_name + "*>(" + name + ")";
	}

	string cast(const CLType& t, string name)
	{
		return "(*static_cast<" + t.m_id + "*>(" + name + "))";
	}

	string enum_to_string_body(const CLEnum& e)
	{
		return "str = g_enu[" + type_get(e) + ".m_id]->name(uint32_t(" + cast(e, "val") + "));";
	}

	string enum_to_value_body(const CLEnum& e)
	{
		return cast(e, "val") + " = " + e.m_id + "(g_enu[" + type_get(e) + ".m_id]->value(str.c_str()));";
	}

#if LAMBDAS
	string enum_to_string_func(const CLEnum& e)
	{
		return "[](void* val, string& str) { " + enum_to_string_body(e) + " }";
	}

	string enum_to_value_func(const CLEnum& e)
	{
		return "[](const string& str, void* val) { " + enum_to_value_body(e) + " }";
	}
#else
	string enum_to_string_def(const CLEnum& e)
	{
		return "void " + id(e, "_to_string") + "(void* val, string& str) { " + enum_to_string_body(e) + " }";
	}

	string enum_to_value_def(const CLEnum& e)
	{
		return "void " + id(e, "_to_value") + "(const string& str, void* val) { " + enum_to_value_body(e) + " }";
	}

	string enum_to_string_func(const CLEnum& e) { return id(e, "_to_string"); }
	string enum_to_value_func(const CLEnum& e) { return id(e, "_to_value"); }
#endif

	string iterable_size_body(const CLType& t)
	{
		return "return " + cast(t, "vec") + ".size();";
	}

	string iterable_at_body(const CLType& t)
	{
		return "return &" + cast(t, "vec") + "[i];";
	}

	string sequence_push_body(const CLType& t, const string& e)
	{
		return cast(t, "vec") + ".emplace_back();";
	}

	string sequence_add_body(const CLType& t, const string& e)
	{
		return cast(t, "vec") + ".push_back(" + cast(e, "value") + ");";
	}

	string sequence_remove_body(const CLType& t, const string& e)
	{
		return "vector_remove_any(" + cast(t, "vec") + ", " + cast(e, "value") + ");";
	}

#if LAMBDAS
	string iterable_size_func(const CLType& t)
	{
		return "[](void* vec) { " + iterable_size_body(t) + " }";
	}

	string iterable_at_func(const CLType& t)
	{
		return "[](void* vec, size_t i) -> void* { " + iterable_at_body(t) + " }";
	}

	string sequence_add_func(const CLType& t, const string& e)
	{
		return "[](void* vec, void* value) { " + sequence_add_body(t, e) + " }";
	}

	string sequence_remove_func(const CLType& t, const string& e)
	{
		return "[](void* vec, void* value) { " + sequence_remove_body(t, e) + " }";
	}
#else
	string iterable_size_def(const CLType& t)
	{
		return "size_t " + id(t, "_size") + "(void* vec) { " + iterable_size_body(t) + " }";
	}

	string iterable_at_def(const CLType& t)
	{
		return "void* " + id(t, "_at") + "(void* vec, size_t i) { " + iterable_at_body(t) + " }";
	}

	string iterable_size_func(const CLType& t) { return id(t, "_size"); }
	string iterable_at_func(const CLType& t) { return id(t, "_at"); }

	string sequence_push_def(const CLType& t, const string& e)
	{
		return "void " + id(t, "_push") + "(void* vec) { " + sequence_push_body(t, e) + " }";
	}

	string sequence_add_def(const CLType& t, const string& e)
	{
		return "void " + id(t, "_add") + "(void* vec, void* value) { " + sequence_add_body(t, e) + " }";
	}

	string sequence_remove_def(const CLType& t, const string& e)
	{
		return "void " + id(t, "_remove") + "(void* vec, void* value) { " + sequence_remove_body(t, e) + " }";
	}

	string sequence_push_func(const CLType& t, const string& e) { return id(t, "_push"); }
	string sequence_add_func(const CLType& t, const string& e) { return id(t, "_add"); }
	string sequence_remove_func(const CLType& t, const string& e) { return id(t, "_remove"); }
#endif

	string return_type(const CLQualType& t)
	{
		if(t.m_spelling == "void")
			return "g_qvoid";
		else
			return "{ &" + type_get(t) + ", QualType::None }";
	}
	
	string value_assign(const CLQualType& t, string var, string value)
	{
		if(t.value() && !t.pointer())
			return cast(*t.m_type, var) + " = " + value;
		else
			return var + " = " + string(t.pointer() ? "" : "&") + value;
	}

	string oname(const CLCallable& f)
	{
		if(f.m_kind == CLPrimitiveKind::Constructor)
			return "construct_" + to_string(f.m_overload_index);
		else if(f.m_kind == CLPrimitiveKind::Method)
			return f.m_name + "_" + to_string(f.m_overload_index);
		else
			return f.m_name;
	}

	string param_default_name(const CLCallable& f, const CLParam& p)
	{
		string prefix = f.m_parent->m_kind == CLPrimitiveKind::Type ? oname(f) + "_" : "";
		return prefix + p.m_name + "_default";
	}

	string type_decl(const CLQualType& t)
	{
		return t.reference() && t.isconst() ? t.m_type_name : t.m_spelling;
	}

	string param_default_decl(const CLCallable& f, const CLParam& p)
	{
		return "static " + type_decl(p.m_type) + " " + param_default_name(f, p) + " = " + p.m_default + ";";
	}

	string param_default(const CLCallable& f, const CLParam& p)
	{
		return p.m_default == "" ? "" : ", &" + param_default_name(f, p);
	}

	string param_forward(const CLParam& p, size_t index)
	{
		bool move = p.m_type.m_type->m_move_only && p.m_type.value();
		string arg = cast(p.m_type, "args[" + to_string(index) + "]");
		return move ? "move(" + arg + ")" : arg;
	}

	string param_flags(const CLParam& p)
	{
		bool flags[] = { p.m_type.nullable(), p.m_output, p.m_has_default };
		string labels[] = { "Param::Nullable", "Param::Output", "Param::Default" };
		return orflags("Param::Flags", flags, labels, "");
	}

	string param_decl(const CLCallable& f, const CLParam& p)
	{
		return "{ \"" + p.m_name + "\", " + type_get(p.m_type) + ", " + param_flags(p) + param_default(f, p) + " }";
	}
	
	vector<string> params_types(const CLCallable& f)
	{
		return transform<string>(f.m_params, [](const CLParam& p) { return p.m_type.m_spelling; });
	}

	string params_def(const CLCallable& f)
	{
		vector<string> decls = transform<string>(f.m_params, [&](const CLParam& p) { return param_decl(f, p); });
		return f.m_params.size() > 0 ? "{ " + comma(decls) + " }" : "{}";
	}

	string function_signature(const CLCallable& f)
	{
		return f.m_return_type.m_spelling + "(*)(" + comma(params_types(f)) + ")";
	}

	string method_signature(const CLMethod& m)
	{
		return m.m_return_type.m_spelling + "(" + m.m_parent->m_id + "::*)(" + comma(params_types(m)) + ")" + (m.m_const ? " const" : "");
	}

	string function_identity(const CLCallable& f)
	{
		return "nullptr";// "funcptr<" + function_signature(f) + ">(&" + f.m_id + ")";
	}
	
	string function_return_def(const CLCallable& f)
	{
		return return_type(f.m_return_type);
	}

	string get_args(const vector<CLParam>& params)
	{
		vector<string> decls = transform<string>(params, [](const CLParam& p) { return param_forward(p, p.m_index); });
		return comma(decls);
	}

	string method_identity(const CLType& c, const CLMethod& m)
	{
		string signature = m.m_overloaded || true ? "<" + method_signature(m) + ">" : "";
		return "Address()"; // "member_address" + signature + "(&" + c.m_id + "::" + m.m_name + ")";
	}

	string method_pointer(const CLType& c, const CLMethod& m)
	{
		string pointer = "&" + c.m_id + "::" + m.m_name;
		return m.m_overloaded ? "static_cast<" + method_signature(m) + ">(" + pointer + ")" : pointer;
	}

	string field_identity(const CLType& c, const CLMember& m)
	{
		//return m.m_type.reference() ? "Address()" : "member_address(&" + c.m_id + "::" + m.m_member + ")";
		return m.m_type.reference() ? "SIZE_MAX" : "offsetof(" + c.m_id + ", " + m.m_member + ")";
	}

	string member_identity(const CLType& c, const CLMember& m)
	{
		return m.m_method ? "SIZE_MAX" : field_identity(c, m);
	}

	string member_flags(const CLMember& m)
	{
		bool flags[] = { m.m_type.pointer(), m.m_type.memvalue(), m.m_nonmutable, m.m_structure, m.m_component,	m.m_link || m.m_type.pointer() || m.m_type.reference() };
		string labels[] = { "Member::Pointer", "Member::Value", "Member::NonMutable", "Member::Structure", "Member::Component", "Member::Link" };
		return orflags("Member::Flags", flags, labels, "Member::None");
	}
	
	string unused_args(const CLCallable& f)
	{
		return f.m_params.size() == 0 ? "UNUSED(args); " : "";
	}

	string type_class(const CLType& c)
	{
		if(c.m_struct)
			return "TypeClass::Struct";
		else
			return "TypeClass::Object";
	}

	string member_type(const CLMember& m)
	{
		return m.m_type.m_type_name + (m.m_type.pointer() ? "*" : "");
	}
	
	string getter_body(const CLType& c, const CLMember& m)
	{
		if(m.m_method)
			return "return " + string(m.m_type.iscstring() ? "(void*)" : "") + string(!m.m_type.pointer() ? "&" : "") + cast(c, "object") + "." + m.m_member + "();";
		else //if(m.m_type.reference())
			return "return &" + cast(c, "object") + "." + m.m_member + ";";
	}

#if LAMBDAS
	string getter_lambda(const CLType& c, const CLMember& m)
	{
		return "[](void* object) -> void* { " + getter_body(c, m) + " }";
	}

	string getter_func(const CLType& c, const CLMember& m)
	{
		return m.m_method || m.m_type.reference() ? getter_lambda(c, m) : "nullptr";
	}
#else
	string getter_def(const CLType& c, const CLMember& m)
	{
		return "void* " + id(c, "_get_" + m.m_name) + "(void* object) { " + getter_body(c, m) + " }";
	}

	string getter_func(const CLType& c, const CLMember& m)
	{
		return m.m_method || m.m_type.reference() ? id(c, "_get_" + m.m_name) : "nullptr";
	}
#endif

	string setter_body(const CLType& c, const CLMember& m)
	{
		if(m.m_setter)
			return cast(c, "object") + ".set" + m.m_capname + "(" + cast(m.m_type, "v") + ");";
		else
			return cast(c, "object") + "." + m.m_member + " = " + cast(m.m_type, "v") + ";";
	}

#if LAMBDAS
	string setter_lambda(const CLType& c, const CLMember& m)
	{
		return "[](void* object, void* v) { " + setter_body(c, m) + " }";
	}

	string setter_func(const CLType& c, const CLMember& m)
	{
		return m.m_setter || !m.m_nonmutable ? setter_lambda(c, m) : "nullptr";
	}
#else
	string setter_def(const CLType& c, const CLMember& m)
	{
		return "void " + id(c, "_set_" + m.m_name) + "(void* object, void* v) { " + setter_body(c, m) + " }";
	}

	string setter_func(const CLType& c, const CLMember& m)
	{
		return m.m_setter || !m.m_nonmutable ? id(c, "_get_" + m.m_name) : "nullptr";
	}
#endif

	string member_default_decl(const CLType& c, const CLMember& m)
	{
		UNUSED(c);
		return  "static " + type_decl(m.m_type) + " " + m.m_name + "_default = " + m.m_default + ";";
	}

	string member_default(const CLType& c, const CLMember& m)
	{
		UNUSED(c);
		return m.m_default != "" ? string(m.m_type.pointer() ? "" : "&") + m.m_name + "_default" : "nullptr";
	}

	string method_body(const CLType& c, const CLMethod& m)
	{
		string call = m.m_function ? m.m_function->m_id + "(" + cast(c, "object") + (m.m_params.size() > 0 ? ", " + get_args(m.m_params) : "") + ")"
								   : cast(c, "object") + "." + m.m_name + "(" + get_args(m.m_params) + ")";
		
		if(m.m_return_type.isvoid())
			return "UNUSED(result); " + unused_args(m) + call + ";";
		else
			return unused_args(m) + value_assign(m.m_return_type, "result", call) + ";";
	}

#if LAMBDAS
	string method_lambda(const CLType& c, const CLMethod& m)
	{
		return "[](void* object, span<void*> args, void*& result) { " + method_body(c, m) + " }";
	}

	string method_func(const CLType& c, const CLMethod& m)
	{
		return method_lambda(c, m);
	}
#else
	string method_def(const CLType& c, const CLMethod& m)
	{
		return "void " + id(c, m.m_name) + "(void* object, span<void*> args, void*& result) { " + method_body(c, m) + " }";
	}

	string method_func(const CLType& c, const CLMethod& m)
	{
		return id(c, m.m_name);
	}
#endif

	string constructor_body(const CLType& c, const CLConstructor& ctor)
	{
		return unused_args(ctor) + "new(stl::placeholder(), ref) " + c.m_id + "( " + get_args(ctor.m_params) + " );";
	}

#if LAMBDAS
	string constructor_lambda(const CLType& c, const CLConstructor& ctor)
	{
		return "[](void* ref, span<void*> args) { " + constructor_body(c, ctor) + " }";
	}

	string constructor_func(const CLType& c, const CLConstructor& ctor)
	{
		return constructor_lambda(c, ctor);
	}
#else
	string constructor_def(const CLType& c, const CLConstructor& ctor)
	{
		return "void " + id(c, "_construct_" + to_string(ctor.m_overload_index)) + "(void* ref, span<void*> args) { " + constructor_body(c, ctor) + " }";
	}

	string constructor_func(const CLType& c, const CLConstructor& ctor)
	{
		return id(c, "_construct_" + to_string(ctor.m_overload_index));
	}
#endif

	string copy_constructor_body(const CLType& c)
	{
		return "new(stl::placeholder(), ref) " + c.m_id + "(" + cast(c, "other") + ");";
	}

#if LAMBDAS
	string copy_constructor_lambda(const CLType& c)
	{
		return "[](void* ref, void* other) { " + copy_constructor_body(c) + " }";
	}

	string copy_constructor_func(const CLType& c)
	{
		return copy_constructor_lambda(c);
	}
#else
	string copy_constructor_def(const CLType& c)
	{
		return "void " + id(c, "_copy_construct") + "(void* ref, void* other) { " + copy_constructor_body(c) + " }";
	}

	string copy_constructor_func(const CLType& c)
	{
		return id(c, "_copy_construct");
	}
#endif

	string constructor_decl(const CLType& c, const CLConstructor& ctor)
	{
		return "t, " + constructor_func(c, ctor) + ", " + params_def(ctor);
	}

	string copy_constructor_decl(const CLType& c)
	{
		return "t, " + copy_constructor_func(c);
	}

	string member_decl(const CLType& c, const CLMember& m)
	{
		return "t, " + member_identity(c, m) + ", " + type_get(m.m_type) + ", \"" + m.m_name + "\", " + member_default(c, m) + ", " + member_flags(m) + ", " + getter_func(c, m);
	}

	string method_decl(const CLType& c, const CLMethod& m)
	{
		return "t, \"" + m.m_name + "\", " + method_identity(c, m) + ", " + method_func(c, m) + ", " + params_def(m) + ", " + function_return_def(m);
	}

	string static_decl(const CLType& c, const CLStatic& m)
	{
		return "t, \"" + m.m_name + "\", Ref(&" + c.m_id + "::" + m.m_member + ")";
	}

	string meta_decl(const CLType& c, const string& type_class)
	{
		string size = c.m_name == "void" ? "0" : "sizeof(" + c.m_id + ")";
		return "static Meta meta = { t, " + clnamespace(c) + ", \"" + c.m_name + "\", " + size + ", " + type_class + " };";
	}

	string function_body(const CLCallable& f)
	{
		if(f.m_return_type.isvoid())
			return "UNUSED(result); " + unused_args(f) + " " + f.m_id + "(" + get_args(f.m_params) + ");";
		else
			return unused_args(f) + value_assign(f.m_return_type, "result", f.m_id + "(" + get_args(f.m_params) + ")") + ";";
	}

#if LAMBDAS
	string function_lambda(const CLFunction& f)
	{
		return "[](span<void*> args, void*& result) { " + function_body(f) + " }";
	}

	string function_func(const CLFunction& f)
	{
		return function_lambda(f);
	}
#else
	string function_def(const CLFunction& f)
	{
		return "void " + id(f, f.m_index) + "(span<void*> args, void*& result) { " + function_body(f) + " }";
	}

	string function_func(const CLFunction& f)
	{
		return id(f, f.m_index);
	}
#endif

	string function_decl(const CLFunction& f)
	{
		return clnamespace(f) + ", \"" + f.m_name + "\", " + function_identity(f) + ", " + function_func(f) + ", " + params_def(f) + ", " + function_return_def(f);
	}

	void write_line(string& t, int& i, const string& s, bool spaces = false, bool noendl = false)
	{
		if(s[0] == '}') --i;
		if(t.back() == '\n')
			for(size_t c = 0; c < i; ++c) t += spaces ? "    " : "\t";
		t += s;
		if(!noendl) t += "\n";
		for(size_t c = 1; c < s.size(); ++c) { if(s[c] == '{') ++i; if(s[c] == '}') --i; }
		if(s[0] == '{') ++i;
	}

	string forward_h_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s); };

		p("#pragma once");

		p("#include <infra/Config.h>");

		for(CLModule* d : m.m_dependencies)
			p("#include <" + d->m_subdir + "/Forward.h>");

		p("#ifndef " + m.m_export);
		p("#define " + m.m_export + " MUD_IMPORT");
		p("#endif");

		for(auto& key_namespace : m.m_namespaces)
		{
			CLNamespace& n = *key_namespace.second;
			
			p(namespace_begin(n));
			for(auto& e : m.m_enums)
				if(e->m_parent == &n)
					p("enum " + e->m_scoped ? "class " : "" + e->m_name + " : " + e->m_enum_type + ";");
        
			//for(auto& c : m.m_class_templates)
			//	if(c->m_parent == &n && c->m_template_used)
			//		p("template <class T> " + string(c->m_struct ? "struct" : "class") + " } " + c->m_template_name + ";");

			for(auto& c : m.m_classes)
				if(c->m_parent == &n && !c->m_nested && !c->m_is_templated && !c->m_extern)
					p(string(c->m_struct ? "struct" : "class") + " " + c->m_name + ";");
				else if(c->m_is_template)
					p("template <class T> " + string(c->m_struct ? "struct" : "class") + " " + c->m_id + ";");

			p(namespace_end(n));
		}

		return t;
	}

	string convert_h_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s); };

		p("#pragma once");
		p("");
		p("#include <" + m.m_subdir + "/Types.h>");
		p("");
		p("#if !defined MUD_MODULES || defined MUD_TYPE_LIB");
		p("#include <refl/Meta.h>");
		p("#include <refl/Enum.h>");
		p("#include <infra/StringOps.h>");
		p("#endif");
		p("");
		p("namespace mud");
		p("{");
		for(auto& e : m.m_enums)
		if(e->m_reflect)
		{
			p("export_ template <> inline void to_value(const string& str, " + e->m_id + "& val) { val = " + e->m_id + "(enu<" + e->m_id + ">().value(str.c_str())); };");
			p("export_ template <> inline void to_string(const " + e->m_id + "& val, string& str) { str = enu<" + e->m_id + ">().name(uint32_t(val)); };");
			p("");
		}
		p("");
		p("}");

		return t;
	}

	string module_h_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s); };
		auto s = [&](const string& s) { i--; write_line(t, i, s); i++; };

		p("#pragma once");
		p("");
		p("#if !defined MUD_MODULES || defined MUD_TYPE_LIB");
		p("#include <refl/Module.h>");
		p("#endif");
		p("");
		p("#include <" + m.m_subdir + "/Forward.h>");
		p("//#include <" + m.m_subdir + "/Types.h>");
		p("");
		p("#ifndef " + m.m_refl_export);
		p("#define " + m.m_refl_export + " MUD_IMPORT");
		p("#endif");
		p("");
		if(m.m_namespace != "")
		{
			p("namespace " + m.m_namespace);
			p("{");
		}
		p("export_ class " + m.m_refl_export + " " + m.m_id + " : public mud::Module");
		p("{");
		s("private:");
		p(m.m_id + "();");
		p("");
		s("public:");
		p("static " + m.m_id + "& m() { static " + m.m_id + " instance; return instance; }");
		p("};");
		if(m.m_namespace != "")
			p("}");
		p("");
		p("#ifdef " + m.m_preproc_name + "_MODULE");
		p("extern \"C\"");
		p(m.m_refl_export + " Module& getModule();");
		p("#endif");

		return t;
	}

	string module_cpp_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s); };
		auto s = [&](const string& s) { i--; write_line(t, i, s); i++; };

		auto deps = [&](){ return comma(transform<string>(m.m_dependencies, [](CLModule* d) { return "&" + d->m_id + "::m()"; })); };

		auto empty_val_decl = [&](const CLType& t)
		{
			p("g_meta[t.m_id]->m_empty_var = var(" + t.m_id + "());");
		};

		auto convert_decl = [&](const CLType& t)
		{
			p("static Convert convert = { [](void* val, string& str) { to_string<" + t.m_id + ">(" + cast(t, "val") + ", str); }, ");
			s("                           [](const string& str, void* val) { to_value<" + t.m_id + ">(str, " + cast(t, "val") + "); } };");
			p("g_convert[t.m_id] = &convert;");
		};

		auto enum_convert_decl = [&](const CLEnum& t)
		{
			p("static Convert convert = { " + enum_to_string_func(t) + ",");
			s("                           " + enum_to_value_func(t) + " };");
			p("g_convert[t.m_id] = &convert;");
		};

		auto iterable_decl = [&](const CLType& t, const CLType& et, const string& e)
		{
			p("static Iterable iterable = { &" + type_get(et) + ",");
			s("                             " + iterable_size_func(t) + ",");
			s("                             " + iterable_at_func(t) + "};");
			p("g_iterable[t.m_id] = &iterable;");
		};

		auto sequence_decl = [&](const CLType& t, const string& e)
		{
			p("static Sequence sequence = { " + sequence_push_func(t, e) + ",");
			s("                             " + sequence_add_func(t, e) + ",");
			s("                             " + sequence_remove_func(t, e) + " };");
			p("g_sequence[t.m_id] = &sequence;");
		};

		p("#include <infra/Cpp20.h>");
		p("");
		p("#ifdef MUD_MODULES");
		p("module " + m.m_namespace + "." + m.m_name + ";");
		p("#else");
		if(m.m_has_reflected)
		{
			p("#include <cstddef>");
			p("#include <stl/new.h>");
			p("#include <infra/ToString.h>");
			p("#include <infra/ToValue.h>");
			if(m.m_decl_basetypes)
				p("#include <type/Any.h>");
			p("#include <type/Vector.h>");
			p("#include <refl/MetaDecl.h>");
		}
		p("#include <refl/Module.h>");
		for(CLModule* d : m.m_dependencies)
			p("#include <meta/" + d->m_dotname + ".meta.h>");
		p("#include <meta/" + m.m_dotname + ".meta.h>");
		if(m.m_has_reflected)
			p("#include <meta/" + m.m_dotname + ".conv.h>");
		p("#endif");
		p("");

		if(m.m_has_reflected)
		{
		p("#include <" + m.m_subdir + "/Api.h>");
		p("");
		p("using namespace mud;");
		p("");

#if !LAMBDAS
		for(auto& pe : m.m_enums)
			if(pe->m_reflect)
			{
				CLEnum& e = *pe;
				p(enum_to_string_def(e));
				p(enum_to_value_def(e));
			}

		for(auto& ps : m.m_sequences)
			if(ps->m_reflect)
			{
				CLClass& s = *ps;
				p(iterable_size_def(s));
				p(iterable_at_def(s));
				if(s.m_name.find("vector") != string::npos)
				{
					p(sequence_push_def(s, s.m_element));
					p(sequence_add_def(s, s.m_element));
					p(sequence_remove_def(s, s.m_element));
				}
			}

		for(auto& pc : m.m_classes)
			if(pc->m_reflect)
			{
				CLClass& c = *pc;
				for(CLConstructor& ctor : c.m_constructors)
				{
					p(constructor_def(c, ctor));
				}
				if(c.m_struct && !c.m_move_only)
					p(copy_constructor_def(c));
				for(CLMember& a : c.m_members)
				{
					if(a.m_method || a.m_type.reference())
						p(getter_def(c, a));
					//if(a.m_setter || !a.m_nonmutable)
					//	p(setter_def(c, a));
				}
				for(CLMethod& f : c.m_methods)
				{
					p(method_def(c, f));
				}
				for(CLStatic& s : c.m_statics)
				{
				}
			}
		
		for(auto& pf : m.m_functions)
			if(pf->m_reflect)
			{
				CLFunction& f = *pf;
				p(function_def(f));
			}
#endif

		p("");
		p("namespace mud");
		p("{");

		p("void " + m.m_id + "_meta(Module& m)");
		p("{");

		i--;

		p("UNUSED(m);");
		p("");

		p("// Base Types");
		for(auto& pb : m.m_basetypes)
			if(pb->m_reflect)
			{
				CLBaseType& b = *pb;
				p("{");
				p("Type& t = " + type_get(b) + ";");
				p(meta_decl(b, "TypeClass::BaseType"));
				if(b.m_id != "void" && b.m_id != "void*" && b.m_id != "const char*")
				{
					empty_val_decl(b);
					convert_decl(b);
				}
				p("}");
			}
		p("");


		p("// Enums");
		for(auto& pe : m.m_enums)
			if(pe->m_reflect)
			{
				auto toarr = [](const string& name, size_t size) { return "{ " + name + ", " + to_string(size) + " }"; };
				CLEnum& e = *pe;
				p("{");
				p("Type& t = " + type_get(e) + ";");
				p(meta_decl(e, "TypeClass::Enum"));
				p("static cstring ids[] = { " + comma(quote(e.m_ids)) + " };");
				p("static uint32_t values[] = { " + comma(e.m_values) + " };");
				p("static " + e.m_id + " vars[] = { " + comma(e.m_scoped_ids) + "};");
				p("static void* refs[] = { " + comma(transform<string>(0, e.m_count, [](size_t i) { return "&vars[" + to_string(i) + "]"; })) + "};");
				p("static Enum enu = { t, " + string(e.m_scoped ? "true" : "false") + ", ids, values, refs };");
				//empty_val_decl(e);
				enum_convert_decl(e);
				p("}");
			}
		p("");

		p("// Sequences");
		for(auto& ps : m.m_sequences)
			if(ps->m_reflect)
			{
				CLClass& s = *ps;
				p("{");
				p("Type& t = " + type_get(s) + ";");
				p(meta_decl(s, "TypeClass::Sequence"));
				p("static Class cls = { t };");
				iterable_decl(s, *s.m_element_type, s.m_element);
				if(s.m_name.find("vector") != string::npos)
					sequence_decl(s, s.m_element);
				//else
				//	p("meta_sequence<" + s.m_id + ", " + s.m_element + ">();");
				p("}");
			}
		p("");

		for(auto& pc : m.m_classes)
			if(pc->m_reflect)
			{
				CLClass& c = *pc;
				p("// " + c.m_id);
				p("{");
				p("Type& t = " + type_get(c) + ";");
				p(meta_decl(c, type_class(c)));
				p("// bases");
				if(c.m_bases.size() > 0)
				{
					p("static Type* bases[] = { " + comma(transform<string>(c.m_bases, [&](CLType* base) { return "&" + type_get(*base); })) + " };");
					p("static size_t bases_offsets[] = { " + comma(transform<string>(c.m_bases, [&](CLType* base) { return base_offset_get(c, *base); })) + " };");
				}
				p("// defaults");
				for(CLMember& a : c.m_members)
					if(a.m_default != "")
						p(member_default_decl(c, a));
				for(CLConstructor& f : c.m_constructors)
					for(const CLParam& a : f.m_params)
						if(a.m_has_default)
							p(param_default_decl(f, a));
				for(CLMethod& f : c.m_methods)
					for(const CLParam& a : f.m_params)
						if(a.m_has_default)
							p(param_default_decl(f, a));
				p("// constructors");
				if(c.m_constructors.size() > 0)
				{
					p("static Constructor constructors[] = {");
					for(CLConstructor& ctor : c.m_constructors)
						p("{ " + constructor_decl(c, ctor) + " }" + (&ctor == &c.m_constructors.back() ? "" : ","));
					p("};");
				}
				p("// copy constructor");
				if(c.m_struct && !c.m_move_only)
				{
					p("static CopyConstructor copy_constructor[] = {");
					p("{ " + copy_constructor_decl(c) + " }");
					p("};");
				}
				p("// members");
				if(c.m_members.size() > 0)
				{
					p("static Member members[] = {");
					for(CLMember& a : c.m_members)
						p("{ " + member_decl(c, a) + " }" + (&a == &c.m_members.back() ? "" : ","));
					p("};");
				}
				p("// methods");
				if(c.m_methods.size() > 0)
				{
					p("static Method methods[] = {");
					for(CLMethod& f : c.m_methods)
						p("{ " + method_decl(c, f) + " }" + (&f == &c.m_methods.back() ? "" : ","));
					p("};");
				}
				p("// static members");
				if(c.m_statics.size() > 0)
				{
					p("static Static statics[] = {");
					for(CLStatic& s : c.m_statics)
						p("{ " + static_decl(c, s) + " }" + (&s == &c.m_statics.back() ? "" : ","));
					p("};");
				}
				p("static Class cls = { t, "
					+ string(c.m_bases.size() > 0 ? "bases, bases_offsets, " : "{}, {}, ")
					+ string(c.m_constructors.size() > 0 ? "constructors, " : "{}, ")
					+ string(c.m_struct && !c.m_move_only ? "copy_constructor, " : "{}, ")
					+ string(c.m_members.size() > 0 ? "members, " : "{}, ")
					+ string(c.m_methods.size() > 0 ? "methods, " : "{}, ")
					+ string(c.m_statics.size() > 0 ? "statics, " : "{}, ")
					+ "};"
				);
				//if(c.m_constructors.size() > 0 && !c.m_struct)
					//p("init_pool<" + c.m_id + ">();");
				//if(c.m_array)
					//p("init_string<" + c.m_id + ">();");
				//p("meta_class<" + c.m_id + ">();");
				p("}");
			}
		p("");

		for(auto& pa : m.m_aliases)
			if(pa->m_reflect)
			{
				CLAlias& a = *pa;
				p("{");
				p("Type& t = " + type_get(*a.m_target) + ";");
				p("static Alias alias = { &t, " + clnamespace(a) + ", \"" + a.m_name + "\" };");
				p("m.m_aliases.push_back(&alias);");
				p("}");
			}
		p("");

		i++;

		for(CLType* ty : m.m_types)
			if(ty->m_reflect)
				p("m.m_types.push_back(&" + type_get(*ty) + ");");

		for(auto& pf : m.m_functions)
			if(pf->m_reflect)
			{
				CLFunction& f = *pf;
				p("{");
				for(const CLParam& a : f.m_params)
					if(a.m_has_default)
						p(param_default_decl(f, a));
				p("static Function f = { " + function_decl(f) + " };");
				p("m.m_functions.push_back(&f);");
				p("}");
			}

		p("}");
		p("}");
		p("");
		}

		if(m.m_namespace != "")
		{
			p("namespace " + m.m_namespace);
			p("{");
		}
		p(m.m_id + "::" + m.m_id + "()");
		p("\t: Module(\"" + string(m.m_namespace != "" ? m.m_namespace + "::" : "") + m.m_name + "\", { " + deps() + " })");
		if(m.m_has_reflected)
		{
			p("{");
			p("// setup reflection meta data");
			p(m.m_id + "_meta(*this);");
			p("}");
		}
		else
		{
			p("{}");
		}
		if(m.m_namespace != "")
			p("}");
		p("");
		p("#ifdef " + m.m_preproc_name + "_MODULE");
		p("extern \"C\"");
		p("Module& getModule()");
		p("{");
		p("	return " + m.m_id + "::m();");
		p("}");
		p("#endif");

		return t;
	}

	string types_h_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s, true); };

		p("#pragma once");
		p("");
		p("#include <stdint.h>");
		p("#include <stl/string.h>");
		p("#include <stl/vector.h>");
		p("#include <" + m.m_subdir + "/Forward.h>");
		p("");
		p("#if !defined MUD_MODULES || defined MUD_TYPE_LIB");
		p("#include <type/Type.h>");
		p("#endif");
		p("");
		p("#ifndef MUD_MODULES");
		for(auto& d : m.m_dependencies)
			p("#include <" + d->m_subdir + "/Types.h>");
		p("#endif");
		p("");
		if(m.m_has_structs)
			p("#include <" + m.m_subdir + "/Structs.h>");
		p("");
		p("namespace mud");
		p("{");
		p("// Exported types");
		for(auto& b : m.m_basetypes)
			if(b->m_reflect)
				p("export_ template <> " + m.m_export + " Type& type<" + b->m_name + ">();");
		for(auto& e : m.m_enums)
			if(e->m_reflect) // !e->m_nested || 
				p("export_ template <> " + m.m_export + " Type& type<" + e->m_id + ">();");
		p("");
		for(auto& c : m.m_sequences)
			if(c->m_reflect && !c->m_nested && c->m_id != "mud::Type")
				p("export_ template <> " + m.m_export + " Type& type<" + c->m_id + ">();");
		p("");
		for(auto& c : m.m_classes)
			if(c->m_reflect && !c->m_nested && c->m_id != "mud::Type")
				p("export_ template <> " + m.m_export + " Type& type<" + c->m_id + ">();");
		//for(auto& c : m.m_classes)
		//	if(c->m_reflect && !c->m_nested && c->m_id != "mud::Type")
		//		p("export_ template <> " + m.m_export + " Type& type<vector<" + c->m_id + "*>>();");
		p("}");

		return t;
	}

	string types_cpp_template(CLModule& m)
	{
		string t;
		int i = 0;
		auto p = [&](const string& s) {	write_line(t, i, s, true); };

		auto type_decl_name = [](const string& name, const string& id)
		{
			return "static Type ty(\"" + name + "\"" + (id == "void" ? "" : ", sizeof(" + id + ")") + "); return ty;";
		};

		auto type_decl = [](const CLType& t)
		{
			return "static Type ty(\"" + t.m_name + "\"" + (t.m_bases.size() > 0 ? ", " + type_get(*t.m_bases[0]) : "") + ", sizeof(" + t.m_id + ")); return ty;";
		};

		p("#include <infra/Cpp20.h>");
		p("");
		p("#ifdef MUD_MODULES");
		p("module " + m.m_namespace + "." + m.m_name + ";");
		p("#else");
		p("#include <" + m.m_subdir + "/Types.h>");
		p("#include <" + m.m_subdir + "/Api.h>");
		p("#include <type/Vector.h>");
		p("#endif");
		p("");
		p("namespace mud");
		p("{");
		p("// Exported types");
		for(auto& b : m.m_basetypes)
			if(b->m_reflect)
				p("template <> " + m.m_export + " Type& type<" + b->m_name + ">() { " + type_decl_name(b->m_name, b->m_id) + " }");
		for(auto& e : m.m_enums)
			if(e->m_reflect)
				p("template <> " + m.m_export + " Type& type<" + e->m_id + ">() { " + type_decl_name(e->m_name, e->m_id) + " }");
		p("");
		for(auto& c : m.m_sequences)
			if(c->m_reflect)
				p("template <> " + m.m_export + " Type& type<" + c->m_id + ">() { " + type_decl(*c) + " }");
		p("");
		for(auto& c : m.m_classes)
			if(c->m_reflect && !c->m_nested && c->m_id != "mud::Type")
				p("template <> " + m.m_export + " Type& type<" + c->m_id + ">() { " + type_decl(*c) + " }");
		//for(auto& c : m.m_classes)
		//	if(c->m_reflect && !c->m_nested && c->m_id != "mud::Type")
		//		p("template <> " + m.m_export + " Type& type<vector<" + c->m_id + "*>>() { " + type_decl_name("vector<" + c->m_id + "*>", "vector<" + c->m_id + "*>") + " }");
		p("}");

		return t;
	}

	export_ template <class T>
	inline vector<T> vector_slice(const vector<T>& vector, size_t begin, size_t end)
	{
		vector<T> result;
		for(size_t i = begin; i < end; ++i)
			result.push_back(vector[i]);
		return result;
	}

	string js_namespace_path(const CLModule& m, const CLPrimitive& n)
	{
		if(n.m_name != "" && n.m_name != m.m_namespace)
			return js_namespace_path(m, *n.m_parent) + "['" + name(n) + "']";
		else
			return "";
	};

	void bind_javascript(CLModule& m)
	{
		// @todo:
		// - static members
		// - vectors
		// - span members
		// - span types
		// - noncopy types
		// - handle types
		// - references to strings

		// not implemented yet
		vector<string> blacklist = { "vector", "span", "mud::Complex" };

		auto blacklist_type = [&](const CLQualType& t) { for(const string& n : blacklist) { if(t.m_spelling.find(n) != string::npos) return true; } if(t.isstring() && t.reference() && !t.isconst()) return true; return false; };
		auto blacklist_class = [&](const CLClass& c) { for(const string& n : blacklist) { if(c.m_id.find(n) != string::npos) return true; } return false; };
		auto blacklist_callable = [&](const CLCallable& f) { for(const CLParam& p : f.m_params) if(blacklist_type(p.m_type)) return true; if(blacklist_type(f.m_return_type)) return true; return false; };
		auto blacklist_member = [&](const CLMember& m) { if(blacklist_type(m.m_type)) return true; return false; };

		string module_js;

		string ct;
		string jst;

		int ci = 0;
		int jsi = 0;

		auto cw = [&](const string& s) { write_line(ct, ci, s); };
		auto jsw = [&](const string& s, bool noendl = false) { write_line(jst, jsi, s, true, noendl); };

		for(CLModule* d : m.m_modules)
			cw("#include <" + d->m_subdir + "/Api.h>");
		cw("");
		cw("#ifdef MUD_PLATFORM_EMSCRIPTEN");
		cw("#include <emscripten.h>");
		cw("#define DECL EMSCRIPTEN_KEEPALIVE");
		cw("#else");
		cw("#define DECL");
		cw("#endif");
		cw("");
		cw("");
		cw("extern \"C\" {");
		cw("");

#if 0
		cw("// Not using size_t for span indices as the values used by the javascript code are signed.");
		cw("void array_bounds_check(const int array_size, const int array_idx) {");
		cw("  if(array_idx < 0 || array_idx >= array_size) {");
		cw("    EM_ASM({");
		cw("      throw 'Array index ' + $0 + ' out of bounds: [0,' + $1 + ')';");
		cw("    }, array_idx, array_size);");
		cw("  }");
		cw("}");
#endif

		auto address = [](const CLQualType& t) -> string {
			return (!t.isbasetype() && !t.isenum()) && (t.reference() || t.value()) ? "&" : "";
		};

		auto value = [](const CLQualType& t) -> string {
			return (!t.isbasetype() && !t.isenum()) && (t.reference() || t.value()) ? "*" : "";
		};

		auto type_to_c = [&](const CLQualType& t, bool non_pointing = false, bool no_array = true) -> string
		{
			if(t.isstring()) return "const char*";
			if(t.isenum()) return t.m_type_name;
			string name = !t.isbasetype() ? t.m_type_name + (non_pointing ? "" : "*") : t.m_type_name;
			return (t.isconst() && name != "const char*" ? "const " : "") + name + (!no_array && t.isarray() ? "[]" : "");
		};

		/*auto type_to_cdec = [&](const CLQualType& raw)
		{
			string ret;
			string name = ret = type_to_c(raw.m_type->m_name, true);
			if(raw.isconst()) ret = "const " + ret;
			//if name not in interfaces: return ret
			if(raw.reference()) return ret + "&";
			if(raw.value()) return ret;
			return ret + "*";
		};*/

		string js_impl_methods;

		struct Overloads { string name; std::map<size_t, const CLCallable*> sigs; };
		using OverloadMap = vector<Overloads>;

		auto cramfunc = [](Overloads& o, const CLCallable& f)
		{
			for(size_t i = f.m_min_args; i <= f.m_params.size(); ++i)
				o.sigs.insert({ i, &f });
		};

		auto first_overload = [&](OverloadMap& overloads, const CLCallable& f) -> Overloads*
		{
			for(Overloads& o : overloads)
				if(o.name == f.m_name)
				{
					return &o;
				}
			Overloads& o = push(overloads);
			o.name = f.m_name;
			cramfunc(o, f);
			return nullptr;
		};

		auto overload = [&](OverloadMap& overloads, const CLCallable& f)
		{
			Overloads* fo = first_overload(overloads, f);
			if(fo == nullptr) return;

			Overloads& o = *fo;
			bool clashes = false;
			//size_t max_args = (*(o.sigs.end()--)).first;
			for(size_t i = f.m_min_args; i < f.m_params.size(); ++i)
				if(o.sigs.find(i) != o.sigs.end())
					clashes = true;
			if(!clashes)// && f.m_params.size() > max_args)
				cramfunc(o, f);
			else
				printf("WARNING: can't bind %s%s (can only overload signatures of different lengths)\n", f.m_parent->m_id.c_str(), f.m_name.c_str());
		};

		auto c_forward_arg = [&](const CLParam& p)
		{
			if(p.m_type.istypedptr())
				return "{ " + p.m_name + ", *" + p.m_name + "_type" + " }";
			else
				return value(p.m_type) + p.m_name;
		};

		auto c_forward_args = [&](const CLCallable& f, size_t n)
		{
			return transform<string>(0, n, [&](size_t i) { return c_forward_arg(f.m_params[i]); });
		};

		auto c_typed_arg = [&](const CLParam& p)
		{
			if(p.m_type.istypedptr())
				return "void* " + p.m_name + ", " + "mud::Type* " + p.m_name + "_type";
			else
				return type_to_c(p.m_type) + " " + p.m_name;
		};

		auto c_typed_args = [&](const CLCallable& f, size_t i)
		{
			vector<string> args = transform<string>(0, i, [&](size_t i) { return c_typed_arg(f.m_params[i]); });
			return f.m_kind == CLPrimitiveKind::Method ? merge({ f.m_parent->m_id + "* self" }, args)
													   : args;
		};

		auto js_namespace = [&](const CLPrimitive& n)
		{
			string path = "Module" + js_namespace_path(m, n);
			jsw(path + " = " + path + " || {};");
		};

		auto js_module_path = [&](const CLModule& m, const CLPrimitive& p)
		{
			return "Module" + js_namespace_path(m, p);
		};

#if MULTI_FUNC
		auto js_call_convert_arg = [&](const CLQualType& t, const string& a, bool no_array = false)
		{
			if(t.iscstring() || t.isstring()) return "ensureString(" + a + ")";
			else if(t.istypedptr()) return "ensureRef(" + a + "), ensureRefType(" + a + ")";
			else if(t.isclass()) return a + ".ptr";
			else if(t.isarray() && !no_array)
			{
				if(has({ "char", "unsigned char" }, t.m_type->m_name)) return "ensureInt8(" + a + ")";
				else if(has({ "short", "unsigned short" }, t.m_type->m_name)) return "ensureInt16(" + a + ")";
				else if(has({ "int", "unsigned int" }, t.m_type->m_name)) return "ensureInt32(" + a + ")";
				else if(has({ "float" }, t.m_type->m_name)) return "ensureFloat32(" + a + ")";
				else if(has({ "double" }, t.m_type->m_name)) return "ensureFloat64(" + a + ")";
			}
			else return a;
		};
#endif
		
		auto js_signature_args = [](const CLCallable& f, size_t n)
		{
			return transform<string>(0, n, [&](size_t i) { return "a" + to_string(i); });
		};

		auto js_forward_arg = [&](const CLCallable& f, const CLParam& p, size_t i)
		{
			string name = "/*" + p.m_name + "*/" + "a" + to_string(i);
			return js_call_convert_arg(p.m_type, name);
		};

		auto js_forward_args = [&](const CLCallable& f, size_t n)
		{
			vector<string> args = transform<string>(0, n, [&](size_t i) { return js_forward_arg(f, f.m_params[i], i); });
			return f.m_kind == CLPrimitiveKind::Method ? merge({ "self" }, args)
													   : args;
		};

		auto js_call_inner = [&](const CLCallable& f, size_t n, const string& name)
		{
			return "_" + name + "(" + comma(js_forward_args(f, n)) + ")";
		};

		auto js_call_return_wrap = [&](const CLQualType& return_type, const string& call)
		{
			if(return_type.isclass())
				return "return wrapPointer(" + call + ", " + name(*return_type.m_type) + ");";
			else if(return_type.iscstring() || return_type.isstring())
				return "return Pointer_stringify(" + call + ");";
			else if(return_type.isboolean())
				return "return !!(" + call + ");";
			else if(!return_type.isvoid())
				return "return " + call + ";";
			else
				return call + ";";
		};

		auto js_call_func = [&](const CLCallable& f, size_t n)
		{
			return js_call_return_wrap(f.m_return_type, js_call_inner(f, n, id(f, n)));
		};

		auto js_call_constructor = [&](const CLCallable& f, size_t n)
		{
			string call = "this.ptr = " + js_call_inner(f, n, id(*f.m_parent, "_construct_" + to_string(n))) + "; ";
			call += "this.type = " + name(*f.m_parent) + "; ";
			call += "getCache(" + name(*f.m_parent) + ")[this.ptr] = this;";
			return call;
		};

		auto js_wrap_optional_call = [&](const CLCallable& f, size_t n, const string& call, size_t max_args)
		{
			if(n < max_args)
				jsw("if (a" + to_string(n) + " === undefined) { " + call + (f.m_return_type.isvoid() ? " return;" : "") + " }");
			else
				jsw(call);
		};

		auto js_call_n = [&](const CLCallable& f, size_t n, size_t max_args)
		{
			string call = f.m_kind == CLPrimitiveKind::Constructor ? js_call_constructor(f, n) : js_call_func(f, n);
			return js_wrap_optional_call(f, n, call, max_args);

		};

		auto js_call = [&](const CLCallable& f, const Overloads& o)
		{
			for(auto& sig : o.sigs)
				js_call_n(*sig.second, sig.first, f.m_params.size());
		};

		auto js_call_prepare = [&](const CLCallable& f)
		{
			bool prepare = false;
			for(const CLParam& p : f.m_params)
				if(p.m_type.iscstring() || p.m_type.isstring() || p.m_type.isarray())
					prepare = true;
			if(prepare)
				jsw("ensureCache.prepare();");

		};

		auto js_check_msg = [](const CLClass& c, const CLCallable& f, const CLParam& p)
		{
			return "[CHECK FAILED] " + c.m_name + "::" + f.m_name + "(arg" + to_string(p.m_index) + ":" + p.m_name + "): ";
		};

		auto js_call_check_arg = [&](const CLQualType& t, const string& a, bool optional)
		{
			string msg = "";// js_check_msg(c, f, p);
			if(optional) jsw("if (typeof " + a + " !== \"undefined\" && " + a + " !== null) {");

			if(t.isinteger())
				jsw("assert(typeof " + a + " === \"number\" && !isNaN(" + a + "), \"" + msg + "Expecting <integer>\");");
			else if(t.isfloat())
				jsw("assert(typeof " + a + " === \"number\", \"" + msg + "Expecting <number>\");");
			else if(t.isboolean())
				jsw("assert(typeof " + a + " === \"boolean\" || (typeof " + a + " === \"number\" && !isNaN(" + a + ")), \"" + msg + "Expecting <boolean>\");");
			else if(t.iscstring() || t.isstring())
				jsw("assert(typeof " + a + " === \"string\" || (" + a + " && typeof " + a + " === \"object\" && typeof " + a + ".ptr === \"number\"), \"" + msg + "Expecting <string>\");");
			else if(t.isclass())
				jsw("assert(typeof " + a + " === \"object\" && typeof " + a + ".ptr === \"number\", \"" + msg + "Expecting <pointer>\");");

			if(optional) jsw("}");
		};

		// We need to avoid some closure errors on the constructors we define here.
		//string js_supress = "/** @suppress {undefinedVars, duplicate} */ ";
		string js_supress = "";

		auto js_bind_callable = [&](const Overloads& o)
		{
			const CLCallable& f = *(*--o.sigs.end()).second;
			bool ctor = f.m_kind == CLPrimitiveKind::Constructor;
			size_t max_args = f.m_params.size();
			if(f.m_kind == CLPrimitiveKind::Method)
				jsw(name(*f.m_parent) + ".prototype[\"" + f.m_name + "\"] = " + name(*f.m_parent) + ".prototype." + f.m_name + " = ", true);
			else if(f.m_kind == CLPrimitiveKind::Function)
				jsw(js_module_path(m, f) + " = ", true);
			jsw(js_supress + "function" + (ctor ? " " + name(*f.m_parent) : "") + "(" + comma(js_signature_args(f, max_args)) + ") {");
			if(f.m_kind == CLPrimitiveKind::Method)
				jsw("var self = this.ptr;");
			js_call_prepare(f);
			js_call(f, o);
			jsw("};");
		};

		auto js_getter = [&](const CLClass& c, const CLMember& m, bool isarray = false)
		{
			jsw("function(" + string(isarray ? "index" : "") + ") {");
			jsw("var self = this.ptr;");
			jsw(js_call_return_wrap(m.m_type, "_" + id(c, "_get_" + m.m_name) + "(self" + (isarray ? ", index" : "") + ")"));
			jsw("}", true);
		};

		auto js_setter = [&](const CLClass& c, const CLMember& m, bool isarray = false)
		{
			jsw("function(" + string(isarray ? "index, " : "") + "value) {");
			jsw("var self = this.ptr;");
#if MULTI_FUNC
			jsw("_" + id(c, "_set_" + m.m_name) + "(self" + (isarray ? ", index" : "") + ", " + js_call_convert_arg(m.m_type, "value", isarray) + ");");
#else
			js_call_convert_arg(m.m_type, "value", false);
			jsw("_" + id(c, "_set_" + m.m_name) + "(self" + (isarray ? ", index" : "") + ", value);");
#endif
			jsw("}");
		};

		auto js_destructor = [&](const CLClass& c)
		{
			jsw("function() {");
			jsw("var self = this.ptr;");
			jsw("_" + id(c, "_destroy") + "(self);");
			jsw("};");
		};

		auto js_constructor = [&](const CLClass& c)
		{
			string implementing = c.m_bases.size() > 0 ? name(real_type(*c.m_bases[0])) : "WrapperObject"; // = implements[name][0] if implements.get(name) else 'WrapperObject'
			jsw(name(c) + ".prototype = Object.create(" + implementing + ".prototype);");
			jsw(name(c) + ".prototype.constructor = " + name(c) + ";");
			jsw(name(c) + ".prototype.__class__ = " + name(c) + ";");
			jsw(name(c) + ".__cache__ = {};");
			jsw(js_module_path(m, c) + " = " + name(c) + ";");
		};

		auto js_function = [&](const Overloads& o)
		{
			js_bind_callable(o);
		};

		auto c_call_n = [&](const CLCallable& f, size_t i)
		{
			if(f.m_kind == CLPrimitiveKind::Constructor)
				return "new " + f.m_parent->m_id + "(" + comma(c_forward_args(f, i)) + ")";
			else if(f.m_kind == CLPrimitiveKind::Method)
			{
				CLMethod& m = (CLMethod&)f;
				if(m.m_function) // hack for function methods
					return m.m_function->m_id + "(" + comma(merge({ "*self" }, c_forward_args(f, i))) + ")";
				else
					return "self->" + f.m_name + "(" + comma(c_forward_args(f, i)) + ")";
			}
			else if(f.m_kind == CLPrimitiveKind::Function || true)
				return f.m_id + "(" + comma(c_forward_args(f, i)) + ")";
		};

		auto c_call_operator = [&](const CLCallable& f, const string& op)
		{
			string cast_self = "self";
			string maybe_deref = value(f.m_params[0].m_type);
			if(op.find("=") != string::npos)
				cw("(*" + cast_self + ") " + op + " " + maybe_deref + f.m_params[0].m_name);
			else if(op == "[]")
				cw("((*" + cast_self + ")[" + maybe_deref + f.m_params[0].m_name + "])");
			else
				printf("ERROR: unfamiliar operator %s\n", op.c_str());
		};

		auto c_value_wrap = [&](const CLQualType& type, const string& call, bool ref)
		{
			if(type.isvoid())
				cw(call + ";");
			else if(type.isstring())
				cw("return " + call + ".c_str();");
			else if(type.isbasetype() || type.isenum())
				cw("return " + call + ";");
			else if(ref || !type.value() || !type.copyable())
				cw("return " + address(type) + call + ";");
			else
			{
				cw("static " + type.m_type_name + " temp;");
				cw("return (temp = " + call + ", &temp);");
			}
		};

		auto c_call_return_wrap = [&](const CLQualType& return_type, const string& call, bool ctor = false)
		{
			if(ctor)
				cw("return " + call + ";");
			else
				return c_value_wrap(return_type, call, false);
		};

		auto c_call_wrapped_n = [&](const CLCallable& f, size_t i)
		{
			c_call_return_wrap(f.m_return_type, c_call_n(f, i), f.m_kind == CLPrimitiveKind::Constructor);
		};

		auto c_bind_callable_n = [&](const CLCallable& f, size_t i)
		{
			if(f.m_kind != CLPrimitiveKind::Constructor)
				cw((f.m_return_type.isconst() ? "const " : "") + type_to_c(f.m_return_type) + " DECL " + id(f, i) + "(" + comma(c_typed_args(f, i)) + ") {");
			else
				cw(f.m_parent->m_id + "* DECL " + id(*f.m_parent, "_construct_" + to_string(i)) + "(" + comma(c_typed_args(f, i)) + ") {");

			c_call_wrapped_n(f, i);
			cw("}");

			/*if(!ctor)
				if(i == max_args)
				{
					vector<string> C_FLOATS = { "float", "double" };

					string dec_args = comma(transform<string>(0, i, [&](size_t j) { return type_to_cdec(raw[j]->m_type) + ' ' + args[j]; })); ;
					string js_call_args = comma(transform<string>(0, i, [&](size_t j) { return (is_interface(sig[j]) ? "(int)" : "") + take_addr_if_nonpointer(raw[j]->m_type) + args[j]; }));

					js_impl_methods += "  " + c_return_type + " " + func_name + "(" + dec_args + ") " + maybe_const + " {";
					js_impl_methods += basic_return + "EM_ASM_" + (!has(C_FLOATS, c_return_type) ? "INT" : "DOUBLE") + "({";
					js_impl_methods += "var self = Module['getCache'](Module['" + class_name + "'])[$0];";
					js_impl_methods += "if (!self.hasOwnProperty('" + func_name + "')) throw 'a JSImplementation must implement all functions, you forgot " + class_name + "::" + func_name + ".';";
					js_impl_methods += "return " + return_prefix + "self['" + func_name + "'](" + comma(transform<string>(1, max_args + 1, [](size_t i) { return "$" + to_string(i); })) + ")" + return_postfix + ";";
					js_impl_methods += "}, (int)this" + string(js_call_args.size() > 0 ? ", " : "") + js_call_args + ");";
				}*/
		};

		auto c_bind_callable = [&](const Overloads& o)
		{
			for(auto& sig : o.sigs)
				c_bind_callable_n(*sig.second, sig.first);
		};

		auto c_type_handle = [&](const CLClass& c)
		{
			cw("mud::Type* DECL " + id(c, "_type") + "() {");
			cw("return &mud::type<" + c.m_id + ">();");
			cw("}");
		};

		auto c_getter = [&](const CLClass& c, const CLMember& m)
		{
			cw(type_to_c(m.m_type) + " DECL " + id(c, "_get_" + m.m_name) + "(" + c.m_id + "* self" + (m.m_type.isarray() ? ", unsigned int index" : "") + ") {");
			c_value_wrap(m.m_type, "self->" + m.m_member + (m.m_method ? "()" : "") + (m.m_type.isarray() ? "[index]" : ""), !m.m_method);
			cw("}");
		};

		auto c_setter = [&](const CLClass& c, const CLMember& m)
		{
			cw("void DECL " + id(c, "_set_" + m.m_name) + "(" + c.m_id + "* self, " + (m.m_type.isarray() ? "unsigned int index, " : "") + type_to_c(m.m_type) + " value"  + ") {");
			if(m.m_setter)
				cw("self->" + m.m_member + "(" + value(m.m_type) + "value);");
			else
				cw("self->" + m.m_member + (m.m_type.isarray() ? "[index]" : "") + " = " + value(m.m_type) + "value;");
			cw("}");
		};

		auto c_destructor = [&](const CLClass& c)
		{
			cw("void DECL " + id(c, "_destroy") + "(" + c.m_id + "* self) {");
			cw("delete self;");
			cw("}");
		};


/*for name, interface in interfaces.items():
  js_impl = interface.getExtendedAttribute('JSImplementation')
  if not js_impl: continue
  implements[name] = [js_impl[0]]*/

		for(auto& pair : m.m_namespaces)
		{
			CLNamespace& n = *pair.second;
			if(n.m_reflect_content && n.m_name != m.m_namespace)
				js_namespace(*pair.second);
		}

		for(auto& pc : m.m_classes)
			if(pc->m_reflect)
			{
				if(blacklist_class(*pc)) continue;

				CLClass& c = *pc;

				jsw("// " + c.m_name);
				cw("// " + c.m_name);

				c_type_handle(c);

				//js_impl_methods = "";

				//js_impl = interface.getExtendedAttribute('JSImplementation')
				//if js_impl:
				//  js_impl = js_impl[0]

				OverloadMap constructors;

				for(CLConstructor& ctor : c.m_constructors)
				{
					overload(constructors, ctor);
				}

				for(Overloads& o : constructors)
				{
					c_bind_callable(o);
					js_bind_callable(o);
				}

				if(c.m_constructors.empty())
				{
					// Ensure a constructor even if one is not specified.
					jsw(js_supress + "function " + name(c) + "() { throw \"cannot construct a " + c.m_name + ", no constructor in IDL\" }");
				}

				js_constructor(c);

				OverloadMap methods;

				for(CLMethod& f : c.m_methods)
				{
					if(blacklist_callable(f)) continue;
					overload(methods, f);
				}

				for(Overloads& o : methods)
				{
					c_bind_callable(o);
					js_bind_callable(o);
				}

				for(CLMember& a : c.m_members)
				{
					if(blacklist_member(a)) continue;
					//if(a.m_type.m_type->m_is_templated) continue;

					c_getter(c, a);
					if(!a.m_nonmutable)
						c_setter(c, a);

					jsw("Object.defineProperty(" + name(c) + ".prototype, \"" + a.m_name + "\", {");
					jsw("get: ", true);
					js_getter(c, a);
					if(!a.m_nonmutable)
					{
						jsw(",");
						jsw("set: ", true);
						js_setter(c, a);
					}
					jsw("});");
				}


				if(true) //not interface.getExtendedAttribute("NoDelete"):
				{
					c_destructor(c);
					jsw(name(c) + ".prototype[\"__destroy__\"] = " + name(c) + ".prototype.__destroy__ = ", true);
					js_destructor(c);
				}
				
				// Emit C++ class implementation that calls into JS implementation

				/*if(js_impl != "")
				{
					pre_c += "\n";
					pre_c += "class " + c.m_name + " : public " + type_to_c(js_impl, true) + " {";
					pre_c += "public:";
					pre_c += join(js_impl_methods, "\n");
					pre_c += "};";
				}*/
			}

			OverloadMap functions;

			for(auto& pf : m.m_functions)
				if(pf->m_reflect)
				{
					if(blacklist_callable(*pf)) continue;
					overload(functions, *pf);
				}

			for(Overloads& o : functions)
			{
				c_bind_callable(o);
				js_bind_callable(o);
			}

			for(auto& pc : m.m_aliases)
				if(pc->m_reflect)
				{
					CLAlias& a = *pc;
					// only bind aliases to classes (not base types)
					if(a.m_target->m_type_kind == CLTypeKind::Class)
						jsw(js_module_path(m, a) + " = " + name(*a.m_target) + ";");
				}

			jsw("");
			jsw("(function() {");
			jsw("function setup() {");
			
			for(auto& pc : m.m_classes)
				if(pc->m_reflect && !pc->m_is_templated)
				{
					if(blacklist_class(*pc)) continue;

					CLClass& c = *pc;

					jsw(replace(c.m_name, "::", "_") + ".__type__ = " + "_" + replace(c.m_id, "::", "_") + "__type();"); // add wrapPointer() ?
				}

			//string enum_prefix = "emscripten_enum_";
			string enum_prefix = "";

			for(auto& pe : m.m_enums)
				if(pe->m_reflect)
				{
					CLEnum& e = *pe;
					cw("// " + e.m_name);
					jsw("// " + e.m_name);
					if(e.m_scoped)
						js_namespace(e);
					for(size_t i = 0; i < e.m_ids.size(); ++i)
					{
						string f = enum_prefix + replace(e.m_id, "::", "_") + "_" + e.m_ids[i];
						cw(e.m_id + " DECL " + f + "() {");
						cw("return " + e.m_scoped_ids[i] + ";");
						cw("}");

						jsw(js_module_path(m, e.m_scoped ? e : *e.m_parent) + "['" + e.m_ids[i] + "'] = _" + f + "();");
					}
				}

			jsw("}");
			jsw("if (Module['calledRun']) setup();");
			jsw("else addOnPreMain(setup);");
			jsw("})();");

			cw("\n}\n\n");

			write_file((m.m_bind_path + "/" + m.m_dotname + ".c.cpp").c_str(), ct.c_str());
			write_file((m.m_bind_path + "/" + m.m_dotname + ".js").c_str(), jst.c_str());
	}
}
}
