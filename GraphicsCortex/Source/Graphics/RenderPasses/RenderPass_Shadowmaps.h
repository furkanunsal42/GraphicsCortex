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
	}

	void render_scene(std::span<MeshRendererComponent*> mesh_renderer_components, Program& program) {

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

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

		std::span<MeshRendererComponent*> mesh_renderer_components = scene.get_components<MeshRendererComponent>();
		std::span<LightComponent*> light_components = scene.get_components<LightComponent>();

		uint32_t d_light_count = 0;
		uint32_t p_light_count = 0;
		uint32_t s_light_count = 0;

		// count the light types
		for (LightComponent* light : light_components) {
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

		uint32_t d_light_index = 0;
		uint32_t p_light_index = 0;
		uint32_t s_light_index = 0;
		
		for (LightComponent* light : light_components) {
			switch (light->type) {
			case LightComponent::directional:
				
				break;
			case LightComponent::point:
				break;
			case LightComponent::spot:
				break;
			}
		}
	}
	
	std::shared_ptr<Program> depth_program_cubemap = nullptr; 
	std::shared_ptr<Program> depth_program = nullptr;

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
