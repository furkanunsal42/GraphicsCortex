#include "ShaderCompiler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

#include <GL/glew.h>
#include "Debuger.h"

#include <unordered_set>

#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

#include "Buffer.h"
#include "UniformBuffer.h"

Shader::Shader() { ; }

Shader::Shader(const std::filesystem::path& target_file)
{
	read_shader(target_file.string());
	filename = target_file.filename().string();
}
Shader::Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& fragment_target_file) {
	read_shader(vertex_target_file.string());
	read_shader(fragment_target_file.string());
	filename = vertex_target_file.filename().string() + " | " + fragment_target_file.filename().string();
}
Shader::Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& geometry_target_file, const std::filesystem::path& fragment_target_file) {
	read_shader(vertex_target_file.string());
	read_shader(geometry_target_file.string());
	read_shader(fragment_target_file.string());
	filename = vertex_target_file.filename().string() + " | " + geometry_target_file.filename().string() + " | " + fragment_target_file.filename().string();
}

void Shader::read_shader(const std::filesystem::path& path) {

	if (Package::is_package_loaded()) {
		if (!Package::loaded_package->does_exist(path)) {
			std::cout << "[Filepath Error] Shader tried to read_shader() with package but " << path << " couldn't found" << std::endl;
			std::cout << path << std::endl;
			ASSERT(false);
		}

		std::string source_code = Package::loaded_package->get(path);

		read_shader_source(source_code);
		return;
	}
	else {
		std::ifstream file(path);
		if (!file) {
			std::cout << "[Filepath Error] Shader tried to read_shader() but " << path << " couldn't found" << std::endl;
			ASSERT(false);
		}
		std::string file_content(std::filesystem::file_size(path), '\0');
		file.read(&file_content[0], file_content.size());

		read_shader_source(file_content);
		return;
	}
}

void Shader::read_shader_source(const std::string& source_code)
{
	std::string type = "";
	std::stringstream file(source_code);
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

Program::Program(const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values)
{
	set_preprocessor(preprocessing_key_values);

	_generate_program();
}

Program::Program(const Shader& shader, const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values)
{
	set_preprocessor(preprocessing_key_values);

	_generate_program();
	compile_shader(shader);
}

Program::~Program()
{
	release();
}

void Program::release()
{
	if (_program_generated) {
		GLCall(glDeleteProgram(id));
		_program_generated = false;
		_program_compiled = false;
	}
}

void Program::bind()
{
	if (!_program_generated) {
		std::cout << "[OpenGL Error] Program tried to bind() but it was released" << std::endl;
		ASSERT(false);
	}

	if (!_program_compiled) {
		std::cout << "[OpenGL Error] Program tried to bind() but it wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glUseProgram(id));
}

void Program::unbind()
{
	GLCall(glUseProgram(0));
}

void Program::clear_preprocessor()
{
	_preprocessing_defines.clear();
}

void Program::set_preprocessor(const std::string& key, const std::string& value)
{
	_preprocessing_defines[key] = value;
}

void Program::set_preprocessor(const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values)
{
	for (const std::pair<std::string, std::string>& key_value : preprocessing_key_values)
		set_preprocessor(key_value.first, key_value.second);
}

std::string Program::get_preprocessor(const std::string& key)
{
	return _preprocessing_defines[key];
}


namespace {
	bool is_char_whitespace(char c) {
		return
			c == ' ' ||
			c == '\t' ||
			c == '\r' ||
			c == '\n';
	}
}

namespace {
	unsigned int compile_gl_shader(unsigned int type, const std::string& shader_source) {
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

		GLCall(glCompileShader(shader_id));

		int compile_status;
		GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status));
		if (compile_status != GL_TRUE) {
			char* compile_error_msg = (char*)malloc(1024 * 16 * sizeof(char));
			GLCall(glGetShaderInfoLog(shader_id, 1024 * 16, nullptr, compile_error_msg));
			std::cout << "shader compile error with massage: " << compile_error_msg << '\n';
		}
		return shader_id;
	}

	std::string apply_preprocessing_definitions(
		const std::string& original_source,
		const std::unordered_map<std::string, std::string>& preprocessing_defines
	) {
		std::string result;
		std::string line;
		std::stringstream source_shader_stream(original_source);
		while (std::getline(source_shader_stream, line)) {
			if (int define_begin = line.find("#define") != std::string::npos) {

				int name_position_begin = define_begin + 7;
				while (name_position_begin < line.size() && is_char_whitespace(line.at(name_position_begin))) { name_position_begin++; }

				int name_position_end = name_position_begin;
				while (name_position_end < line.size() && !is_char_whitespace(line.at(name_position_end))) { name_position_end++; }

				int value_position_begin = name_position_end;
				while (value_position_begin < line.size() && is_char_whitespace(line.at(value_position_begin))) { value_position_begin++; }

				bool has_key = false;
				int value_position_end = value_position_begin;
				while (value_position_end < line.size()) { if (!is_char_whitespace(line.at(value_position_end))) has_key = true; value_position_end++; }

				std::string define_name = line.substr(name_position_begin, name_position_end - name_position_begin);

				if (preprocessing_defines.find(define_name) != preprocessing_defines.end()) {
					if (!has_key) line = line.substr(0, name_position_end) + " " + preprocessing_defines.at(define_name);
					else line.replace(value_position_begin, value_position_end, preprocessing_defines.at(define_name));
				}
			}

			result += line + "\n";
		}
		return result;
	}
}

