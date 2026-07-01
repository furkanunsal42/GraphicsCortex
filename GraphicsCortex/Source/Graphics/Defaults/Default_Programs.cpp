#include "Default_Programs.h"

Program default_program::debug::flatcolor_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"
	);
	return Program(default_shader);
}

Program default_program::debug::normal_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normal.frag"
	);
	return Program(default_shader);
}

Program default_program::debug::normal_abs_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/normalabs.frag"
	);
	return Program(default_shader);
}

Program default_program::debug::texcoord_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoord.frag"
	);
	return Program(default_shader);
}

Program default_program::debug::texcoord_abs_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoordabs.frag"
	);
	return Program(default_shader);
}

Program default_program::debug::texcoord_pattern_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoord_pattern.frag"
	);
	return Program(default_shader);
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

std::shared_ptr<Program> default_program::debug::texcoord_pattern_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Debug/basic.vert",
		"../GraphicsCortex/Source/GLSL/Debug/texcoord_pattern.frag"
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
	return Program(default_shader);
}

Program default_program::text_program()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Text/text.vert",
		"../GraphicsCortex/Source/GLSL/Text/text.frag"
	);
	return Program(default_shader);
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

std::shared_ptr<Program> default_program::text_program_s()
{
	Shader default_shader(
		"../GraphicsCortex/Source/GLSL/Text/text.vert",
		"../GraphicsCortex/Source/GLSL/Text/text.frag"
	);
	std::shared_ptr<Program> program = std::make_shared<Program>(default_shader);
	return program;
}
