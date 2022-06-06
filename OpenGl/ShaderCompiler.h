#pragma once

#include <string>
#include <map>

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
};

Shader read_shader(const std::string target_file);

unsigned int compile_shader(unsigned int type, const std::string shader_source);

unsigned int compile_program(const std::string vertex_shader_source, const std::string fragment_shader_source);

struct Program {
public:
	unsigned int id;
	std::map<std::string, unsigned int> uniforms;

	Program() {
		id = 0;
	}
	Program(std::string vertex_shader_code, std::string fragment_shader_code) {
		compile(vertex_shader_code, fragment_shader_code);
	}
	void define_uniform(std::string name) {
		GLCall(uniforms.insert(std::pair<std::string, unsigned int>(name, glGetUniformLocation(this->id, name.c_str()))));
	}
	void update_uniform4f(std::string name, float a, float b, float c, float d) {
		bind();
		GLCall(glUniform4f(uniforms[name], a, b, c, d));
	}

	void compile(std::string vertex_shader_code, std::string fragment_shader_code) {
		id = compile_program(vertex_shader_code, fragment_shader_code);
	}
	void bind() {
		GLCall(glUseProgram(id));
	}
	void unbind() {
		GLCall(glUseProgram(0));
	}
};