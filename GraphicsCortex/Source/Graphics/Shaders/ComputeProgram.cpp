#include "ComputeProgram.h"
#include "Debuger.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"
#include "StandardBuffer.h"

#include <sstream>

ComputeProgram::ComputeProgram()
{
	_generate_program();
}

ComputeProgram::ComputeProgram(const Shader& shader, std::vector<std::pair<std::string, std::string>> preprocessing_key_values)
{
	for (std::pair<std::string, std::string>& key_value : preprocessing_key_values)
		set_preprocessor(key_value.first, key_value.second);
	
	_generate_program();
	compile_shader(shader);
}

ComputeProgram::~ComputeProgram()
{
	release();
}

void ComputeProgram::release()
{
	if (_program_generated) {
		GLCall(glDeleteProgram(id));
		_program_generated = false;
		_program_compiled = false;
	}
}

void ComputeProgram::dispatch(int workgroup_size_x, int workgroup_size_y, int workgroup_size_z)
{
	dispatch_without_barrier(workgroup_size_x, workgroup_size_y, workgroup_size_z);
	memory_barrier(MemoryBarrierType::ALL_BARRIER_BITS);
}

void ComputeProgram::dispatch_thread(int thread_count_x, int thread_count_y, int thread_count_z)
{
	dispatch(std::ceil(thread_count_x / (float)_work_group_size.x), std::ceil(thread_count_y / (float)_work_group_size.y), std::ceil(thread_count_z / (float)_work_group_size.z));
}

