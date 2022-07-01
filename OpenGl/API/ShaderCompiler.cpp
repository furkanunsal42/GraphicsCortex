#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <GL\glew.h>
#include "Debuger.h"

Shader::Shader() { ; }

Shader::Shader(const std::string& target_file)
{
	read_shader(target_file);
}
Shader::Shader(const std::string& vertex_target_file, const std::string& fragment_target_file) {
	read_shader(vertex_target_file);
	read_shader(fragment_target_file);
}
Shader::Shader(const std::string& vertex_target_file, const std::string& geometry_terget_file, const std::string& fragment_target_file) {
	read_shader(vertex_target_file);
	read_shader(geometry_terget_file);
	read_shader(fragment_target_file);
}

void Shader::read_shader(const std::string& target_file) {
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
		else if (line.find("#<geometry shader>") != std::string::npos) {
			type = "geometry";
			continue;
		}
		if (type == "vertex") {
			vertex_shader += line + '\n';
		}
		else if (type == "fragment") {
			fragment_shader += line + '\n';
		}
		else if (type == "geometry") {
			geometry_shader += line + '\n';
		}
	}
}

unsigned int Program::compile_shader(unsigned int type, const std::string& shader_source) {
	unsigned int shader_id = glCreateShader(type);
	const char* shader_code_pointer = &shader_source[0];
	GLCall(glShaderSource(shader_id, 1, &shader_code_pointer, nullptr));

	std::string type_string = std::to_string(type);
	if (type == GL_VERTEX_SHADER)
		type_string = "vertex shader";
	else if (type == GL_FRAGMENT_SHADER)
		type_string = "fragment shader";

	std::cout << "compiling the shader of type " << type_string << '\n';

	GLCall(glCompileShader(shader_id));

	int compile_status;
	GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status));
	if (compile_status != GL_TRUE) {
		char* compile_error_msg = (char*)malloc(1024 * sizeof(char));
		GLCall(glGetShaderInfoLog(shader_id, 1024, nullptr, compile_error_msg));
		std::cout << "shader compile error with massage: " << compile_error_msg << '\n';
	}
	return shader_id;
}

Program::Program() {
	id = 0;
}
Program::Program(const std::string& vertex_shader_code, const std::string& fragment_shader_code) {
	compile(vertex_shader_code, fragment_shader_code);
}
Program::Program(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code) {
	compile(vertex_shader_code, geometry_shader_code, fragment_shader_code);
}


void Program::compile(const std::string& vertex_shader_code, const std::string& fragment_shader_code) {
	id = glCreateProgram();
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_code);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_code);

	GLCall(glAttachShader(id, vertex_shader));
	GLCall(glAttachShader(id, fragment_shader));
	GLCall(glLinkProgram(id));

	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));
}

void Program::compile(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code){
	id = glCreateProgram();
	unsigned int vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_code);
	unsigned int geometry_shader = compile_shader(GL_GEOMETRY_SHADER, geometry_shader_code);
	unsigned int fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_code);

	GLCall(glAttachShader(id, vertex_shader));
	GLCall(glAttachShader(id, geometry_shader));
	GLCall(glAttachShader(id, fragment_shader));
	GLCall(glLinkProgram(id));

	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(geometry_shader));
	GLCall(glDeleteShader(fragment_shader));
}

void Program::bind() {
	GLCall(glUseProgram(id));
}
void Program::unbind() {
	GLCall(glUseProgram(0));
}

// UNIFORMS
void Program::define_uniform(std::string name) {
	GLCall(uniforms.insert(std::pair<std::string, unsigned int>(name, glGetUniformLocation(this->id, name.c_str()))));
}

void Program::update_uniform(std::string name, glm::mat4 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(std::string name, glm::mat3 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix3fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(std::string name, glm::mat2 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix2fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}

void Program::update_uniform(std::string name, glm::vec4 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniform4fv(uniforms[name], 1, glm::value_ptr(a)));
}
void Program::update_uniform(std::string name, glm::vec3 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniform3fv(uniforms[name], 1, glm::value_ptr(a)));
}
void Program::update_uniform(std::string name, glm::vec2 a) {
	if (uniforms.find(name) == uniforms.end())
		define_uniform(name);
	bind();
	GLCall(glUniform2fv(uniforms[name], 1, glm::value_ptr(a)));
}
