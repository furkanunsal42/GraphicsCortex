#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
	compile(vertex_shader_code, fragment_shader_code);
}
Program::Program(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code) {
	if (geometry_shader_code == "")
		compile(vertex_shader_code, fragment_shader_code);
	else
		compile(vertex_shader_code, geometry_shader_code, fragment_shader_code);
}

Program::Program(const Shader& shader) : Program(shader.vertex_shader, shader.geometry_shader, shader.fragment_shader) {}


void Program::_detect_and_define_all_uniforms_legacy(const std::string& shader_code) {

	// detect uniform definitions
	std::stringstream ss(shader_code);
	std::string line;
	while (std::getline(ss, line)) {
		int location = line.find("uniform");
		if (location != std::string::npos) {

			// find the name of the uniform
			// uniform type NAME;
			// uniform type NAME = 0;
			// _ws0   _ws1 _ws2 _ws3		ws: white space: ' ', '=', ';'

			// NAME = string (ws2 + 1 : ws3)

			int space_counter = 0;
			int begin, end;
			for (int iterate = 0; space_counter < 3; iterate++) {
				char current_character = line[location + iterate];
				if (current_character == ' ' || current_character == '=' || current_character == ';' || current_character == '\t') {
					space_counter++;
					if (space_counter == 2)
						begin = location + iterate;
					else if (space_counter == 3) {
						end = location + iterate;
					}
				}
			}
			std::string uniform_name = line.substr(begin + 1, end - begin - 1);
			// uniforms can also be arrays
			// uniform type name[number];
			int array_bracet_begin = uniform_name.find('[');
			int array_bracet_end = uniform_name.find(']');
			if(array_bracet_begin == std::string::npos)
				define_uniform(uniform_name);
			else {
				int array_size = std::stoi(uniform_name.substr(array_bracet_begin + 1, array_bracet_end - array_bracet_begin - 1));
				for (int i = 0; i < array_size; i++) {
					std::string new_uniform_name = uniform_name.substr(0, array_bracet_begin + 1) + std::to_string(i) + uniform_name.substr(array_bracet_end);
					define_uniform(new_uniform_name);
				}
			}
		}
	}
}

void Program::_detect_and_define_all_uniforms(unsigned int id) {
	int uniform_amount;
	GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_amount));

	for (int i = 0; i < uniform_amount; i++)
	{
		int name_length;
		int size;
		GLenum type;
		char name[64];
		GLCall(glGetActiveUniform(id, (GLuint)i, 32, &name_length, &size, &type, name));

		std::cout << name << std::endl;
		define_uniform(std::string(name));

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

	#ifdef DEFINE_SHADER_UNIFORMS_UPON_COMPILE
	_detect_and_define_all_uniforms(id);
	#endif
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

	#ifdef DEFINE_SHADER_UNIFORMS_UPON_COMPILE
	_detect_and_define_all_uniforms(id);
	#endif
}

void Program::bind() {
	GLCall(glUseProgram(id));
}
void Program::unbind() {
	GLCall(glUseProgram(0));
}

// UNIFORMS
void Program::define_uniform(const std::string& name) {
	auto a = uniforms.find(name);
	auto b = uniforms.end();
	if (a == b){
		GLCall(unsigned int uniform_id = glGetUniformLocation(this->id, name.c_str()));
		if (uniform_id == -1) {
			std::cout << "[OpenGL Error] Program::define_uniform(const std::string& name) was called but program doesn't have a uniform with given name : \"" << name << "\"" <<  std::endl;
		}
		GLCall(uniforms.insert(std::pair<std::string, unsigned int>(name, uniform_id)));
	}
}

unsigned int Program::define_get_uniform_id(const std::string& name) {
	auto a = uniforms.find(name);
	auto b = uniforms.end();
	if (a == b){
		GLCall(unsigned int uniform_id = glGetUniformLocation(this->id, name.c_str()));
		if (uniform_id == -1) {
			std::cout << "[OpenGL Error] Program::define_get_uniform_id(const std::string& name) was called but program doesn't have a uniform with given name : \"" << name << "\"" << std::endl;
		}
		GLCall(uniforms.insert(std::pair<std::string, unsigned int>(name, uniform_id)));
		return uniform_id;
	}
	return uniforms[name];
}

void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform4i(uniforms[name], a, b, c, d));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform4f(uniforms[name], a, b, c, d));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform3i(uniforms[name], a, b, c));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform3f(uniforms[name], a, b, c));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform2i(uniforms[name], a, b));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform2f(uniforms[name], a, b));
}
void Program::update_uniform(const std::string& name, const int& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform1i(uniforms[name], a));
}
void Program::update_uniform(const std::string& name, const float& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform1f(uniforms[name], a));
}
void Program::update_uniform(const std::string& name, const glm::mat4& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat3& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix3fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat2& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniformMatrix2fv(uniforms[name], 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec4& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform4fv(uniforms[name], 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec3& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform3fv(uniforms[name], 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec2& a) {
	define_uniform(name);
	if (name.find('.') != std::string::npos)
		define_uniform(name);
	bind();
	GLCall(glUniform2fv(uniforms[name], 1, glm::value_ptr(a)));
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

