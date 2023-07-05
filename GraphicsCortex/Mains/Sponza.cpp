#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	RenderPipeline pipeline = default_program::default_pipeline(frame);

	pipeline.programs["solid_multitexture"] = default_program::solid_program_multitexture_s();

	pipeline.cameras["camera"] = scene.camera;
	pipeline.activate_camera("camera");

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

			pipeline->activate_program("solid_multitexture");
			pipeline->render_single_graphic("sponza");

			pipeline->render();
		});

	Mesh_s sponza_mesh(Model("Models/Sponza/Sponza.fbx", 0.1f, Model::ALL));
	UnorderedMaterial_s sponza_material("Models/Sponza/Sponza.fbx");
	Graphic_s sponza;
	sponza->load_model(sponza_mesh);
	sponza->load_material(sponza_material);
	pipeline.graphics["sponza"] = sponza;
	pipeline.deattach_graphic("sponza");

	DirectionalLight_s sun(glm::vec3(0, 0, 0), glm::vec3(0, -1, 1), glm::vec3(0.7, 0.7, 0.7));
	AmbiantLight_s ambiance(glm::vec3(0.2, 0.2, 0.2));
	pipeline.directional_ligths["sun"] = sun;
	pipeline.ambiant_ligths["ambiance"] = ambiance;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.display_performance();

		scene.camera->handle_movements(frame.window, deltatime);

		pipeline.render_sequence(frame);
	}
}