#pragma once

#include <memory>

#include "Constants.h"
#include "ShaderCompiler.h"
#include "Mesh.h"
#include "FrameBuffer.h"

class EquirectangularProjector {
public:

	EquirectangularProjector();

	void project_to_cubemap(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture, uint32_t mipmap = 0);

	//void project_to_equirectangular(TextureCubeMap& cubemap_texture, Texture2D& equirectangular_texture);
	//std::shared_ptr<Texture2D> project_to_equirectangular(TextureCubeMap& cubemap_texture);

	
private:
	void init();

	bool initialized = false;
	Mesh unit_cube;
	std::shared_ptr<Program> to_cubemap_projection_program = nullptr;
	//std::shared_ptr<Program> to_equiengular_projection_program = nullptr;
	std::shared_ptr<Framebuffer> framebuffer = nullptr;
};