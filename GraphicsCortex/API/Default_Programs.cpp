#include "Default_Programs.h"

namespace default_program {

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

	std::shared_ptr<Program> solid_program_s() {
		Shader default_shader("Shaders/Solid.vert", "Shaders/Solid.geom", "Shaders/Solid.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> solid_program_multitexture_s() {
		Shader default_shader("Shaders/Solid_multitexture.vert", "Shaders/Solid_multitexture.geom", "Shaders/Solid_multitexture.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> flatcolor_program_s() {
		Shader default_shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> framebuffer_program_s() {
		Shader default_shader("Shaders/FrameBuffer.vert", "Shaders/FrameBuffer.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> cubemap_program_s() {
		Shader default_shader("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> gui_program_s() {
		Shader default_shader("Shaders/Gui.vert", "Shaders/Gui.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> text_program_s() {
		Shader default_shader("Shaders/Text.vert", "Shaders/Text.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}
	std::shared_ptr<Program> depth_program_s() {
		Shader default_shader("Shaders/Depth.vert", "Shaders/Depth.frag");
		std::shared_ptr<Program> renderer = std::make_shared<Program>(default_shader);
		return renderer;
	}

	RenderPipeline default_pipeline(Frame& frame) {
		RenderPipeline pipeline;
		pipeline.programs["solid"] = solid_program_s();
		pipeline.programs["depth"] = depth_program_s();
		pipeline.programs["framebuffer"] = framebuffer_program_s();

		std::shared_ptr<FrameBuffer> shadowmap = std::make_shared<FrameBuffer>(frame.window_width * 8, frame.window_height * 8);

		pipeline.framebuffers["shadowmap"] = shadowmap;

		pipeline.graphic_uniforms["solid"] = [](UniformFunction_Graphic) {
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

		pipeline.ambiantlight_uniforms["solid"] = [](UniformFunction_AmbiantLight) {
			program->update_uniform("a_lights[" + std::to_string(object_index) + "].color", ambiant_light->color);
			program->update_uniform("a_lights_count", object_index + 1);
		};

		pipeline.directionallight_uniforms["solid"] = [](UniformFunction_DirectionalLight) {
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].color", directional_light->color);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].direction", directional_light->direction);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].view_matrix", directional_light->light_view_matrix);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].projection_matrix", directional_light->light_projection_matrix);
			program->update_uniform("d_lights_count", object_index + 1);
		};

		pipeline.graphic_uniforms["shadowmap"] = [](UniformFunction_Graphic) {
			program->update_uniform("model", graphic->model_matrix);
		};
		pipeline.directionallight_uniforms["shadowmap"] = [](UniformFunction_DirectionalLight) {
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

		std::shared_ptr<FrameBuffer> shadowmap = std::make_shared<FrameBuffer>(frame.window_width * 8, frame.window_height * 8);

		pipeline.framebuffers["shadowmap"] = shadowmap;

		pipeline.graphic_uniforms["solid"] = [](UniformFunction_Graphic) {
			program->update_uniform("model", graphic->model_matrix);
			program->update_uniform("view", camera->view_matrix);
			program->update_uniform("projection", camera->projection_matrix);
			program->update_uniform("cube_map", 13);
			program->update_uniform("use_cube_map_reflection", (int)(graphic->get_reflection_strength() > 0.001));
			program->update_uniform("cube_map_reflection_strength", graphic->get_reflection_strength());
			program->update_uniform("camera_coords", camera->position);
			program->update_uniform("shadow_map", 2);
		};

		pipeline.ambiantlight_uniforms["solid"] = [](UniformFunction_AmbiantLight) {
			program->update_uniform("a_lights[" + std::to_string(object_index) + "].color", ambiant_light->color);
			program->update_uniform("a_lights_count", object_index + 1);
		};

		pipeline.directionallight_uniforms["solid"] = [](UniformFunction_DirectionalLight) {
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].color", directional_light->color);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].direction", directional_light->direction);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].view_matrix", directional_light->light_view_matrix);
			program->update_uniform("d_lights[" + std::to_string(object_index) + "].projection_matrix", directional_light->light_projection_matrix);
			program->update_uniform("d_lights_count", object_index + 1);
		};

		pipeline.graphic_uniforms["shadowmap"] = [](UniformFunction_Graphic) {
			program->update_uniform("model", graphic->model_matrix);
		};
		pipeline.directionallight_uniforms["shadowmap"] = [](UniformFunction_DirectionalLight) {
			program->update_uniform("view", directional_light->light_view_matrix);
			program->update_uniform("projection", directional_light->light_projection_matrix);
		};

		pipeline.set_rendering_sequence([](RenderPipeline* pipeline_r, Frame& frame) {
			RenderPipeline_MultiTextured* pipeline = (RenderPipeline_MultiTextured*)pipeline_r;

			pipeline->reset_active_objects();

			for (auto& d_light_pair : pipeline->directional_ligths) {
				std::shared_ptr<DirectionalLight>& d_light = d_light_pair.second;
				//d_light->position = pipeline->cameras[pipeline->get_active_camera_name()]->position;
				//d_light->update_matricies();
			}
			
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

			pipeline->activate_program("solid");
			pipeline->activate_uniforms_directional_light("solid");
			pipeline->activate_uniforms_ambiant_light("solid");
			pipeline->activate_uniforms_graphic("solid");
			pipeline->render();

			pipeline->activate_program("solid_multitexture");
			pipeline->activate_uniforms_graphic("solid");
			pipeline->render_multitextured();
			});

		return pipeline;
	}
}