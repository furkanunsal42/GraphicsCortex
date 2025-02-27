#include "Default_Programs.h"

Program default_program::debug::flatcolor_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"
	);
	Program program(default_shader);
	return program;
}

Program default_program::debug::normal_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normal.frag"
	);
	Program program(default_shader);
	return program;
}

Program default_program::debug::normal_abs_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normalabs.frag"
	);
	Program program(default_shader);
	return program;
}

Program default_program::debug::texcoord_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoord.frag"
	);
	Program program(default_shader);
	return program;
}

Program default_program::debug::texcoord_abs_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoordabs.frag"
	);
	Program program(default_shader);
	return program;
}


// ----------------------------------


std::shared_ptr<Program> default_program::debug::flatcolor_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}

std::shared_ptr<Program> default_program::debug::normal_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normal.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}

std::shared_ptr<Program> default_program::debug::normal_abs_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normalabs.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}

std::shared_ptr<Program> default_program::debug::texcoord_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoord.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}

std::shared_ptr<Program> default_program::debug::texcoord_abs_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoordabs.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}

Program default_program::surface_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Surface/surface.vert",
		"../GraphicsCortex/Source/GLSL/Surface/surface.frag"
	);
	Program program = Program(default_shader);
	return program;
}

std::shared_ptr<Program> default_program::surface_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Surface/surface.vert",
		"../GraphicsCortex/Source/GLSL/Surface/surface.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}
