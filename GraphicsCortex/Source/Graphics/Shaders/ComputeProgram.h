#pragma once
#include "ShaderCompiler.h"

class ComputeProgram {
public:
	unsigned int id;

	ComputeProgram();
	ComputeProgram(const Shader& shader);
	ComputeProgram(const ComputeProgram& other) = delete;
	~ComputeProgram();
	void release();

	void load_shader(const Shader& shader);


private:

	bool _program_generated = false;


	void _generate_program();

};