#pragma once
#include "RenderPipeline.h"
#include "Components/Component_MeshRenderer.h"

#include "Components/Component_DirectionalLight.h"
#include "Components/Component_PointLight.h"
#include "Components/Component_SpotLight.h"
//#include "Components/Component_Skylight.h"

#include "UniformBuffer.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"

class RenderPass_Forward : public RenderPass {
public:

	RenderPass_Forward() = default;

	void on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

		directional_lights_buffer = std::make_shared<UniformBuffer>();
		point_lights_buffer = std::make_shared<UniformBuffer>();
		spot_lights_buffer = std::make_shared<UniformBuffer>();

		directional_lights_buffer->push_variable_array(directional_light_max_count);	// vec4 directions
		directional_lights_buffer->push_variable_array(directional_light_max_count);	// vec4 colors
		directional_lights_buffer->push_variable<int32_t>();							// int count

		point_lights_buffer->push_variable_array(point_light_max_count);	// vec4 positions
		point_lights_buffer->push_variable_array(point_light_max_count);	// vec4 colors
		point_lights_buffer->push_variable<int32_t>();						// int count

		spot_lights_buffer->push_variable_array(spot_light_max_count);	// vec4 positions
		spot_lights_buffer->push_variable_array(spot_light_max_count);	// vec4 direction
		spot_lights_buffer->push_variable_array(spot_light_max_count);	// vec4 colors
		spot_lights_buffer->push_variable<int32_t>();					// int count

	}

	void update_light_buffers(Scene& scene) {
		std::span<DirectionalLightComponent*> d_lights_comps = scene.get_components<DirectionalLightComponent>();
		std::span<PointLightComponent*> p_lights_comps = scene.get_components<PointLightComponent>();
		std::span<SpotLightComponent*> s_lights_comps = scene.get_components<SpotLightComponent>();

		{
			uint32_t d_light_count = std::min(d_lights_comps.size(), (size_t)directional_light_max_count);
			for (uint32_t i = 0; i < std::min(d_lights_comps.size(), (size_t)directional_light_max_count); i++) {
				DirectionalLight& light = d_lights_comps[i]->directional_light;
				auto transform_c = d_lights_comps[i]->get_entity()->get_component<TransformComponent>();
				if (transform_c == nullptr) continue;
				glm::vec4 position_data = glm::vec4(glm::vec3(transform_c->transform[3]), 0);

				glm::vec4 direction_data = glm::vec4(light.direction, 0);
				glm::vec4 color_data = glm::vec4(light.color, 0);
				directional_lights_buffer->set_data(sizeof(glm::vec4)*i, 0, sizeof(glm::vec4), &direction_data);
				directional_lights_buffer->set_data(sizeof(glm::vec4)*directional_light_max_count + sizeof(glm::vec4) * i, 0, sizeof(glm::vec4), &color_data);
			}
			directional_lights_buffer->set_data(sizeof(glm::vec4)*2*directional_light_max_count, 0, sizeof(int32_t), &d_light_count);
		}

		{
			uint32_t p_light_count = std::min(p_lights_comps.size(), (size_t)point_light_max_count);
			for (uint32_t i = 0; i < std::min(p_lights_comps.size(), (size_t)point_light_max_count); i++) {
				PointLight& light = p_lights_comps[i]->point_light;
				auto transform_c = p_lights_comps[i]->get_entity()->get_component<TransformComponent>();
				if (transform_c == nullptr) continue;

				glm::vec4 position_data = glm::vec4(glm::vec3(transform_c->transform[3]), 0);
				glm::vec4 color_data = glm::vec4(light.color, 0);
				point_lights_buffer->set_data(sizeof(glm::vec4)*i, 0, sizeof(glm::vec4), &position_data);
				point_lights_buffer->set_data(sizeof(glm::vec4)*point_light_max_count + sizeof(glm::vec4) * i, 0, sizeof(glm::vec4), &color_data);
			}
			point_lights_buffer->set_data(sizeof(glm::vec4)*2*point_light_max_count, 0, sizeof(int32_t), &p_light_count);
		}

		{
			uint32_t s_light_count = std::min(s_lights_comps.size(), (size_t)spot_light_max_count);
			for (uint32_t i = 0; i < std::min(s_lights_comps.size(), (size_t)spot_light_max_count); i++) {
				SpotLight& light = s_lights_comps[i]->spot_light;
				auto transform_c = s_lights_comps[i]->get_entity()->get_component<TransformComponent>();
				if (transform_c == nullptr) continue;
				glm::vec4 position_data = glm::vec4(glm::vec3(transform_c->transform[3]), 0);

				glm::vec4 direction_data = glm::vec4(light.direction, glm::cos(light.angle_radian));
				glm::vec4 color_data = glm::vec4(light.color, 0);
				spot_lights_buffer->set_data(sizeof(glm::vec4)*i, 0, sizeof(glm::vec4), &position_data);
				spot_lights_buffer->set_data(sizeof(glm::vec4)*spot_light_max_count + sizeof(glm::vec4) * i, 0, sizeof(glm::vec4), &direction_data);
				spot_lights_buffer->set_data(sizeof(glm::vec4)*2*spot_light_max_count + sizeof(glm::vec4) * i, 0, sizeof(glm::vec4), &color_data);
			}
			spot_lights_buffer->set_data(sizeof(glm::vec4)*3*spot_light_max_count, 0, sizeof(int32_t), &s_light_count);
		}

		directional_lights_buffer->upload_data();
		point_lights_buffer->upload_data();
		spot_lights_buffer->upload_data();
	}

	void on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

		std::span<MeshRendererComponent*> mesh_renderers = scene.get_components<MeshRendererComponent>();
		if (mesh_renderers.size() == 0) 
			return;

		update_light_buffers(scene);

		for (MeshRendererComponent* mesh_renderer : mesh_renderers) {
			MaterialComponent* material_c = nullptr;
			std::shared_ptr<Program> program = nullptr;
			material_c = mesh_renderer->get_material();
			if (material_c != nullptr)
				program = material_c->get_program();
			if (program != nullptr) {
				program->update_uniform(directional_light_buffer_name, *directional_lights_buffer);
				program->update_uniform(point_light_buffer_name, *point_lights_buffer);
				program->update_uniform(spot_light_buffer_name, *spot_lights_buffer);
			}

			mesh_renderer->render(camera);
		}
	}

	/*static*/ const std::string directional_light_buffer_name = "d_lights_buffer";
	/*static*/ const std::string point_light_buffer_name = "p_lights_buffer";
	/*static*/ const std::string spot_light_buffer_name = "s_lights_buffer";

	uint32_t directional_light_max_count = 32;
	uint32_t point_light_max_count = 32;
	uint32_t spot_light_max_count = 32;

	std::shared_ptr<UniformBuffer> directional_lights_buffer;
	std::shared_ptr<UniformBuffer> point_lights_buffer;
	std::shared_ptr<UniformBuffer> spot_lights_buffer;
};
