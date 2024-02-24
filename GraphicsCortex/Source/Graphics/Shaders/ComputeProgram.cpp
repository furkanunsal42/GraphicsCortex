#include "ComputeProgram.h"
#include "Debuger.h"
#include "Texture2D.h"
#include "TextureCubeMap.h"

ComputeProgram::ComputeProgram()
{
	_generate_program();
}

ComputeProgram::ComputeProgram(const Shader& shader)
{
	_generate_program();
	load_shader(shader);
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
	glDispatchCompute()
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

void ComputeProgram::load_shader(const Shader& shader)
{
	unsigned int compute_shader;
	GLCall(compute_shader = glCreateShader(GL_COMPUTE_SHADER));
	const char* string = shader.compute_shader.c_str();
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

		std::cout << "[OpenGL Error] Compute Shader \"" << shader.filename << "\" failed to compiler with message : " << info_log;
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

		std::cout << "[OpenGL Error] Compute Shader \"" << shader.filename << "\" failed to link with message : " << info_log;
		ASSERT(false);
		return;
	}

	GLCall(glDetachShader(id, compute_shader));

	_program_compiled = true;
}

void ComputeProgram::update_uniform(const std::string& name, Texture2D& texture2d)
{
	texture2d.wait_async_load();
	if (!texture2d._texture_handle_created) texture2d._allocate_texture();
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texture2d.texture_handle));
}

void ComputeProgram::update_uniform(const std::string& name, TextureCubeMap& texturecubemap)
{
	texturecubemap.wait_async_load();
	if (!texturecubemap._texture_handle_created) texturecubemap._allocate_texture();
	GLCall(glProgramUniformHandleui64ARB(id, _get_uniform_location(name), texturecubemap.texture_handle));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a)
{
	GLCall(glProgramUniform1i(id, _get_uniform_location(name), a));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b)
{
	GLCall(glProgramUniform2i(id, _get_uniform_location(name), a, b));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c)
{
	GLCall(glProgramUniform3i(id, _get_uniform_location(name), a, b, c));
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d)
{
	GLCall(glProgramUniform4i(id, _get_uniform_location(name), a, b, c, d));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a)
{
	GLCall(glProgramUniform1f(id, _get_uniform_location(name), a));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b)
{
	GLCall(glProgramUniform2f(id, _get_uniform_location(name), a, b));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c)
{
	GLCall(glProgramUniform3f(id, _get_uniform_location(name), a, b, c));
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d)
{
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

void ComputeProgram::update_uniform(const std::string& name, const glm::mat2& a)
{
	GLCall(glProgramUniformMatrix2fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat3& a)
{
	GLCall(glProgramUniformMatrix3fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat4& a)
{
	GLCall(glProgramUniformMatrix4fv(id, _get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a)));
}

void ComputeProgram::_generate_program()
{
	if (_program_generated) return;

	GLCall(id = glCreateProgram());
	_program_generated = true;
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
		_uniform_location_table[name] = location;
		return location;
	}
}
