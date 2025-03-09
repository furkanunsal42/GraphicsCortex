#pragma once
#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"

#include "Components/Component_Light.h"
#include "Components/Component_Skylight.h"

#include "UniformBuffer.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Constants.h"

class RenderPass_Shadowmaps : public RenderPass {
public:

	RenderPass_Shadowmaps() = default;

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

		depth_program_cubemap = std::make_shared<Program>(
			Shader(
				default_paths::shadowmaps / "point_shadowmap.vert",
				default_paths::shadowmaps / "point_shadowmap.geom",
				default_paths::shadowmaps / "point_shadowmap.frag"
			)
		);

		depth_program = std::make_shared<Program>(
			Shader(
				default_paths::shadowmaps / "shadowmap.vert",
				default_paths::shadowmaps / "shadowmap.frag"
			)
		);

		if (framebuffer == nullptr)
			framebuffer = std::make_shared<Framebuffer>();
	}

	void init_shadowmaps() {
		if (framebuffer == nullptr)
			framebuffer = std::make_shared<Framebuffer>();

		if (d_shadowmaps == nullptr ||
			d_shadowmap_resolution != d_shadowmaps->get_size().x ||
			d_shadowmap_cascade_count != d_shadowmaps->get_size().z ||
			shadowmap_format != d_shadowmaps->get_internal_format_depthstencil()
			) {
			d_shadowmaps = std::make_shared<Texture2DArray>(
				d_shadowmap_resolution, d_shadowmap_resolution, 
				d_shadowmap_cascade_count, 
				shadowmap_format, 1, 0, 0
			);
		}

	}

	void render_scene(std::span<MeshRendererComponent*> mesh_renderer_components, Program& program, const glm::mat4& view_matrix, const glm::mat4& projection_matrix) {

		if (mesh_renderer_components.size() == 0)
			return;

		for (MeshRendererComponent* mesh_renderer : mesh_renderer_components) {

			TransformComponent* transform_c = mesh_renderer->get_transform();
			if (transform_c == nullptr) continue;
			glm::mat4 model_matrix = transform_c->get_matrix();

			MeshComponent* mesh_c = mesh_renderer->get_mesh();
			if (mesh_c == nullptr) continue;

			std::shared_ptr<Mesh> mesh = mesh_c->mesh;
			if (mesh == nullptr) continue;

			Camera camera;
			camera.update_matrixes();
			camera.update_default_uniforms(program);

			RenderParameters render_parameters;

			program.update_uniform("view", view_matrix);
			program.update_uniform("projection", projection_matrix);

			mesh->traverse([&](Mesh::Node& node, glm::mat4& transform) {

				if (node.get_submeshes().size() == 0) return;

				program.update_uniform("model", model_matrix * transform);

				for (mesh_t submesh : node.get_submeshes()) {

					primitive_renderer::render(
						*depth_program_cubemap,
						*mesh->get_mesh(submesh),
						render_parameters
					);
				}
				});

			//pipeline.framebuffer->bind_draw();
			//mesh_renderer->render(camera);
		}
	}

	std::array<glm::vec3, 8> get_frustum_corners(Camera& camera) {
		
		const glm::mat4 inverse_matrix = glm::inverse(camera.projection_matrix * camera.view_matrix);
		
		std::array<glm::vec3, 8> frustum_corners;

		for (unsigned int x = 0; x < 2; ++x){
			for (unsigned int y = 0; y < 2; ++y){
				for (unsigned int z = 0; z < 2; ++z){
					const glm::vec4 corner4 =
						inverse_matrix * glm::vec4(
							2.0f * x - 1.0f,
							2.0f * y - 1.0f,
							2.0f * z - 1.0f,
							1.0f);
					frustum_corners[z*2*2 + y*2 + x] = glm::vec3(corner4) / corner4.w;
				}
			}
		}

		return frustum_corners;
	}

	glm::vec3 get_frustum_center(const std::array<glm::vec3, 8>& frustum_corners) {
		
		glm::vec3 center = glm::vec3(0, 0, 0);
		
		for (const glm::vec3& v : frustum_corners)
			center += v;
		
		center /= frustum_corners.size();
	}

	glm::mat4 get_projection_matrix_from_frustum_ortho(const std::array<glm::vec3, 8>& frustum_corners, const glm::mat4& light_view_matrix, float z_multiplier = 10) {
		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (const glm::vec3& v : frustum_corners)
		{
			const auto trf = light_view_matrix * glm::vec4(v, 1);
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		if (minZ < 0)
			minZ *= z_multiplier;
		else
			minZ /= z_multiplier;
		
		if (maxZ < 0)
			maxZ /= z_multiplier;
		else
			maxZ *= z_multiplier;

		glm::mat4 light_projection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return light_projection;
	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

		std::span<MeshRendererComponent*> mesh_renderer_components = scene.get_components<MeshRendererComponent>();
		std::span<LightComponent*> light_components = scene.get_components<LightComponent>();

		uint32_t d_light_count = 0;
		uint32_t p_light_count = 0;
		uint32_t s_light_count = 0;

		// count the light types
		for (LightComponent* light : light_components) {
			
			Entity* entity = light->get_entity();
			if (entity == nullptr) {
				ASSERT(false);
			}
			std::shared_ptr<TransformComponent> transform_c = entity->get_component<TransformComponent>();
			if (transform_c == nullptr)
				continue;

			switch (light->type) {
			case LightComponent::directional:
				d_light_count++;
				break;
			case LightComponent::point:
				p_light_count++;
				break;
			case LightComponent::spot:
				s_light_count++;
				break;
			}
		}

		std::array<glm::vec3, 8> frustum_corners = get_frustum_corners(camera);
		glm::vec3 frustum_center = get_frustum_center(frustum_corners);

		uint32_t d_light_index = 0;
		uint32_t p_light_index = 0;
		uint32_t s_light_index = 0;
		
		init_shadowmaps();

		for (LightComponent* light : light_components) {
			Entity* entity = light->get_entity();
			if (entity == nullptr) {
				ASSERT(false);
			}
			std::shared_ptr<TransformComponent> light_transform_c = entity->get_component<TransformComponent>();
			if (light_transform_c == nullptr)
				continue;

			TransformComponent& light_transform = *light_transform_c;

			switch (light->type) {
			case LightComponent::directional:

				glm::mat4 light_view_matrix = glm::lookAt(
					frustum_center, 
					frustum_center + light_transform.get_z_direction(),
					glm::vec3(0, 1, 0)
				);

				glm::mat4 light_projection_ortho = get_projection_matrix_from_frustum_ortho(frustum_corners, light_view_matrix, 10);

				framebuffer->attach_depth_layered(*d_shadowmaps);

				render_scene(mesh_renderer_components, *depth_program, light_view_matrix, light_projection_ortho);

				break;
			case LightComponent::point:
				break;
			case LightComponent::spot:
				break;
			}
		}
	}
	
	uint32_t d_shadowmap_resolution = 1024;
	uint32_t d_shadowmap_cascade_count = 6;
	uint32_t p_shadowmap_cube_resolution = 1024;
	uint32_t p_shadowmap_cube_count = 4;
	uint32_t s_shadowmap_resolution = 1024;
	uint32_t s_shadowmap_count = 4;

	TextureBase2::DepthStencilTextureFormat shadowmap_format = Texture2D::DepthStencilTextureFormat::DEPTH_COMPONENT24;

	std::shared_ptr<Program> depth_program_cubemap = nullptr; 
	std::shared_ptr<Program> depth_program = nullptr;

	std::shared_ptr<Framebuffer> framebuffer = nullptr;

	std::shared_ptr<Texture2DArray> d_shadowmaps = nullptr;
	std::shared_ptr<Texture2DArray> p_shadowmaps_cube = nullptr;
	std::shared_ptr<Texture2DArray> s_shadowmaps = nullptr;

	// /*static*/ const std::string directional_light_buffer_name = "d_lights_buffer";
	// /*static*/ const std::string point_light_buffer_name = "p_lights_buffer";
	// /*static*/ const std::string spot_light_buffer_name = "s_lights_buffer";
	// 
	// uint32_t directional_light_max_count = 32;
	// uint32_t point_light_max_count = 32;
	// uint32_t spot_light_max_count = 32;
	// 
	// std::shared_ptr<UniformBuffer> directional_lights_buffer;
	// std::shared_ptr<UniformBuffer> point_lights_buffer;
	// std::shared_ptr<UniformBuffer> spot_lights_buffer;
};
