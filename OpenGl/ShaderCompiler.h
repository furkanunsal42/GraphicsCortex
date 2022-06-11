#pragma once
#include <string>
#include <map>
#include "Debuger.h"

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
	Shader();
Shader(std::string target_file);
};

Shader read_shader(const std::string target_file);
unsigned int compile_shader(unsigned int type, const std::string shader_source);
unsigned int compile_program(const std::string vertex_shader_source, const std::string fragment_shader_source);

struct Program {
public:
	unsigned int id;
	std::map<std::string, unsigned int> uniforms;
	Program();
	Program(std::string vertex_shader_code, std::string fragment_shader_code);
	void define_uniform(std::string name);
	void update_uniformf(std::string name, float a, float b, float c, float d);
	void compile(std::string vertex_shader_code, std::string fragment_shader_code);
	void bind();
	void unbind();
};