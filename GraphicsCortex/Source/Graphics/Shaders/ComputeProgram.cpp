#include "ComputeProgram.h"
#include "Debuger.h"

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
}

void ComputeProgram::update_uniform(const std::string& name, TextureCubeMap& texturecubemap)
{
}

void ComputeProgram::update_uniform(const std::string& name, const bool& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const bool& a, const bool& b)
{
}

void ComputeProgram::update_uniform(const std::string& name, const bool& a, const bool& b, const bool& c)
{
}

void ComputeProgram::update_uniform(const std::string& name, const bool& a, const bool& b, const bool& c, const bool& d)
{
}

void ComputeProgram::update_uniform(const std::string& name, const int& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b)
{
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c)
{
}

void ComputeProgram::update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d)
{
}

void ComputeProgram::update_uniform(const std::string& name, const float& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b)
{
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c)
{
}

void ComputeProgram::update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec2& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec3& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::vec4& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat2& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat3& a)
{
}

void ComputeProgram::update_uniform(const std::string& name, const glm::mat4& a)
{
}

void ComputeProgram::_generate_program()
{
	if (_program_generated) return;

	GLCall(id = glCreateProgram());
	_program_generated = true;
}