void ComputeProgram::dispatch_without_barrier(int workgroup_size_x, int workgroup_size_y, int workgroup_size_z)
{
	if (!_program_generated) {
		std::cout << "[OpenGL Error] ComputeProgram tried to dispatch() but it was released" << std::endl;
		ASSERT(false);
	}

	if (!_program_compiled) {
		std::cout << "[OpenGL Error] ComputeProgram tried to dispatch() but it wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

	bind();
	GLCall(glDispatchCompute(workgroup_size_x, workgroup_size_y, workgroup_size_z));
}

void ComputeProgram::dispatch_thread_without_barrier(int thread_count_x, int thread_count_y, int thread_count_z)
{
	dispatch_without_barrier(std::ceil(thread_count_x / (float)_work_group_size.x), std::ceil(thread_count_y / (float)_work_group_size.y), std::ceil(thread_count_z / (float)_work_group_size.z));
}

void ComputeProgram::memory_barrier(MemoryBarrierType barrier)
{
	unsigned int gl_barrier_bits = 0;

	gl_barrier_bits |= barrier & VERTEX_ATTRIB_ARRAY_BARRIER_BIT	?	GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT	: 0;
	gl_barrier_bits |= barrier & ELEMENT_ARRAY_BARRIER_BIT			?	GL_ELEMENT_ARRAY_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & UNIFORM_BARRIER_BIT				?	GL_UNIFORM_BARRIER_BIT				: 0;
	gl_barrier_bits |= barrier & TEXTURE_FETCH_BARRIER_BIT			?	GL_TEXTURE_FETCH_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & SHADER_IMAGE_ACCESS_BARRIER_BIT	?	GL_SHADER_IMAGE_ACCESS_BARRIER_BIT	: 0;
	gl_barrier_bits |= barrier & COMMAND_BARRIER_BIT				?	GL_COMMAND_BARRIER_BIT				: 0;
	gl_barrier_bits |= barrier & PIXEL_BUFFER_BARRIER_BIT			?	GL_PIXEL_BUFFER_BARRIER_BIT			: 0;
	gl_barrier_bits |= barrier & TEXTURE_UPDATE_BARRIER_BIT			?	GL_TEXTURE_UPDATE_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & BUFFER_UPDATE_BARRIER_BIT			?	GL_BUFFER_UPDATE_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & FRAMEBUFFER_BARRIER_BIT			?	GL_FRAMEBUFFER_BARRIER_BIT			: 0;
	gl_barrier_bits |= barrier & TRANSFORM_FEEDBACK_BARRIER_BIT		?	GL_TRANSFORM_FEEDBACK_BARRIER_BIT	: 0;
	gl_barrier_bits |= barrier & ATOMIC_COUNTER_BARRIER_BIT			?	GL_ATOMIC_COUNTER_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & SHADER_STORAGE_BARRIER_BIT			?	GL_SHADER_STORAGE_BARRIER_BIT		: 0;
	gl_barrier_bits |= barrier & QUERY_BUFFER_BARRIER_BIT			?	GL_QUERY_BUFFER_BARRIER_BIT			: 0;
	gl_barrier_bits |= barrier & ALL_BARRIER_BITS					?	GL_ALL_BARRIER_BITS					: 0;

	if (gl_barrier_bits != 0) {
		GLCall(glMemoryBarrier(gl_barrier_bits));
	}
}

void ComputeProgram::bind()
{
	if (!_program_generated) {
		std::cout << "[OpenGL Error] ComputeProgram tried to bind() but it was released" << std::endl;
		ASSERT(false);
	}

	if (!_program_compiled) {
		std::cout << "[OpenGL Error] ComputeProgram tried to bind() but it wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glUseProgram(id));
}

void ComputeProgram::unbind()
{
	GLCall(glUseProgram(0));
}

void ComputeProgram::compile_shader(const Shader& shader)
{
	Shader shader_to_use = shader;
	if (_preprocessing_defines.size() != 0) {
		Shader variant;
		std::string line;
		std::stringstream compute_shader_stream(shader.compute_shader);
		while (std::getline(compute_shader_stream, line)) {
			if (int define_begin = line.find("#define") != std::string::npos) {
				
				int name_position_begin = define_begin + 7;
				while (name_position_begin < line.size() && line.at(name_position_begin) == ' ') { name_position_begin++; }
				
				int name_position_end = name_position_begin;
				while (name_position_end < line.size() && line.at(name_position_end) != ' ') { name_position_end++; }

				int value_position_begin = name_position_end;
				while (value_position_begin < line.size() && line.at(value_position_begin) == ' ') { value_position_begin++; }

				int value_position_end = value_position_begin;
				while (value_position_end < line.size() && line.at(value_position_end) != ' ') { value_position_end++; }

				std::string define_name = line.substr(name_position_begin, name_position_end-name_position_begin);
				bool has_key = value_position_begin != value_position_end;
				
				if (_preprocessing_defines.find(define_name) != _preprocessing_defines.end()) {
					if (!has_key) line = line.substr(0, name_position_end) + " " + _preprocessing_defines[define_name];
					else line.replace(value_position_begin, value_position_end, _preprocessing_defines[define_name]);
				}
			}

			variant.compute_shader += line + "\n";
		}

		shader_to_use = variant;
	}

	unsigned int compute_shader;
	GLCall(compute_shader = glCreateShader(GL_COMPUTE_SHADER));
	const char* string = shader_to_use.compute_shader.c_str();
	GLCall(glShaderSource(compute_shader, 1, &string, nullptr));

	GLCall(glCompileShader(compute_shader));

	int is_compiled = 0;
	GLCall(glGetShaderiv(compute_shader, GL_COMPILE_STATUS, &is_compiled));
	if (is_compiled == GL_FALSE) {
		int max_length = 0;
		GLCall(glGetShaderiv(compute_shader, GL_INFO_LOG_LENGTH, &max_length));
		std::string info_log(max_length, 0);
		GLCall(glGetShaderInfoLog(compute_shader, max_length, &max_length, &info_log[0]));
		GLCall(glDeleteShader(compute_shader));

		std::cout << "[OpenGL Error] Compute Shader \"" << shader.filename << "\" failed to compiler with message : " << info_log << std::endl;;
		ASSERT(false);
		return;
	}

	GLCall(glAttachShader(id, compute_shader));
	GLCall(glLinkProgram(id));

	int is_linked = 0;
	GLCall(glGetProgramiv(id, GL_LINK_STATUS, (int*)&is_linked));
	if (is_linked == GL_FALSE) {
		GLint max_length = 0;
		GLCall(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length));
		std::string info_log(max_length, 0);
		GLCall(glGetProgramInfoLog(id, max_length, &max_length, &info_log[0]));
		GLCall(glDeleteProgram(id));
		GLCall(glDeleteShader(compute_shader));

		std::cout << "[OpenGL Error] Compute Shader \"" << shader.filename << "\" failed to link with message : " << info_log << std::endl;;
		ASSERT(false);
		return;
	}

	GLCall(glDetachShader(id, compute_shader));

	_program_compiled = true;
	_work_group_size = get_work_group_size();
}

void ComputeProgram::clear_preprocessor()
{
	_preprocessing_defines.clear();
}

void ComputeProgram::set_preprocessor(const std::string& key, const std::string& value)
{
	_preprocessing_defines[key] = value;
}

std::string ComputeProgram::get_preprocessor(const std::string& key)
{
	return _preprocessing_defines[key];
}

glm::ivec3 ComputeProgram::get_work_group_size()
{
	if (!_program_generated) {
		std::cout << "[OpenGL Error] ComputeProgram tried to get_work_group_size() but it was released" << std::endl;
		ASSERT(false);
	}

	if (!_program_compiled) {
		std::cout << "[OpenGL Error] ComputeProgram tried to get_work_group_size() but it wasn't compiled yet" << std::endl;
		ASSERT(false);
	}
	
	int work_group_size[3];

	glGetProgramiv(id, GL_COMPUTE_WORK_GROUP_SIZE, work_group_size);

	return glm::ivec3(work_group_size[0], work_group_size[1], work_group_size[2]);
}

void ComputeProgram::update_uniform_as_storage_buffer(const std::string& name, Buffer& buffer, size_t offset, size_t size) {
	
	// I don't think they are necessary
	//buffer.wait_to_sycronize_upload();
	//buffer.wait_to_sycronize_download();

	if (!buffer._buffer_allocated) buffer._allocate_buffer(buffer._buffer_size);

	GLCall(int slot = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name.c_str()));
	if (slot == -1) {	// maybe
		ASSERT(false);
	}

	buffer.bind_as_storage_buffer(slot, offset, size);
}

