#pragma once

#include <string>
#include <filesystem>

#include "glm.hpp"
#include "gtx/quaternion.hpp"

#include "Texture2D.h"
#include "VertexAttributeBuffer.h"
#include "ShaderCompiler.h"
#include "ComputeProgram.h"
#include "FrameBuffer.h"
#include "Camera.h"

#include "Tools/GraphicsOperation/GraphicsOperation.h"

extern std::filesystem::path curved_surface_parent_shader_path;

class CurvedSurface {
public:

	void define_surface(const std::string& curve_definition, glm::ivec2 resolution = glm::ivec2(1024));
	void set_position(glm::vec3 position);
	void set_rotation(glm::quat quaternion);
	void set_scale(glm::vec3 scale);

	glm::vec3 get_position();
	glm::quat get_rotation();
	glm::vec3 get_scale();

	void render(Framebuffer& target_framebuffer, Camera& camera);
	void render(Camera& camera);

private:

	void compile_shaders();
	void generate_mesh_from_texture();

	glm::vec3 position = glm::vec3(0);
	glm::quat quaternion;
	glm::vec3 scale = glm::vec3(1);
	
	bool shaders_compiled = false;
	static constexpr Texture2D::ColorTextureFormat surface_height_map_internal_format = Texture2D::ColorTextureFormat::R32F;

	std::shared_ptr<Texture2D> surface_height_map = nullptr;
	std::shared_ptr<GraphicsOperation> operation = nullptr;
	
	std::shared_ptr<ComputeProgram> cp_texture_to_mesh = nullptr;

	std::shared_ptr<VertexAttributeBuffer> vab = nullptr;
	std::shared_ptr<Buffer> index_buffer = nullptr;

	std::shared_ptr<Program> surface_program = nullptr;

};