#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include "Debuger.h"

#include "DirectoryUtils.h"
#include <unordered_set>

#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

Shader::Shader() { ; }

Shader::Shader(const std::filesystem::path& target_file)
{
	read_shader(target_file.string());
	filename = compute_filename(target_file.string());
}
Shader::Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& fragment_target_file) {
	read_shader(vertex_target_file.string());
	read_shader(fragment_target_file.string());
	filename = compute_filename(vertex_target_file.string()) + " | " + compute_filename(fragment_target_file.string());
}
Shader::Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& geometry_terget_file, const std::filesystem::path& fragment_target_file) {
	read_shader(vertex_target_file.string());
	read_shader(geometry_terget_file.string());
	read_shader(fragment_target_file.string());
	filename = compute_filename(vertex_target_file.string()) + " | " + compute_filename(geometry_terget_file.string()) + " | " + compute_filename(fragment_target_file.string());
}

void Shader::read_shader(const std::string& target_file) {

	filename = target_file;

	std::string type = "";
	std::ifstream file(target_file);
	std::string line;

	if (!file){
		std::cout << "[Filepath Error] Shader tried to read_shader() but \"" << target_file << "\" couldn't found" << std::endl;
		ASSERT(false);
	}

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
		else if (line.find("#<compute shader>") != std::string::npos) {
			type = "compute";
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
		else if (type == "compute") {
			compute_shader += line + '\n';
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
	int uniform_amount = 0;
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

		GLCall(uint32_t uniform_index = glGetUniformLocation(id, uniform_name.c_str()));
		uniforms[uniform_name] = UniformData(uniform_name, uniform_type, uniform_index);
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

	int32_t link_result;
	GLCall(glGetProgramiv(id, GL_LINK_STATUS, &link_result));

	if (link_result != GL_TRUE) {
		char buff[1024];
		int32_t length;
		glGetProgramInfoLog(id, 1024, &length, buff);

		std::cout << "[OpenGL Error] program failed to link with error message : \n\"" << buff << "\"" << std::endl;
		ASSERT(false);
	}

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

void Program::update_uniform(const std::string& name, UniformBuffer& uniform_buffer, size_t offset, size_t size)
{
	uniform_buffer.upload_data();

	GLCall(unsigned int index = glGetUniformBlockIndex(id, name.c_str()));
	if (index == GL_INVALID_INDEX) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_buffer_slots() but uniform block named: \"" + name + "\" wasn't found in the shader" << std::endl;
	}

	int32_t binding = -1;
	GLCall(glGetActiveUniformBlockiv(id, index, GL_UNIFORM_BLOCK_BINDING, &binding));

	if (binding == -1) {	// maybe
		ASSERT(false);
	}

	uniform_buffer.bind(binding, offset, size);
}

void Program::update_uniform(const std::string& name, UniformBuffer& uniform_buffer, size_t offset)
{
	uniform_buffer.upload_data(); 

	GLCall(unsigned int index = glGetUniformBlockIndex(id, name.c_str()));
	if (index == GL_INVALID_INDEX) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_buffer_slots() but uniform block named: \"" + name + "\" wasn't found in the shader" << std::endl;
	}

	int32_t binding = -1;
	GLCall(glGetActiveUniformBlockiv(id, index, GL_UNIFORM_BLOCK_BINDING, &binding));

	if (binding == -1) {	// maybe
		ASSERT(false);
	}
	
	uniform_buffer.bind(binding, offset, uniform_buffer.get_buffer_size_in_bytes());
}

void Program::update_uniform(const std::string& name, Texture1D& texture1d)
{
	texture1d.wait_async_load();
	if (!texture1d._texture_allocated) texture1d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture1d.bind(slot);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform(const std::string& name, Texture2D& texture2d)
{
	texture2d.wait_async_load();
	if (!texture2d._texture_allocated) texture2d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture2d.bind(slot);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform(const std::string& name, Texture3D& texture3d)
{
	texture3d.wait_async_load();
	if (!texture3d._texture_allocated) texture3d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture3d.bind(slot);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform(const std::string& name, Texture2DArray& texturearray)
{
	texturearray.wait_async_load();
	if (!texturearray._texture_allocated) texturearray._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texturearray.bind(slot);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform(const std::string& name, TextureCubeMap& texturecubemap)
{
	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_allocated) texturecubemap._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texturecubemap.bind(slot);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform_bindless(const std::string& name, Texture1D& texture1d)
{
	if (!texture1d.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture1D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture1d.wait_async_load();
	if (!texture1d._texture_handle_created) texture1d._allocate_texture();
	GLCall(unsigned int location = glGetUniformLocation(id, name.c_str()));
	GLCall(glProgramUniformHandleui64ARB(id, location, texture1d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture2D& texture2d)
{
	if (!texture2d.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture2D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2d.wait_async_load();
	if (!texture2d._texture_handle_created) texture2d._allocate_texture();
	GLCall(unsigned int location = glGetUniformLocation(id, name.c_str()));
	GLCall(glProgramUniformHandleui64ARB(id, location, texture2d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture3D& texture3d)
{
	if (!texture3d.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture3D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture3d.wait_async_load();
	if (!texture3d._texture_handle_created) texture3d._allocate_texture();
	GLCall(unsigned int location = glGetUniformLocation(id, name.c_str()));
	GLCall(glProgramUniformHandleui64ARB(id, location, texture3d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture2DArray& texture2darray)
{
	if (!texture2darray.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture2DArray.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2darray.wait_async_load();
	if (!texture2darray._texture_handle_created) texture2darray._allocate_texture();
	GLCall(unsigned int location = glGetUniformLocation(id, name.c_str()));
	GLCall(glProgramUniformHandleui64ARB(id, location, texture2darray.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, TextureCubeMap& texturecubemap)
{
	//if (!texturecubemap.is_bindless) {
	//	std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture2DArray.is_bindless was false" << std::endl;
	//	ASSERT(false);
	//}

	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_handle_created) texturecubemap._allocate_texture();
	GLCall(unsigned int location = glGetUniformLocation(id, name.c_str()));
	GLCall(glProgramUniformHandleui64ARB(id, location, texturecubemap.texture_handle));
}

void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d) {
	GLCall(glProgramUniform4i(id, uniforms[name].index, a, b, c, d));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d) {
	GLCall(glProgramUniform4f(id, uniforms[name].index, a, b, c, d));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c) {
	GLCall(glProgramUniform3i(id, uniforms[name].index, a, b, c));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c) {
	GLCall(glProgramUniform3f(id, uniforms[name].index, a, b, c));
}
void Program::update_uniform(const std::string& name, const int& a, const int& b) {
	GLCall(glProgramUniform2i(id, uniforms[name].index, a, b));
}
void Program::update_uniform(const std::string& name, const float& a, const float& b) {
	GLCall(glProgramUniform2f(id, uniforms[name].index, a, b));
}
void Program::update_uniform(const std::string& name, const int& a) {
	GLCall(glProgramUniform1i(id, uniforms[name].index, a));
}
void Program::update_uniform(const std::string& name, const float& a) {
	GLCall(glProgramUniform1f(id, uniforms[name].index, a));
}
void Program::update_uniform(const std::string& name, const glm::mat4& a) {
	GLCall(glProgramUniformMatrix4fv(id, uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat3& a) {
	GLCall(glProgramUniformMatrix3fv(id, uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::mat2& a) {
	GLCall(glProgramUniformMatrix2fv(id, uniforms[name].index, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec4& a) {
	GLCall(glProgramUniform4fv(id, uniforms[name].index, 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec3& a) {
	GLCall(glProgramUniform3fv(id, uniforms[name].index, 1, glm::value_ptr(a)));
}
void Program::update_uniform(const std::string& name, const glm::vec2& a) {
	GLCall(glProgramUniform2fv(id, uniforms[name].index, 1, glm::value_ptr(a)));
}

// ------------------------------------

void Program::update_uniform(unsigned int uniform_id, Texture2D& texture2d)
{
	if (!texture2d._texture_handle_created) texture2d._allocate_texture();
	GLCall(glProgramUniformHandleui64ARB(id, uniform_id, texture2d.texture_handle));
}

void Program::update_uniform(unsigned int uniform_id, TextureCubeMap& texturecubemap)
{
	if (!texturecubemap._texture_handle_created) texturecubemap._allocate_texture();
	GLCall(glProgramUniformHandleui64ARB(id, uniform_id, texturecubemap.texture_handle));
}

void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c, const int& d) {
	GLCall(glProgramUniform4i(id, uniform_id, a, b, c, d));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c, const float& d) {
	GLCall(glProgramUniform4f(id, uniform_id, a, b, c, d));
}
void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c) {
	GLCall(glProgramUniform3i(id, uniform_id, a, b, c));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c) {
	GLCall(glProgramUniform3f(id, uniform_id, a, b, c));
}
void Program::update_uniform(unsigned int uniform_id, const int& a, const int& b) {
	GLCall(glProgramUniform2i(id, uniform_id, a, b));
}
void Program::update_uniform(unsigned int uniform_id, const float& a, const float& b) {
	GLCall(glProgramUniform2f(id, uniform_id, a, b));
}
void Program::update_uniform(unsigned int uniform_id, const int& a) {
	GLCall(glProgramUniform1i(id, uniform_id, a));
}
void Program::update_uniform(unsigned int uniform_id, const float& a) {
	GLCall(glProgramUniform1f(id, uniform_id, a));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat4& a) {
	GLCall(glProgramUniformMatrix4fv(id, uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat3& a) {
	GLCall(glProgramUniformMatrix3fv(id, uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::mat2& a) {
	GLCall(glProgramUniformMatrix2fv(id, uniform_id, 1, GL_FALSE, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec4& a) {
	GLCall(glProgramUniform4fv(id, uniform_id, 1, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec3& a) {
	GLCall(glProgramUniform3fv(id, uniform_id, 1, glm::value_ptr(a)));
}
void Program::update_uniform(unsigned int uniform_id, glm::vec2& a) {
	GLCall(glProgramUniform2fv(id, uniform_id, 1, glm::value_ptr(a)));
}

bool Program::_does_uniform_exist(const std::string& name)
{
	auto index = _uniform_location_table.find(name);
	if (index != _uniform_location_table.end())
		return true;
	else {
		GLCall(int location = glGetUniformLocation(id, name.c_str()));
		if (location == -1) {
			return false;
		}
		_uniform_location_table[name] = location;
		return true;
	}
}

int Program::_get_uniform_location(const std::string& name)
{
	auto index = _uniform_location_table.find(name);
	if (index != _uniform_location_table.end())
		return index->second;
	else {
		GLCall(int location = glGetUniformLocation(id, name.c_str()));
		if (location == -1) {
			std::cout << "[OpenGL Error] Program tried to _get_uniform_location() but no uniform with name: \"" << name << "\" was found" << std::endl;
			ASSERT(false);
		}
		_uniform_location_table[name] = location;
		return location;
	}
}