void Program::compile_shader(const Shader& shader)
{
	Shader shader_to_use = shader;
	if (_preprocessing_defines.size() != 0) {
		shader_to_use.vertex_shader		= apply_preprocessing_definitions(shader_to_use.vertex_shader, _preprocessing_defines);
		shader_to_use.fragment_shader	= apply_preprocessing_definitions(shader_to_use.fragment_shader, _preprocessing_defines);
		shader_to_use.geometry_shader	= apply_preprocessing_definitions(shader_to_use.geometry_shader, _preprocessing_defines);
	}

	if (!_program_generated) {
		std::cout << "[OpenGL Error] Program::compile_shader() is called on a deleted program" << std::endl;
		ASSERT(false);
	}

	if (shader_to_use.geometry_shader == "") {
		unsigned int vertex_shader		= compile_gl_shader(GL_VERTEX_SHADER,	shader_to_use.vertex_shader);
		unsigned int fragment_shader	= compile_gl_shader(GL_FRAGMENT_SHADER, shader_to_use.fragment_shader);

		GLCall(glAttachShader(id, vertex_shader));
		GLCall(glAttachShader(id, fragment_shader));
		GLCall(glLinkProgram(id));

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(fragment_shader));
	}
	else {

		unsigned int vertex_shader		= compile_gl_shader(GL_VERTEX_SHADER,	shader.vertex_shader);
		unsigned int geometry_shader	= compile_gl_shader(GL_GEOMETRY_SHADER, shader.geometry_shader);
		unsigned int fragment_shader	= compile_gl_shader(GL_FRAGMENT_SHADER, shader.fragment_shader);

		GLCall(glAttachShader(id, vertex_shader));
		GLCall(glAttachShader(id, geometry_shader));
		GLCall(glAttachShader(id, fragment_shader));
		GLCall(glLinkProgram(id));

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(geometry_shader));
		GLCall(glDeleteShader(fragment_shader));
	}

	int32_t link_result;
	GLCall(glGetProgramiv(id, GL_LINK_STATUS, &link_result));

	if (link_result != GL_TRUE) {
		char buff[1024];
		int32_t length;
		glGetProgramInfoLog(id, 1024, &length, buff);

		std::cout << "[OpenGL Error] program failed to link with error message : \n\"" << buff << "\"" << std::endl;
		ASSERT(false);
	}

	_program_compiled = true;
}

