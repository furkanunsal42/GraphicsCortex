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

		std::shared_ptr<Program> flatcolor_program_s();
		std::shared_ptr<Program> normal_program_s();
		std::shared_ptr<Program> normal_abs_program_s();
		std::shared_ptr<Program> texcoord_program_s();
		std::shared_ptr<Program> texcoord_abs_program_s();

	}


	//Program solid_program();
	//std::shared_ptr<Program> solid_program_s();
}
