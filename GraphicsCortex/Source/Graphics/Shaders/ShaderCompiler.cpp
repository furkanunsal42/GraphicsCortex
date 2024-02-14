#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL\glew.h>
#include "Debuger.h"

#include "DirectoryUtils.h"
#include <unordered_set>

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

void Program::_define_all_uniforms() {
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

		UniformData::Type uniform_type;
		switch (type) {
			case GL_FLOAT:
				uniform_type = UniformData::FLOAT;
				break;
			case GL_FLOAT_VEC2:
				uniform_type = UniformData::FLOAT_VEC2;
				break;
			case GL_FLOAT_VEC3:
				uniform_type = UniformData::FLOAT_VEC3;
				break;
			case GL_FLOAT_VEC4:
				uniform_type = UniformData::FLOAT_VEC4;
				break;
			case GL_DOUBLE:
				uniform_type = UniformData::DOUBLE;
				break;
			case GL_DOUBLE_VEC2:
				uniform_type = UniformData::DOUBLE_VEC2;
				break;
			case GL_DOUBLE_VEC3:
				uniform_type = UniformData::DOUBLE_VEC3;
				break;
			case GL_DOUBLE_VEC4:
				uniform_type = UniformData::DOUBLE_VEC4;
				break;
			case GL_INT:
				uniform_type = UniformData::INT;
				break;
			case GL_INT_VEC2:
				uniform_type = UniformData::INT_VEC2;
				break;
			case GL_INT_VEC3:
				uniform_type = UniformData::INT_VEC3;
				break;
			case GL_INT_VEC4:
				uniform_type = UniformData::INT_VEC4;
				break;
			case GL_UNSIGNED_INT:
				uniform_type = UniformData::UNSIGNED_INT;
				break;
			case GL_UNSIGNED_INT_VEC2:
				uniform_type = UniformData::UNSIGNED_INT_VEC2;
				break;
			case GL_UNSIGNED_INT_VEC3:
				uniform_type = UniformData::UNSIGNED_INT_VEC3;
				break;
			case GL_UNSIGNED_INT_VEC4:
				uniform_type = UniformData::UNSIGNED_INT_VEC4;
				break;
			case GL_BOOL:
				uniform_type = UniformData::BOOL;
				break;
			case GL_BOOL_VEC2:
				uniform_type = UniformData::BOOL_VEC2;
				break;
			case GL_BOOL_VEC3:
				uniform_type = UniformData::BOOL_VEC3;
				break;
			case GL_BOOL_VEC4:
				uniform_type = UniformData::BOOL_VEC4;
				break;
			case GL_FLOAT_MAT2:
				uniform_type = UniformData::FLOAT_MAT2;
				break;
			case GL_FLOAT_MAT3:
				uniform_type = UniformData::FLOAT_MAT3;
				break;
			case GL_FLOAT_MAT4:
				uniform_type = UniformData::FLOAT_MAT4;
				break;
			case GL_FLOAT_MAT2x3:
				uniform_type = UniformData::FLOAT_MAT2x3;
				break;
			case GL_FLOAT_MAT2x4:
				uniform_type = UniformData::FLOAT_MAT2x4;
				break;
			case GL_FLOAT_MAT3x2:
				uniform_type = UniformData::FLOAT_MAT3x2;
				break;
			case GL_FLOAT_MAT3x4:
				uniform_type = UniformData::FLOAT_MAT3x4;
				break;
			case GL_FLOAT_MAT4x2:
				uniform_type = UniformData::FLOAT_MAT4x2;
				break;
			case GL_FLOAT_MAT4x3:
				uniform_type = UniformData::FLOAT_MAT4x3;
				break;
			case GL_DOUBLE_MAT2:
				uniform_type = UniformData::DOUBLE_MAT2;
				break;
			case GL_DOUBLE_MAT3:
				uniform_type = UniformData::DOUBLE_MAT3;
				break;
			case GL_DOUBLE_MAT4:
				uniform_type = UniformData::DOUBLE_MAT4;
				break;
			case GL_DOUBLE_MAT2x3:
				uniform_type = UniformData::DOUBLE_MAT2x3;
				break;
			case GL_DOUBLE_MAT2x4:
				uniform_type = UniformData::DOUBLE_MAT2x4;
				break;
			case GL_DOUBLE_MAT3x2:
				uniform_type = UniformData::DOUBLE_MAT3x2;
				break;
			case GL_DOUBLE_MAT3x4:
				uniform_type = UniformData::DOUBLE_MAT3x4;
				break;
			case GL_DOUBLE_MAT4x2:
				uniform_type = UniformData::DOUBLE_MAT4x2;
				break;
			case GL_DOUBLE_MAT4x3:
				uniform_type = UniformData::DOUBLE_MAT4x3;
				break;
			case GL_SAMPLER_1D:
				uniform_type = UniformData::SAMPLER_1D;
				break;
			case GL_SAMPLER_2D:
				uniform_type = UniformData::SAMPLER_2D;
				break;
			case GL_SAMPLER_3D:
				uniform_type = UniformData::SAMPLER_3D;
				break;
			case GL_SAMPLER_CUBE:
				uniform_type = UniformData::SAMPLER_CUBE;
				break;
			case GL_SAMPLER_1D_SHADOW:
				uniform_type = UniformData::SAMPLER_1D_SHADOW;
				break;
			case GL_SAMPLER_2D_SHADOW:
				uniform_type = UniformData::SAMPLER_2D_SHADOW;
				break;
			case GL_SAMPLER_1D_ARRAY:
				uniform_type = UniformData::SAMPLER_1D_ARRAY;
				break;
			case GL_SAMPLER_2D_ARRAY:
				uniform_type = UniformData::SAMPLER_2D_ARRAY;
				break;
			case GL_SAMPLER_1D_ARRAY_SHADOW:
				uniform_type = UniformData::SAMPLER_1D_ARRAY_SHADOW;
				break;
			case GL_SAMPLER_2D_ARRAY_SHADOW:
				uniform_type = UniformData::SAMPLER_2D_ARRAY_SHADOW;
				break;
			case GL_SAMPLER_2D_MULTISAMPLE:
				uniform_type = UniformData::SAMPLER_2D_MULTISAMPLE;
				break;
			case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::SAMPLER_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_SAMPLER_CUBE_SHADOW:
				uniform_type = UniformData::SAMPLER_CUBE_SHADOW;
				break;
			case GL_SAMPLER_BUFFER:
				uniform_type = UniformData::SAMPLER_BUFFER;
				break;
			case GL_SAMPLER_2D_RECT:
				uniform_type = UniformData::SAMPLER_2D_RECT;
				break;
			case GL_SAMPLER_2D_RECT_SHADOW:
				uniform_type = UniformData::SAMPLER_2D_RECT_SHADOW;
				break;
			case GL_INT_SAMPLER_1D:
				uniform_type = UniformData::INT_SAMPLER_1D;
				break;
			case GL_INT_SAMPLER_2D:
				uniform_type = UniformData::INT_SAMPLER_2D;
				break;
			case GL_INT_SAMPLER_3D:
				uniform_type = UniformData::INT_SAMPLER_3D;
				break;
			case GL_INT_SAMPLER_CUBE:
				uniform_type = UniformData::INT_SAMPLER_CUBE;
				break;
			case GL_INT_SAMPLER_1D_ARRAY:
				uniform_type = UniformData::INT_SAMPLER_1D_ARRAY;
				break;
			case GL_INT_SAMPLER_2D_ARRAY:
				uniform_type = UniformData::INT_SAMPLER_2D_ARRAY;
				break;
			case GL_INT_SAMPLER_2D_MULTISAMPLE:
				uniform_type = UniformData::INT_SAMPLER_2D_MULTISAMPLE;
				break;
			case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_INT_SAMPLER_BUFFER:
				uniform_type = UniformData::INT_SAMPLER_BUFFER;
				break;
			case GL_INT_SAMPLER_2D_RECT:
				uniform_type = UniformData::INT_SAMPLER_2D_RECT;
				break;
			case GL_UNSIGNED_INT_SAMPLER_1D:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_1D;
				break;
			case GL_UNSIGNED_INT_SAMPLER_2D:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_2D;
				break;
			case GL_UNSIGNED_INT_SAMPLER_3D:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_3D;
				break;
			case GL_UNSIGNED_INT_SAMPLER_CUBE:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_CUBE;
				break;
			case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_1D_ARRAY;
				break;
			case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_2D_ARRAY;
				break;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
				break;
			case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_UNSIGNED_INT_SAMPLER_BUFFER:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_BUFFER;
				break;
			case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
				uniform_type = UniformData::UNSIGNED_INT_SAMPLER_2D_RECT;
				break;
			case GL_IMAGE_1D:
				uniform_type = UniformData::IMAGE_1D;
				break;
			case GL_IMAGE_2D:
				uniform_type = UniformData::IMAGE_2D;
				break;
			case GL_IMAGE_3D:
				uniform_type = UniformData::IMAGE_3D;
				break;
			case GL_IMAGE_2D_RECT:
				uniform_type = UniformData::IMAGE_2D_RECT;
				break;
			case GL_IMAGE_CUBE:
				uniform_type = UniformData::IMAGE_CUBE;
				break;
			case GL_IMAGE_BUFFER:
				uniform_type = UniformData::IMAGE_BUFFER;
				break;
			case GL_IMAGE_1D_ARRAY:
				uniform_type = UniformData::IMAGE_1D_ARRAY;
				break;
			case GL_IMAGE_2D_ARRAY:
				uniform_type = UniformData::IMAGE_2D_ARRAY;
				break;
			case GL_IMAGE_2D_MULTISAMPLE:
				uniform_type = UniformData::IMAGE_2D_MULTISAMPLE;
				break;
			case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::IMAGE_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_INT_IMAGE_1D:
				uniform_type = UniformData::INT_IMAGE_1D;
				break;
			case GL_INT_IMAGE_2D:
				uniform_type = UniformData::INT_IMAGE_2D;
				break;
			case GL_INT_IMAGE_3D:
				uniform_type = UniformData::INT_IMAGE_3D;
				break;
			case GL_INT_IMAGE_2D_RECT:
				uniform_type = UniformData::INT_IMAGE_2D_RECT;
				break;
			case GL_INT_IMAGE_CUBE:
				uniform_type = UniformData::INT_IMAGE_CUBE;
				break;
			case GL_INT_IMAGE_BUFFER:
				uniform_type = UniformData::INT_IMAGE_BUFFER;
				break;
			case GL_INT_IMAGE_1D_ARRAY:
				uniform_type = UniformData::INT_IMAGE_1D_ARRAY;
				break;
			case GL_INT_IMAGE_2D_ARRAY:
				uniform_type = UniformData::INT_IMAGE_2D_ARRAY;
				break;
			case GL_INT_IMAGE_2D_MULTISAMPLE:
				uniform_type = UniformData::INT_IMAGE_2D_MULTISAMPLE;
				break;
			case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::INT_IMAGE_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_UNSIGNED_INT_IMAGE_1D:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_1D;
				break;
			case GL_UNSIGNED_INT_IMAGE_2D:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_2D;
				break;
			case GL_UNSIGNED_INT_IMAGE_3D:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_3D;
				break;
			case GL_UNSIGNED_INT_IMAGE_2D_RECT:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_2D_RECT;
				break;
			case GL_UNSIGNED_INT_IMAGE_CUBE:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_CUBE;
				break;
			case GL_UNSIGNED_INT_IMAGE_BUFFER:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_BUFFER;
				break;
			case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_1D_ARRAY;
				break;
			case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_2D_ARRAY;
				break;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
				break;
			case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
				uniform_type = UniformData::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
				break;
			case GL_UNSIGNED_INT_ATOMIC_COUNTER:
				uniform_type = UniformData::UNSIGNED_INT_ATOMIC_COUNTER;
				break;
			default:
				uniform_type = UniformData::UNKNOWN;
		}

		uniforms[uniform_name] = UniformData(uniform_name, uniform_type, i);
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

	_define_all_uniforms();
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

	_define_all_uniforms();
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
	GLCall(glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(a)));
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

