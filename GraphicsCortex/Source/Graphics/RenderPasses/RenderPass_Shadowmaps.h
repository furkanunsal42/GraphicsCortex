#pragma once
#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"


class RenderPass_Shadowmaps : public RenderPass {
public:

	RenderPass_Shadowmaps() = default;

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene);
	void init_shadowmaps();
	void render_scene(std::span<MeshRendererComponent*> mesh_renderer_components, Program& program, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);
	std::array<glm::vec3, 8> get_frustum_corners(Camera& camera, float z_percentage = 1, int32_t index = 0);
	glm::vec3 get_frustum_center(const std::array<glm::vec3, 8>& frustum_corners);
	glm::mat4 get_projection_matrix_from_frustum_ortho(const std::array<glm::vec3, 8>& frustum_corners, const glm::mat4& light_view_matrix, float z_multiplier = 10);
	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera);

	static const std::string directional_shadowmap_texture_name;
	static const std::string directional_shadowmap_buffer_name;

	uint32_t d_shadowmap_max_count = 1;
	uint32_t d_shadowmap_cascade_count = 6;
	uint32_t d_shadowmap_resolution = 1024;
	std::shared_ptr<Texture2DArray> d_shadowmaps = nullptr;
	std::shared_ptr<Texture2DArray> d_shadowmaps_color = nullptr;
	std::shared_ptr<UniformBuffer> d_shadowmaps_buffer = nullptr;

	uint32_t p_shadowmap_cube_resolution = 1024;
	uint32_t p_shadowmap_cube_count = 4;
	uint32_t s_shadowmap_resolution = 1024;
	uint32_t s_shadowmap_count = 4;

	TextureBase2::DepthStencilTextureFormat shadowmap_format = Texture2D::DepthStencilTextureFormat::DEPTH_COMPONENT24;

	std::shared_ptr<Program> depth_program_cubemap = nullptr; 
	std::shared_ptr<Program> depth_program = nullptr;

	std::shared_ptr<Framebuffer> framebuffer = nullptr;

	std::shared_ptr<Texture2DArray> p_shadowmaps_cube = nullptr;
	std::shared_ptr<Texture2DArray> s_shadowmaps = nullptr;

	///*static*/ const std::string point_light_buffer_name = "p_lights_buffer";
	///*static*/ const std::string spot_light_buffer_name = "s_lights_buffer";
	
	//uint32_t point_light_max_count = 32;
	//uint32_t spot_light_max_count = 32;
	
	//std::shared_ptr<UniformBuffer> point_lights_buffer;
	//std::shared_ptr<UniformBuffer> spot_lights_buffer;
};
