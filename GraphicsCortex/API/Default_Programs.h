#pragma once

#include "Scene.h"
#include "CubeMap.h"
#include "UnifromQueue.h"
#include "Text.h"

#include "RenderPipeline.h"

#include <glm.hpp>
#include <vector>

#include "CustomRenderPipelines.h"

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

	RenderPipeline default_pipeline(Frame& frame);
	RenderPipeline_MultiTextured multitextured_pipeline(Frame& frame);


}
