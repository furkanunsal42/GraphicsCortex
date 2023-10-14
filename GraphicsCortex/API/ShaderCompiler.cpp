#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL\glew.h>
#include "Debuger.h"

#include "DirectoryUtils.h"

Shader::Shader() { ; }

Shader::Shader(const std::string& target_file)
{
	read_shader(target_file);
	filename = compute_filename(target_file);
}
Shader::Shader(const std::string& vertex_target_file, const std::string& fragment_target_file) {
	read_shader(vertex_target_file);
	read_shader(fragment_target_file);
	filename = compute_filename(vertex_target_file)+ " | " + compute_filename(fragment_target_file);
}
Shader::Shader(const std::string& vertex_target_file, const std::string& geometry_terget_file, const std::string& fragment_target_file) {
	read_shader(vertex_target_file);
	read_shader(geometry_terget_file);
	read_shader(fragment_target_file);
	filename = compute_filename(vertex_target_file) + " | " + compute_filename(geometry_terget_file) + " | " + compute_filename(fragment_target_file);
}

void Shader::read_shader(const std::string& target_file) {

	filename = target_file;

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
	else if (type == GL_GEOMETRY_SHADER)
		type_string = "geometry shader";

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
	std::cout << "[ProgramInfo] nameless program is compiling." << std::endl;

	compile(vertex_shader_code, fragment_shader_code);
}
Program::Program(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code) {
	std::cout << "[ProgramInfo] nameless program is compiling." << std::endl;

	if (geometry_shader_code == "")
		compile(vertex_shader_code, fragment_shader_code);
	else
		compile(vertex_shader_code, geometry_shader_code, fragment_shader_code);
}

Program::Program(const Shader& shader)
{
	std::cout << "[ProgramInfo] " + shader.filename + " is compiling." << std::endl;

	if (shader.geometry_shader == "")
		compile(shader.vertex_shader, shader.fragment_shader);
	else
		compile(shader.vertex_shader, shader.geometry_shader, shader.fragment_shader);
}

void Program::_define_all_uniforms(unsigned int id) {
	int uniform_amount;
	GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_amount));

	for (int i = 0; i < uniform_amount; i++)
	{
		int name_length;
		int size;
		GLenum type;
		char name[64];

		glGetActiveUniform(id, (GLuint)i, sizeof(name), &name_length, &size, &type, name);

		std::string uniform_name(name, name_length);

		uniforms[uniform_name] = UniformData(uniform_name, UniformData::Float, i);
	}
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

	_define_all_uniforms(id);
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

	_define_all_uniforms(id);
}

void Program::bind() {
	GLCall(glUseProgram(id));
}
void Program::unbind() {
	GLCall(glUseProgram(0));
}

void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d) {
	bind();
	GLCall(glUniform4i(uniforms[name].index, a, b, c, d));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d) {
	bind();
	GLCall(glUniform4f(uniforms[name].index, a, b, c, d));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c) {
	bind();
	GLCall(glUniform3i(uniforms[name].index, a, b, c));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c) {
	bind();
	GLCall(glUniform3f(uniforms[name].index, a, b, c));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b) {
	bind();
	GLCall(glUniform2i(uniforms[name].index, a, b));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b) {
	bind();
	GLCall(glUniform2f(uniforms[name].index, a, b));
}
void Program::update_uniform(const std::string& name, const int& a) {
	bind();
	GLCall(glUniform1i(uniforms[name].index, a));
}
void Program::update_uniform(const std::string& name, const float& a) {
	bind();
	GLCall(glUniform1f(uniforms[name].index, a));
}
void Program::update_uniform(const std::string& name, const glm::mat4& a) {
	bind();
	GLCall(glUniformMatrix4fv(uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat3& a) {
	bind();
	GLCall(glUniformMatrix3fv(uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat2& a) {
	bind();
	GLCall(glUniformMatrix2fv(uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec4& a) {
	bind();
	GLCall(glUniform4fv(uniforms[name].index, 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec3& a) {
	bind();
	GLCall(glUniform3fv(uniforms[name].index, 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec2& a) {
	bind();
	GLCall(glUniform2fv(uniforms[name].index, 1, glm::value_ptr(a)));
}

// ------------------------------------

void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c, const int& d) {
	bind();
	GLCall(glUniform4i(uniform_id, a, b, c, d));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c, const float& d) {
	bind();
	GLCall(glUniform4f(uniform_id, a, b, c, d));
}
void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c) {
	bind();
	GLCall(glUniform3i(uniform_id, a, b, c));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c) {
	bind();
	GLCall(glUniform3f(uniform_id, a, b, c));
}
void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b) {
	bind();
	GLCall(glUniform2i(uniform_id, a, b));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b) {
	bind();
	GLCall(glUniform2f(uniform_id, a, b));
}
void Program::update_uniform(unsigned int uniform_id, const int& a) {
	bind();
	GLCall(glUniform1i(uniform_id, a));
}
void Program::update_uniform(unsigned int uniform_id, const float& a) {
	bind();
	GLCall(glUniform1f(uniform_id, a));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat4& a) {
	bind();
	GLCall(glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat3& a) {
	bind();
	GLCall(glUniformMatrix3fv(uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat2& a) {
	bind();
	GLCall(glUniformMatrix2fv(uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec4& a) {
	bind();
	GLCall(glUniform4fv(uniform_id, 1, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec3& a) {
	bind();
	GLCall(glUniform3fv(uniform_id, 1, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec2& a) {
	bind();
	GLCall(glUniform2fv(uniform_id, 1, glm::value_ptr(a)));
}