void Program::attach_uniform_buffer(const std::string& name, std::shared_ptr<UniformBuffer> uniform_buffer){
	_uniform_buffers[name] = uniform_buffer;
}

void Program::deattach_uniform_buffer(const std::string& name){
	_uniform_buffers.erase(name);
}

void Program::update_uniform_buffer_slots(){
	std::unordered_set<int> used_buffer_slots;
	int slot_iterator = 0;
	for (auto iterator = _uniform_buffers.begin(); iterator != _uniform_buffers.end(); iterator++) {
		const std::string& name = iterator->first;
		const std::shared_ptr<UniformBuffer> uniform_buffer = iterator->second;
		uniform_buffer->upload_data();

		if (!uniform_buffer->ever_bound)
			uniform_buffer->bind(slot_iterator);

		GLCall(unsigned int location = glGetUniformBlockIndex(id, name.c_str()));
		
		if (used_buffer_slots.find(uniform_buffer->bound_slot) == used_buffer_slots.end()) {	// uniform buffer's currently bound position is free
			used_buffer_slots.insert(uniform_buffer->bound_slot);
			GLCall(glUniformBlockBinding(id, location, uniform_buffer->bound_slot));
		}
		else {	// if not, search for a free spot and bind both buffer and uniform to that spot
			for (slot_iterator; used_buffer_slots.find(uniform_buffer->bound_slot) == used_buffer_slots.end(); slot_iterator++){}
			uniform_buffer->bind(slot_iterator);
			slot_iterator++;
			GLCall(glUniformBlockBinding(id, location, uniform_buffer->bound_slot));
		}
	}
}