void Program::update_uniform_as_storage_buffer(const std::string& name, Buffer& buffer, size_t offset, size_t size) {

	// I don't think they are necessary
	//buffer.wait_to_sycronize_upload();
	//buffer.wait_to_sycronize_download();

	if (!buffer._buffer_allocated) buffer._allocate_buffer(buffer._buffer_size);

	GLCall(int index = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
	if (index == -1) {	// maybe
		ASSERT(false);
	}

	uint32_t properties[] = { GL_BUFFER_BINDING };
	int32_t binding;
	GLCall(glGetProgramResourceiv(id, GL_SHADER_STORAGE_BLOCK, index, 1, properties, 1, NULL, &binding));

	buffer.bind_as_storage_buffer(binding, offset, size);
}

void Program::update_uniform_as_storage_buffer(const std::string& name, Buffer& buffer, size_t offset)
{
	// I don't think they are necessary
	//buffer.wait_to_sycronize_upload();
	//buffer.wait_to_sycronize_download();

	if (!buffer._buffer_allocated) buffer._allocate_buffer(buffer._buffer_size);

	GLCall(int index = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
	if (index == -1) {	// maybe
		ASSERT(false);
	}

	uint32_t properties[] = { GL_BUFFER_BINDING };
	int32_t binding;
	GLCall(glGetProgramResourceiv(id, GL_SHADER_STORAGE_BLOCK, index, 1, properties, 1, NULL, &binding));

	buffer.bind_as_storage_buffer(binding, offset, buffer.get_buffer_size_in_bytes());
}

void Program::update_uniform_as_uniform_buffer(const std::string& name, UniformBuffer& uniform_buffer, size_t offset, size_t size)
{
	uniform_buffer.upload_data();

	GLCall(unsigned int index = glGetUniformBlockIndex(id, name.c_str()));
	if (index == GL_INVALID_INDEX) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_as_uniform_buffer() but uniform block named: \"" + name + "\" wasn't found in the shader" << std::endl;
	}

	int32_t binding = -1;
	GLCall(glGetActiveUniformBlockiv(id, index, GL_UNIFORM_BLOCK_BINDING, &binding));

	if (binding == -1) {	// maybe
		ASSERT(false);
	}

	uniform_buffer.bind(binding, offset, size);
}

void Program::update_uniform_as_uniform_buffer(const std::string& name, UniformBuffer& uniform_buffer, size_t offset)
{

	uniform_buffer.upload_data();

	GLCall(unsigned int index = glGetUniformBlockIndex(id, name.c_str()));
	if (index == GL_INVALID_INDEX) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_as_uniform_buffer() but uniform block named: \"" + name + "\" wasn't found in the shader" << std::endl;
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

void Program::update_uniform(const std::string& name, Texture2DArray& texture2darray)
{
	texture2darray.wait_async_load();
	if (!texture2darray._texture_allocated) texture2darray._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture2darray.bind(slot);
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
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture1d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture2D& texture2d)
{
	if (!texture2d.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture2D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2d.wait_async_load();
	if (!texture2d._texture_handle_created) texture2d._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture2d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture3D& texture3d)
{
	if (!texture3d.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture3D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture3d.wait_async_load();
	if (!texture3d._texture_handle_created) texture3d._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture3d.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, Texture2DArray& texture2darray)
{
	if (!texture2darray.is_bindless) {
		std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but Texture2DArray.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2darray.wait_async_load();
	if (!texture2darray._texture_handle_created) texture2darray._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture2darray.texture_handle));
}

void Program::update_uniform_bindless(const std::string& name, TextureCubeMap& texturecubemap)
{
	//if (!texturecubemap.is_bindless) {
	//	std::cout << "[OpenGL Error] Program tried to update_uniform_bindless() but TextureCubeMap.is_bindless was false" << std::endl;
	//	ASSERT(false);
	//}

	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_handle_created) texturecubemap._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texturecubemap.texture_handle));
}

