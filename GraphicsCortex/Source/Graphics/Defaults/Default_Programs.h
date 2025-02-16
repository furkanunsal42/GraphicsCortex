#pragma once

#include <glm.hpp>
#include <vector>
#include "ShaderCompiler.h"
#include <memory>

namespace default_program {
	Program solid_program();
	Program solid_program_multitexture();
	Program flatcolor_program();
	Program framebuffer_program();
	Program cubemap_program();
	Program gui_program();
	Program text_program();
	Program depth_program();

	std::shared_ptr<Program> solid_program_s();
	std::shared_ptr<Program> solid_program_multitexture_s();
	std::shared_ptr<Program> flatcolor_program_s();
	std::shared_ptr<Program> framebuffer_program_s();
	std::shared_ptr<Program> cubemap_program_s();
	std::shared_ptr<Program> gui_program_s();
	std::shared_ptr<Program> text_program_s();
	std::shared_ptr<Program> depth_program_s();
}
