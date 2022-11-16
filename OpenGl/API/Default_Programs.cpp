#include "Default_Programs.h"


namespace default_program {
	Program solid_program() {
		Shader default_shader("Shaders/Solid.vert", "Shaders/Solid.geom", "Shaders/Solid.frag");
		Program solid_program(default_shader.vertex_shader, default_shader.geometry_shader, default_shader.fragment_shader);
		return solid_program;
	}

	uniform_update_queue solid_default_uniform_queue(Scene& scene, Graphic& mesh) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &mesh.model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &scene.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &scene.camera->projection_matrix));
		queue.add_uniform_update(uniform_update<int>("cube_map", 13));
		queue.add_uniform_update(uniform_update<int>("use_cube_map_reflection", 0));
		queue.add_uniform_update(uniform_update<float>("cube_map_reflection_strength", 0.85));
		queue.add_uniform_update(dynamic_uniform_update<float>("camera_coords", &scene.camera->position.x, &scene.camera->position.y, &scene.camera->position.z));
		queue.add_uniform_update(uniform_update<int>("use_color_map", (int)(mesh.material->_enable_color_map)));
		queue.add_uniform_update(uniform_update<int>("use_specular_map", (int)(mesh.material->_enable_specular_map)));
		queue.add_uniform_update(uniform_update<int>("use_normal_map", (int)(mesh.material->_enable_normal_map)));
		queue.add_uniform_update(dynamic_uniform_update<int>("color_map_slot", &mesh.material->color_map_slot));
		queue.add_uniform_update(dynamic_uniform_update<int>("specular_map_slot", &mesh.material->specular_map_slot));
		queue.add_uniform_update(dynamic_uniform_update<int>("normal_map_slot", &mesh.material->normal_map_slot));
		return queue;
	}

	uniform_update_queue flat_default_uniform_queue(Scene& scene, Graphic& mesh) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &mesh.model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &scene.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &scene.camera->projection_matrix));

		queue.add_uniform_update(uniform_update<float>("color", 0.8f, 0.7f, 0.6f, 1.0f));
		return queue;
	}

	uniform_update_queue basic_uniform_queue(Scene& scene, Graphic& mesh) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &mesh.model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &scene.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &scene.camera->projection_matrix));
		return queue;
	}

	uniform_update_queue cubemap_default_uniform_queue(CubeMapTexture& cubemap) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &cubemap.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &cubemap.camera->projection_matrix));
		queue.add_uniform_update(dynamic_uniform_update<int>("cubemap", &cubemap.texture_slot));
		return queue;
	}

	uniform_update_queue ambiant_light_default_uniform_queue(AmbiantLight& ambiant_light, int light_ingdex) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("a_lights[" + std::to_string(light_ingdex) + "].color", &ambiant_light.color.x, &ambiant_light.color.y, &ambiant_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<int>("a_lights_count", &AmbiantLight::count));

		return queue;

	}
	uniform_update_queue directional_light_default_uniform_queue(DirectionalLight& directional_light, int light_ingdex) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[" + std::to_string(light_ingdex) + "].color", &directional_light.color.x, &directional_light.color.y, &directional_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[" + std::to_string(light_ingdex) + "].direction", &directional_light.direction.x, &directional_light.direction.y, &directional_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<int>("d_lights_count", &DirectionalLight::count));

		return queue;
	}
	uniform_update_queue point_light_default_uniform_queue(PointLight& point_light, int light_ingdex) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_ingdex) + "].position", &point_light.position.x, &point_light.position.y, &point_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_ingdex) + "].color", &point_light.color.x, &point_light.color.y, &point_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_ingdex) + "].constant_term", &point_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_ingdex) + "].linear_term", &point_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_ingdex) + "].exponential_term", &point_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<int>("p_lights_count", &PointLight::count));

		return queue;
	}
	uniform_update_queue spot_light_default_uniform_queue(SpotLight& spot_light, int light_ingdex) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].position", &spot_light.position.x, &spot_light.position.y, &spot_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].direction", &spot_light.direction.x, &spot_light.direction.y, &spot_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].color", &spot_light.color.x, &spot_light.color.y, &spot_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].constant_term", &spot_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].linear_term", &spot_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].exponential_term", &spot_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_ingdex) + "].cos_angle", &spot_light.cos_angle));
		queue.add_uniform_update(dynamic_uniform_update<int>("s_lights_count", &SpotLight::count));

		return queue;
	}

	Program flatcolor_program() {
		Shader default_shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");
		Program flatcolor(default_shader.vertex_shader, default_shader.fragment_shader);
		return flatcolor;
	}

	Program framebuffer_program() {
		Shader default_shader("Shaders/FrameBuffer.vert", "Shaders/FrameBuffer.frag");
		Program framebuffer(default_shader.vertex_shader, default_shader.fragment_shader);
		return framebuffer;
	}
	Program cubemap_program() {
		Shader default_shader("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
		Program cubemap(default_shader.vertex_shader, default_shader.fragment_shader);
		return cubemap;
	}

}