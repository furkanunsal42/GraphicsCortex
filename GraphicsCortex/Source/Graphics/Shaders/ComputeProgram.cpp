#include "ComputeProgram.h"
#include "Debuger.h"

ComputeProgram::ComputeProgram()
{
	_generate_program();
}

ComputeProgram::ComputeProgram(const Shader& shader)
{
	_generate_program();
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
	}
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

}

void ComputeProgram::_generate_program()
{
	if (_program_generated) return;

	GLCall(id = glCreateProgram());
	_program_generated = true;
}
