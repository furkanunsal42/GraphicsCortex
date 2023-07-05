#include "GraphicsCortex.h"

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	scene.camera->screen_width = 1920;
	scene.camera->screen_height = 1080;
	scene.camera->max_distance = 100.0f;

	Program_s solid_program = default_program::solid_program_s();
	Program_s depth_program(Shader("Shaders/Depth.vert", "Shaders/Depth.frag"));

	DirectionalLight_s sunlight(glm::vec3(4, 2, 4), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1, 1, 1), solid_program);
	sunlight->update_matricies();
	sunlight->set_uniform_upadte_queue(default_program::directional_light_default_uniform_queue(*sunlight.obj, 0));
	scene.add_light(sunlight);

	AmbiantLight_s ambiance(glm::vec3(0.2, 0.2, 0.2), solid_program);
	ambiance->set_uniform_upadte_queue(default_program::ambiant_light_default_uniform_queue(*ambiance.obj, 0));
	scene.add_light(ambiance);

	UnorderedMaterial_s bricks_material(2);
	bricks_material->set_texture("Images/Bricks/brickcolor.jpg", 4, 0, UnorderedMaterial::COLOR);
	bricks_material->set_texture("Images/Bricks/bricknormal.png", 4, 1, UnorderedMaterial::NORMAL);

	UnorderedMaterial_s stone_tile_material(2);
	stone_tile_material->set_texture("Images/StoneTiles/tiles_color.jpg", 4, 0, UnorderedMaterial::COLOR);
	stone_tile_material->set_texture("Images/StoneTiles/tiles_normal.jpg", 4, 1, UnorderedMaterial::NORMAL);

	Graphic_s box(default_geometry::cube());
	box->load_material(bricks_material);
	auto box_solid_uniforms = default_program::solid_default_uniform_queue(*scene.camera.obj, box);
	box_solid_uniforms.add_uniform_update(uniform_update<int>("shadow_map", 2));

	uniform_update_queue box_shadowmap_uniforms;
	box_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &box->model_matrix));
	box_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &sunlight->light_view_matrix));
	box_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &sunlight->light_projection_matrix));
	box->load_program(depth_program);
	box->set_uniform_all(box_shadowmap_uniforms);
	scene.add_graphic(box);

	Graphic_s plane(default_geometry::cube(glm::vec3(20, 0.1, 20)));
	plane->load_material(stone_tile_material);
	auto plane_solid_uniforms = default_program::solid_default_uniform_queue(*scene.camera.obj, plane);
	plane_solid_uniforms.add_uniform_update(uniform_update<int>("shadow_map", 2));

	uniform_update_queue plane_shadowmap_uniforms;
	plane_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &plane->model_matrix));
	plane_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &sunlight->light_view_matrix));
	plane_shadowmap_uniforms.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &sunlight->light_projection_matrix));
	plane->load_program(depth_program);
	plane->set_uniform_all(plane_shadowmap_uniforms);
	plane->set_position(glm::vec3(0, -2, 0));
	scene.add_graphic(plane);

	Program_s framebuffer_program = default_program::framebuffer_program_s();

	FrameBuffer_s shadowmap(1920, 1080, 0, false);
	shadowmap->load_program(framebuffer_program);
	shadowmap->texture_slot = 2;


	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.display_performance();
		scene.camera->handle_movements(frame.window, frame_time);

		box->set_rotation(box->get_rotation() * glm::quat(glm::vec3(0, 0.0005 * frame_time, 0)));
		//plane->set_rotation(plane->get_rotation() * glm::quat(glm::vec3(0, -0.0002 * frame_time, 0)));

		shadowmap->bind();
		
		frame.clear_window(0.2, 0.2, 0.2);
		box->load_program(depth_program);
		box->set_uniform_all(box_shadowmap_uniforms);
		plane->load_program(depth_program);
		plane->set_uniform_all(plane_shadowmap_uniforms);
		scene.render(false);
				
		shadowmap->unbind();

		shadowmap->color_texture.bind();
		
		frame.clear_window(1, 1, 1, 1);
		box->load_program(solid_program);
		box->set_uniform_all(box_solid_uniforms);
		plane->load_program(solid_program);
		plane->set_uniform_all(plane_solid_uniforms);
		scene.render(false);
	
		//shadowmap->save().save_to_disc("shadowmap_image.png");
		
	}


}