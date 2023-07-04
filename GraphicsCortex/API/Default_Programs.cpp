#include "Default_Programs.h"


namespace default_program {

	uniform_update_queue solid_default_uniform_queue(Camera& camera, Graphic_s graphic) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &graphic->model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &camera.view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &camera.projection_matrix));
		queue.add_uniform_update(uniform_update<int>("cube_map", 13));
		queue.add_uniform_update(uniform_update<int>("use_cube_map_reflection", 0));
		queue.add_uniform_update(uniform_update<float>("cube_map_reflection_strength", 0.85));
		queue.add_uniform_update(dynamic_uniform_update<float>("camera_coords", &camera.position.x, &camera.position.y, &camera.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("active_texture_indicies", &graphic->unordered_material->_active_textures_by_type.x, &graphic->unordered_material->_active_textures_by_type.y, &graphic->unordered_material->_active_textures_by_type.z));

		//mesh.model._model_texture_table.update_uniform_array();
		//int array_size;
		//int* arr = mesh.model._model_texture_table.get_uniform_array(&array_size);
		//std::cout << array_size << std::endl;
		//for (int i = 0; i < array_size; i++)
		//	std::cout << (int)*(arr + i) << std::endl;
		
		//unsigned int model_texture_table_uniform_array[65536];
		//for (int i = 0; i < 65536; i++) {
		//	model_texture_table_uniform_array[i] = mesh.renderer->define_get_uniform_id("model_texture_table_array[" + std::to_string(i) + "]");
		//}
		//queue.add_uniform_update([&mesh, model_texture_table_uniform_array]() {
		//	int array_size;
		//	int* a = mesh.model._model_texture_table.get_uniform_array(&array_size);
		//	for (int i = 0; i < array_size; i++) {
		//		mesh.renderer->update_uniform(model_texture_table_uniform_array[i], a[i]);
		//	}
		//	});


		//queue.add_uniform_update([mesh, uniform_id = mesh.renderer->define_get_uniform_id("use_color_map")]()		{ if (!mesh.use_unordered_material) mesh.renderer->update_uniform(uniform_id, (int)mesh.material->_enable_color_map);		else mesh.renderer->update_uniform(uniform_id, 1);  });
		//queue.add_uniform_update([mesh, uniform_id = mesh.renderer->define_get_uniform_id("use_specular_map")]()	{ if (!mesh.use_unordered_material) mesh.renderer->update_uniform(uniform_id, (int)mesh.material->_enable_specular_map);	else mesh.renderer->update_uniform(uniform_id, 0);  });
		//queue.add_uniform_update([mesh, uniform_id = mesh.renderer->define_get_uniform_id("use_normal_map")]()		{ if (!mesh.use_unordered_material) mesh.renderer->update_uniform(uniform_id, (int)mesh.material->_enable_normal_map);		else mesh.renderer->update_uniform(uniform_id, 0);  });
		
		
		return queue;
	}

	uniform_update_queue flat_default_uniform_queue(Camera& camera, Graphic_s graphic) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &graphic->model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &camera.view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &camera.projection_matrix));

		queue.add_uniform_update(uniform_update<float>("color", 0.8f, 0.7f, 0.6f, 1.0f));
		return queue;
	}
	
	uniform_update_queue basic_uniform_queue(Camera& camera, Graphic_s graphic) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &graphic->model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &camera.view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &camera.projection_matrix));

		return queue;
	}

	uniform_update_queue cubemap_default_uniform_queue(CubeMapTexture& cubemap) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &cubemap.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &cubemap.camera->projection_matrix));
		queue.add_uniform_update(dynamic_uniform_update<int>("cubemap", &cubemap.texture_slot));
		return queue;
	}

	uniform_update_queue text_uniform_queue(Camera& camera, Text& text) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &text.graphic->model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &camera.view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &camera.projection_matrix));
		queue.add_uniform_update(dynamic_uniform_update<int>("texture_slot", (int*)&text._font->_font_atlas.texture_slot));
		queue.add_uniform_update(dynamic_uniform_update<float>("screen_resolution", &camera.screen_width, &camera.screen_height));
		queue.add_uniform_update(dynamic_uniform_update<float>("text_color", &text._color.x, &text._color.y, &text._color.z, &text._color.w));
		return queue;
	}


	uniform_update_queue ambiant_light_default_uniform_queue(AmbiantLight& ambiant_light, int light_index) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("a_lights[" + std::to_string(light_index) + "].color", &ambiant_light.color.x, &ambiant_light.color.y, &ambiant_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<int>("a_lights_count", &AmbiantLight::count));

		return queue;

	}
	uniform_update_queue directional_light_default_uniform_queue(DirectionalLight& directional_light, int light_index) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[" + std::to_string(light_index) + "].color", &directional_light.color.x, &directional_light.color.y, &directional_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[" + std::to_string(light_index) + "].direction", &directional_light.direction.x, &directional_light.direction.y, &directional_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("d_lights[" + std::to_string(light_index) + "].view_matrix", &directional_light.light_view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("d_lights[" + std::to_string(light_index) + "].projection_matrix", &directional_light.light_projection_matrix));
		queue.add_uniform_update(dynamic_uniform_update<int>("d_lights_count", &DirectionalLight::count));

		return queue;
	}
	uniform_update_queue point_light_default_uniform_queue(PointLight& point_light, int light_index) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_index) + "].position", &point_light.position.x, &point_light.position.y, &point_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_index) + "].color", &point_light.color.x, &point_light.color.y, &point_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_index) + "].constant_term", &point_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_index) + "].linear_term", &point_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[" + std::to_string(light_index) + "].exponential_term", &point_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<int>("p_lights_count", &PointLight::count));

		return queue;
	}
	uniform_update_queue spot_light_default_uniform_queue(SpotLight& spot_light, int light_index) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].position", &spot_light.position.x, &spot_light.position.y, &spot_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].direction", &spot_light.direction.x, &spot_light.direction.y, &spot_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].color", &spot_light.color.x, &spot_light.color.y, &spot_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].constant_term", &spot_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].linear_term", &spot_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].exponential_term", &spot_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[" + std::to_string(light_index) + "].cos_angle", &spot_light.cos_angle));
		queue.add_uniform_update(dynamic_uniform_update<int>("s_lights_count", &SpotLight::count));

		return queue;
	}

	Program solid_program() {
		Shader default_shader("Shaders/Solid.vert", "Shaders/Solid.geom", "Shaders/Solid.frag");
		Program solid_program(default_shader);
		return solid_program;
	}
	Program solid_program_multitexture() {
		Shader default_shader("Shaders/Solid_multitexture.vert", "Shaders/Solid_multitexture.geom", "Shaders/Solid_multitexture.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program flatcolor_program() {
		Shader default_shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program framebuffer_program() {
		Shader default_shader("Shaders/FrameBuffer.vert", "Shaders/FrameBuffer.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program cubemap_program() {
		Shader default_shader("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program gui_program() {
		Shader default_shader("Shaders/Gui.vert", "Shaders/Gui.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program text_program() {
		Shader default_shader("Shaders/Text.vert", "Shaders/Text.frag");
		Program renderer(default_shader);
		return renderer;
	}
	Program depth_program() {
		Shader default_shader("Shaders/Depth.vert", "Shaders/Depth.frag");
		Program renderer(default_shader);
		return renderer;
	}

	Program_s solid_program_s() {
		Shader default_shader("Shaders/Solid.vert", "Shaders/Solid.geom", "Shaders/Solid.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s solid_program_multitexture_s() {
		Shader default_shader("Shaders/Solid_multitexture.vert", "Shaders/Solid_multitexture.geom", "Shaders/Solid_multitexture.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s flatcolor_program_s() {
		Shader default_shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s framebuffer_program_s() {
		Shader default_shader("Shaders/FrameBuffer.vert", "Shaders/FrameBuffer.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s cubemap_program_s() {
		Shader default_shader("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s gui_program_s() {
		Shader default_shader("Shaders/Gui.vert", "Shaders/Gui.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s text_program_s() {
		Shader default_shader("Shaders/Text.vert", "Shaders/Text.frag");
		Program_s renderer(default_shader);
		return renderer;
	}
	Program_s depth_program_s() {
		Shader default_shader("Shaders/Depth.vert", "Shaders/Depth.frag");
		Program_s renderer(default_shader);
		return renderer;
	}

	RenderPipeline default_pipeline(Frame& frame) {
		RenderPipeline pipeline;
		pipeline.programs["solid"] = solid_program_s();
		pipeline.programs["depth"] = depth_program_s();
		pipeline.programs["framebuffer"] = framebuffer_program_s();

		FrameBuffer_s shadowmap(frame.window_width * 8, frame.window_height * 8);

		pipeline.framebuffers["shadowmap"] = shadowmap;

		pipeline.graphic_uniforms["solid"] = [](UniformFunction_Graphic_s) {
			program->update_uniform("model", graphic->model_matrix);
			program->update_uniform("view", camera->view_matrix);
			program->update_uniform("projection", camera->projection_matrix);
			program->update_uniform("cube_map", 13);
			program->update_uniform("use_cube_map_reflection", 0);
			program->update_uniform("cube_map_reflection_strength", 0.85f);
			program->update_uniform("camera_coords", camera->position);
			program->update_uniform("active_texture_indicies", graphic->unordered_material->get_active_textures_by_type());
			program->update_uniform("shadow_map", 2);
		};

		pipeline.ambiantlight_uniforms["solid"] = [](UniformFunction_AmbiantLight_s) {
			program->update_uniform("a_lights[" + std::to_string(object_index) + "].color", ambiant_light->color);
			program->update_uniform("a_lights_count", object_index + 1);
		};

		pipeline.directionallight_uniforms["solid"] = [](UniformFunction_DirectionalLight_s) {
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].color", directional_light->color);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].direction", directional_light->direction);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].view_matrix", directional_light->light_view_matrix);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].projection_matrix", directional_light->light_projection_matrix);
			program->update_uniform("d_lights_count", object_index + 1);
		};

		pipeline.graphic_uniforms["shadowmap"] = [](UniformFunction_Graphic_s) {
			program->update_uniform("model", graphic->model_matrix);
		};
		pipeline.directionallight_uniforms["shadowmap"] = [](UniformFunction_DirectionalLight_s) {
			program->update_uniform("view", directional_light->light_view_matrix);
			program->update_uniform("projection", directional_light->light_projection_matrix);
		};

		pipeline.set_rendering_sequence([](RenderPipeline* pipeline, Frame& frame) {
			pipeline->reset_active_objects();

			pipeline->framebuffers["shadowmap"]->bind();
			pipeline->activate_program("depth");
			pipeline->activate_uniforms_graphic("shadowmap");
			pipeline->activate_uniforms_directional_light("shadowmap");
			frame.clear_window(0.2, 0.2, 0.2);

			pipeline->render();

			pipeline->framebuffers["shadowmap"]->unbind();
			
			pipeline->framebuffers["shadowmap"]->color_texture.texture_slot = 2;
			pipeline->framebuffers["shadowmap"]->color_texture.bind();
			
			frame.clear_window(1, 1, 1, 1);
			pipeline->activate_program("solid");
			pipeline->activate_uniforms_graphic("solid");
			pipeline->activate_uniforms_directional_light("solid");
			pipeline->activate_uniforms_ambiant_light("solid");
			
			pipeline->render();
		});

		return pipeline;
	}

	RenderPipeline_MultiTextured multitextured_pipeline(Frame& frame) {
		RenderPipeline_MultiTextured pipeline;
		pipeline.programs["solid"] = solid_program_s();
		pipeline.programs["solid_multitexture"] = solid_program_multitexture_s();
		pipeline.programs["depth"] = depth_program_s();
		pipeline.programs["framebuffer"] = framebuffer_program_s();

		FrameBuffer_s shadowmap(frame.window_width, frame.window_height);

		pipeline.framebuffers["shadowmap"] = shadowmap;

		pipeline.graphic_uniforms["solid"] = [](UniformFunction_Graphic_s) {
			program->update_uniform("model", graphic->model_matrix);
			program->update_uniform("view", camera->view_matrix);
			program->update_uniform("projection", camera->projection_matrix);
			program->update_uniform("cube_map", 13);
			program->update_uniform("use_cube_map_reflection", (int)(graphic->cubemap_reflections_strength > 0.1f));
			program->update_uniform("cube_map_reflection_strength", graphic->cubemap_reflections_strength);
			program->update_uniform("camera_coords", camera->position);
			program->update_uniform("active_texture_indicies", graphic->unordered_material->get_active_textures_by_type());
			program->update_uniform("shadow_map", 2);
		};

		pipeline.ambiantlight_uniforms["solid"] = [](UniformFunction_AmbiantLight_s) {
			program->update_uniform("a_lights[" + std::to_string(object_index) + "].color", ambiant_light->color);
			program->update_uniform("a_lights_count", object_index + 1);
		};

		pipeline.directionallight_uniforms["solid"] = [](UniformFunction_DirectionalLight_s) {
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].color", directional_light->color);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].direction", directional_light->direction);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].view_matrix", directional_light->light_view_matrix);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].projection_matrix", directional_light->light_projection_matrix);
			program->update_uniform("d_lights_count", object_index + 1);
		};

		pipeline.graphic_uniforms["shadowmap"] = [](UniformFunction_Graphic_s) {
			program->update_uniform("model", graphic->model_matrix);
		};
		pipeline.directionallight_uniforms["shadowmap"] = [](UniformFunction_DirectionalLight_s) {
			program->update_uniform("view", directional_light->light_view_matrix);
			program->update_uniform("projection", directional_light->light_projection_matrix);
		};

		pipeline.set_rendering_sequence([](RenderPipeline* pipeline_r, Frame& frame) {
			RenderPipeline_MultiTextured* pipeline = (RenderPipeline_MultiTextured*)pipeline_r;

			pipeline->reset_active_objects();

			pipeline->framebuffers["shadowmap"]->bind();
			glViewport(0, 0, frame.window_width * 8, frame.window_height * 8);

			pipeline->activate_program("depth");
			pipeline->activate_uniforms_graphic("shadowmap");
			pipeline->activate_uniforms_directional_light("shadowmap");
			frame.clear_window(0.2, 0.2, 0.2);

			pipeline->render();
			pipeline->render_multitextured();

			pipeline->framebuffers["shadowmap"]->unbind();
			glViewport(0, 0, frame.window_width, frame.window_height);


			pipeline->framebuffers["shadowmap"]->color_texture.texture_slot = 2;
			pipeline->framebuffers["shadowmap"]->color_texture.bind();

			frame.clear_window(1, 1, 1, 1);
			pipeline->activate_program("solid");
			pipeline->activate_uniforms_directional_light("solid");
			pipeline->activate_uniforms_ambiant_light("solid");
			pipeline->activate_program("solid_multitexture");
			pipeline->activate_uniforms_graphic("solid");
			pipeline->render_multitextured();

			pipeline->activate_program("solid");
			pipeline->activate_uniforms_graphic("solid");
			pipeline->render();
			});

		return pipeline;
	}
}