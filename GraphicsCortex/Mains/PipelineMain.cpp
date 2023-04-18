#include "GraphicsCortex.h"


int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);

	DirectionalLight_s sunlight(glm::vec3(4, 2, 4), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1, 1, 1));

	AmbiantLight_s ambiance(glm::vec3(0.2, 0.2, 0.2));

	UnorderedMaterial_s bricks_material(2);
	bricks_material->set_texture("Images/Bricks/brickcolor.jpg", 4, 0, UnorderedMaterial::COLOR);
	bricks_material->set_texture("Images/Bricks/bricknormal.png", 4, 1, UnorderedMaterial::NORMAL);

	UnorderedMaterial_s stone_tile_material(2);
	stone_tile_material->set_texture("Images/StoneTiles/tiles_color.jpg", 4, 0, UnorderedMaterial::COLOR);
	stone_tile_material->set_texture("Images/StoneTiles/tiles_normal.jpg", 4, 1, UnorderedMaterial::NORMAL);

	RenderPipeline pipeline = default_program::default_pipeline(frame);
	pipeline.cameras["camera"] = scene.camera;
	pipeline.activate_camera("camera");

	Graphic_s box(default_geometry::cube());
	box->load_material(bricks_material);

	Graphic_s plane(default_geometry::cube(glm::vec3(20, 0.1, 20)));
	plane->load_material(stone_tile_material);
	plane->set_position(glm::vec3(0, -2, 0));

	pipeline.graphics["box"] = box;
	pipeline.graphics["plane"] = plane;
	pipeline.ambiant_ligths["ambiance"] = ambiance;
	pipeline.directional_ligths["sun"] = sunlight;

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.display_performance();
		scene.camera->handle_movements(frame.window, frame_time);

		box->set_rotation(box->get_rotation() * glm::quat(glm::vec3(0, 0.0005 * frame_time, 0)));
		
		pipeline.render_sequence(frame);
	}
}