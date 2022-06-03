#pragma once

#include <string>

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
};

Shader read_shader(const std::string target_file);

unsigned int compile_shader(unsigned int type, const std::string shader_source);

unsigned int compile_program(const std::string vertex_shader_source, const std::string fragment_shader_source);
