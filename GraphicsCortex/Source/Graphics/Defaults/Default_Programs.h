#pragma once

#include <glm.hpp>
#include <vector>
#include "ShaderCompiler.h"
#include <memory>

namespace default_program {
	
	namespace debug {

		Program flatcolor_program();
		Program normal_program();
		Program normal_abs_program();
		Program texcoord_program();
		Program texcoord_abs_program();
		Program texcoord_pattern_program();

		std::shared_ptr<Program> flatcolor_program_s();
		std::shared_ptr<Program> normal_program_s();
		std::shared_ptr<Program> normal_abs_program_s();
		std::shared_ptr<Program> texcoord_program_s();
		std::shared_ptr<Program> texcoord_abs_program_s();
		std::shared_ptr<Program> texcoord_pattern_program_s();
	}


	Program surface_program();
	Program text_program();

	std::shared_ptr<Program> surface_program_s();
	std::shared_ptr<Program> text_program_s();
}