void ComputeProgram::update_uniform(const std::string& name, Texture1D& texture1d)
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

void ComputeProgram::update_uniform(const std::string& name, Texture2D& texture2d)
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

void ComputeProgram::update_uniform(const std::string& name, Texture3D& texture3d)
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

void ComputeProgram::update_uniform(const std::string& name, Texture2DArray& texture2darray)
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

void ComputeProgram::update_uniform(const std::string& name, TextureCubeMap& texturecubemap)
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

void ComputeProgram::update_uniform_bindless(const std::string& name, Texture1D& texture1d)
{
	if (!texture1d.is_bindless) {
		std::cout << "[OpenGL Error] ComputeProgram tried to update_uniform_bindless() but Texture1D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture1d.wait_async_load();
	if (!texture1d._texture_handle_created) texture1d._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture1d.texture_handle));
}

void ComputeProgram::update_uniform_bindless(const std::string& name, Texture2D& texture2d)
{
	if (!texture2d.is_bindless) {
		std::cout << "[OpenGL Error] ComputeProgram tried to update_uniform_bindless() but Texture2D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2d.wait_async_load();
	if (!texture2d._texture_handle_created) texture2d._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture2d.texture_handle));
}

void ComputeProgram::update_uniform_bindless(const std::string& name, Texture3D& texture3d)
{
	if (!texture3d.is_bindless) {
		std::cout << "[OpenGL Error] ComputeProgram tried to update_uniform_bindless() but Texture3D.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture3d.wait_async_load();
	if (!texture3d._texture_handle_created) texture3d._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture3d.texture_handle));
}

void ComputeProgram::update_uniform_bindless(const std::string& name, Texture2DArray& texture2darray)
{
	if (!texture2darray.is_bindless) {
		std::cout << "[OpenGL Error] ComputeProgram tried to update_uniform_bindless() but Texture2DArray.is_bindless was false" << std::endl;
		ASSERT(false);
	}

	texture2darray.wait_async_load();
	if (!texture2darray._texture_handle_created) texture2darray._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture2darray.texture_handle));
}