void Program::update_uniform_as_image(const std::string& name, Texture1D& texture1d, int mipmap_level)
{
	texture1d.wait_async_load();
	if (!texture1d._texture_allocated) texture1d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture1d.bind_as_image(slot, mipmap_level);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform_as_image(const std::string& name, Texture2D& texture2d, int mipmap_level)
{
	texture2d.wait_async_load();
	if (!texture2d._texture_allocated) texture2d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture2d.bind_as_image(slot, mipmap_level);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform_as_image(const std::string& name, Texture3D& texture3d, int mipmap_level)
{
	texture3d.wait_async_load();
	if (!texture3d._texture_allocated) texture3d._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));
	if (slot == -1) {
		ASSERT(false);
	}

	texture3d.bind_as_image(slot, mipmap_level);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform_as_image(const std::string& name, Texture2DArray& texture2darray, int mipmap_level)
{
	texture2darray.wait_async_load();
	if (!texture2darray._texture_allocated) texture2darray._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));

	if (slot == -1) {
		ASSERT(false);
	}

	texture2darray.bind_as_image(slot, mipmap_level);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform_as_image(const std::string& name, TextureCubeMap& texturecubemap, int mipmap_level) {
	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_allocated) texturecubemap._allocate_texture();
	if (!_does_uniform_exist(name)) return;

	int slot = -1;
	GLCall(glGetUniformiv(id, _get_uniform_location(name), &slot));

	if (slot == -1) {
		ASSERT(false);
	}

	texturecubemap.bind_as_image(slot, mipmap_level);
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), slot));
}

void Program::update_uniform(const std::string& name, const int& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), a));
}

void Program::update_uniform(const std::string& name, const int& a, const int& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2i(id, _get_uniform_location(name), a, b));
}

void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3i(id, _get_uniform_location(name), a, b, c));
}

void Program::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4i(id, _get_uniform_location(name), a, b, c, d));
}

void Program::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4ui(id, _get_uniform_location(name), a, b, c, d));
}

void Program::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3ui(id, _get_uniform_location(name), a, b, c));
}

void Program::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2ui(id, _get_uniform_location(name), a, b));
}

void Program::update_uniform(const std::string& name, const unsigned int& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1ui(id, _get_uniform_location(name), a));
}

void Program::update_uniform(const std::string& name, const float& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1f(id, _get_uniform_location(name), a));
}

void Program::update_uniform(const std::string& name, const float& a, const float& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2f(id, _get_uniform_location(name), a, b));
}

void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3f(id, _get_uniform_location(name), a, b, c));
}

void Program::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4f(id, _get_uniform_location(name), a, b, c, d));
}

void Program::update_uniform(const std::string& name, const glm::vec2& a)
{
	update_uniform(name, a.x, a.y);
}

void Program::update_uniform(const std::string& name, const glm::vec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void Program::update_uniform(const std::string& name, const glm::vec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void Program::update_uniform(const std::string& name, const glm::ivec2& a)
{
	update_uniform(name, a.x, a.y);
}

void Program::update_uniform(const std::string& name, const glm::ivec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void Program::update_uniform(const std::string& name, const glm::ivec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void Program::update_uniform(const std::string& name, const glm::uvec2& a)
{
	update_uniform(name, a.x, a.y);
}

void Program::update_uniform(const std::string& name, const glm::uvec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void Program::update_uniform(const std::string& name, const glm::uvec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void Program::update_uniform(const std::string& name, const glm::mat2& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix2fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void Program::update_uniform(const std::string& name, const glm::mat3& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix3fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void Program::update_uniform(const std::string& name, const glm::mat4& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix4fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void Program::update_uniform(const std::string& name, const glm::mat3x4& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix3x4fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void Program::_generate_program()
{
	if (_program_generated) return;

	GLCall(id = glCreateProgram());
	_program_generated = true;
}

bool Program::_does_uniform_exist(const std::string& name) {
	if (!_program_generated) {
		std::cout << "[OpenGL Error] Program tried to _does_uniform_exist() but Program was released" << std::endl;
		ASSERT(false);
	}
	if (!_program_compiled) {
		std::cout << "[OpenGL Error] Program tried to _does_uniform_exist() but Program wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

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
	if (!_program_generated) {
		std::cout << "[OpenGL Error] Program tried to _get_unifrom_location() but Program was released" << std::endl;
		ASSERT(false);
	}
	if (!_program_compiled) {
		std::cout << "[OpenGL Error] Program tried to _get_uniform_location() but Program wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

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