#pragma once
#include "SharedPtr.h"
#include "ShaderCompiler.h"

class Program_s {
public:

	_SHARED_POINTER_DEFINITIONS(Program)

		Program_s() :
		obj(std::make_shared<Program>()) {}

	Program_s(const std::string& vertex_shader_code, const std::string& fragment_shader_code) :
		obj(std::make_shared<Program>(vertex_shader_code, fragment_shader_code)) {}

	Program_s(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code) :
		obj(std::make_shared<Program>(vertex_shader_code, geometry_shader_code, fragment_shader_code)) {}

	explicit Program_s(const Shader& shader) :
		obj(std::make_shared<Program>(shader)) {}
};