void ComputeProgram::update_uniform_bindless(const std::string& name, TextureCubeMap& texturecubemap)
{
	//if (!texturecubemap.is_bindless) {
	//	std::cout << "[OpenGL Error] ComputeProgram tried to update_uniform_bindless() but TextureCubeMap.is_bindless was false" << std::endl;
	//	ASSERT(false);
	//}

	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_handle_created) texturecubemap._allocate_texture();
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texturecubemap.texture_handle));
}

void ComputeProgram::update_uniform_as_image(const std::string& name, Texture1D& texture1d, int mipmap_level)
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

void ComputeProgram::update_uniform_as_image(const std::string& name, Texture2D& texture2d, int mipmap_level)
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

void ComputeProgram::update_uniform_as_image(const std::string& name, Texture3D& texture3d, int mipmap_level)
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

void ComputeProgram::update_uniform_as_image(const std::string& name, Texture2DArray& texture2darray, int mipmap_level)
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

void ComputeProgram::update_uniform(const std::string& name, const int& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), a));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2i(id, _get_uniform_location(name), a, b));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3i(id, _get_uniform_location(name), a, b, c));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4i(id, _get_uniform_location(name), a, b, c, d));
}

void ComputeProgram::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4ui(id, _get_uniform_location(name), a, b, c, d));
}

void ComputeProgram::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3ui(id, _get_uniform_location(name), a, b, c));
}

void ComputeProgram::update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2ui(id, _get_uniform_location(name), a, b));
}

void ComputeProgram::update_uniform(const std::string& name, const unsigned int& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1ui(id, _get_uniform_location(name), a));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform1f(id, _get_uniform_location(name), a));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform2f(id, _get_uniform_location(name), a, b));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform3f(id, _get_uniform_location(name), a, b, c));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniform4f(id, _get_uniform_location(name), a, b, c, d));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec2& a)
{
	update_uniform(name, a.x, a.y);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::ivec2& a)
{
	update_uniform(name, a.x, a.y);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::ivec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::ivec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::uvec2& a)
{
	update_uniform(name, a.x, a.y);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::uvec3& a)
{
	update_uniform(name, a.x, a.y, a.z);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::uvec4& a)
{
	update_uniform(name, a.x, a.y, a.z, a.w);
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat2& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix2fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat3& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix3fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat4& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix4fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat3x4& a)
{
	if (!_does_uniform_exist(name)) return;
	GLCall(glProgramUniformMatrix3x4fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::_generate_program()
{
	if (_program_generated) return;

	GLCall(id = glCreateProgram());
	_program_generated = true;
}

bool ComputeProgram::_does_uniform_exist(const std::string& name) {
	if (!_program_generated) {
		std::cout << "[OpenGL Error] ComputeProgram tried to _does_uniform_exist() but ComputeProgram was released" << std::endl;
		ASSERT(false);
	}
	if (!_program_compiled) {
		std::cout << "[OpenGL Error] ComputeProgram tried to _does_uniform_exist() but ComputeProgram wasn't compiled yet" << std::endl;
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

int ComputeProgram::_get_uniform_location(const std::string& name)
{
	if (!_program_generated) {
		std::cout << "[OpenGL Error] ComputeProgram tried to _get_unifrom_location() but ComputeProgram was released" << std::endl;
		ASSERT(false);
	}
	if (!_program_compiled) {
		std::cout << "[OpenGL Error] ComputeProgram tried to _get_uniform_location() but ComputeProgram wasn't compiled yet" << std::endl;
		ASSERT(false);
	}

	auto index = _uniform_location_table.find(name);
	if (index != _uniform_location_table.end())
		return index->second;
	else {
		GLCall(int location = glGetUniformLocation(id, name.c_str()));
		if (location == -1) {
			std::cout << "[OpenGL Error] ComputeProgram tried to _get_uniform_location() but no uniform with name: \"" << name << "\" was found" << std::endl;
			ASSERT(false);
		}
		_uniform_location_table[name] = location;
		return location;
	}
}
