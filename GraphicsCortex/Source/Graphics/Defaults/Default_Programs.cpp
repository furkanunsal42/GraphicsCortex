#include "Default_Programs.h"

namespace default_program {

	Program solid_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Solid.vert", "../GraphicsCortex/Source/GLSL/Solid.geom", "../GraphicsCortex/Source/GLSL/Solid.frag");
		Program solid_program(default_shader);
		return solid_program;
	}
	Program solid_program_multitexture() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Solid_multitexture.vert", "../GraphicsCortex/Source/GLSL/Solid_multitexture.geom", "../GraphicsCortex/Source/GLSL/Solid_multitexture.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program flatcolor_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/FlatColor.vert", "../GraphicsCortex/Source/GLSL/FlatColor.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program framebuffer_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/FrameBuffer.vert", "../GraphicsCortex/Source/GLSL/FrameBuffer.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program cubemap_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/CubeMap.vert", "../GraphicsCortex/Source/GLSL/CubeMap.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program gui_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Gui.vert", "../GraphicsCortex/Source/GLSL/Gui.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program text_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Text.vert", "../GraphicsCortex/Source/GLSL/Text.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program depth_program() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Depth.vert", "../GraphicsCortex/Source/GLSL/Depth.frag");
		Program renderer(default_shader);
		return renderer;
	}

	std::shared_ptr<Program> solid_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Solid.vert", "../GraphicsCortex/Source/GLSL/Solid.geom", "../GraphicsCortex/Source/GLSL/Solid.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> solid_program_multitexture_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Solid_multitexture.vert", "../GraphicsCortex/Source/GLSL/Solid_multitexture.geom", "../GraphicsCortex/Source/GLSL/Solid_multitexture.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> flatcolor_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/FlatColor.vert", "../GraphicsCortex/Source/GLSL/FlatColor.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> framebuffer_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/FrameBuffer.vert", "../GraphicsCortex/Source/GLSL/FrameBuffer.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> cubemap_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/CubeMap.vert", "../GraphicsCortex/Source/GLSL/CubeMap.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> gui_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Gui.vert", "../GraphicsCortex/Source/GLSL/Gui.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> text_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Text.vert", "../GraphicsCortex/Source/GLSL/Text.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> depth_program_s() {
		Shader default_shader("../GraphicsCortex/Source/GLSL/Depth.vert", "../GraphicsCortex/Source/GLSL/Depth.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
}