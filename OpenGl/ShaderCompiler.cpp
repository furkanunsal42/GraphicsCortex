
#include <string>
#include <fstream>
#include <iostream>
#include <GL\glew.h>
#include "Renderer.h"

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
};

Shader read_shader(const std::string target_file) {
	Shader shader;
	std::string type = "";
	std::ifstream file(target_file);
	std::string line;

	while (std::getline(file, line)) {
		if (line.find("#<vertex shader>") != std::string::npos) {
			type = "vertex";
			continue;
		}
		else if (line.find("#<fragment shader>") != std::string::npos) {
			type = "fragment";
			continue;
		}
		if (type == "vertex") {
			shader.vertex_shader += line + '\n';
		}
		else if (type == "fragment") {
			shader.fragment_shader += line + '\n';
		}
	}
	return shader;
}


unsigned int compile_shader(unsigned int type, const std::string shader_source) {
	unsigned int shader = glCreateShader(type);
	const char* shader_code_pointer = &shader_source[0];
	GLCall(glShaderSource(shader, 1, &shader_code_pointer, nullptr));

	std::string type_string = std::to_string(type);
	if (type == GL_VERTEX_SHADER)
		type_string = "vertex shader";
	else if (type == GL_FRAGMENT_SHADER)
		type_string = "fragment shader";

	std::cout << "compiling the shader of type " << type_string << '\n';

	GLCall(glCompileShader(shader));

	int compile_status;
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status));
	if (compile_status != GL_TRUE) {
		char* compile_error_msg = (char*)malloc(1024 * sizeof(char));
		GLCall(glGetShaderInfoLog(shader, 1024, nullptr, compile_error_msg));
		std::cout << "shader compile error with massage: " << compile_error_msg << '\n';
	}

	return shader;
}

unsigned int compile_program(const std::string vertex_shader_source, const std::string fragment_shader_source) {
	unsigned int program = glCreateProgram();
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

	GLCall(glAttachShader(program, vertex_shader));
	GLCall(glAttachShader(program, fragment_shader));
	GLCall(glLinkProgram(program));

	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));

	return program;
}
