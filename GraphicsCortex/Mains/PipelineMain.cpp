#include "GraphicsCortex.h"

//#define UniformFunction_Graphic_s (Graphic_s graphic, Program_s program, std::reference_wrapper<Camera> camera)
//#define UniformFunction_DirectionalLight_s (DirectionalLight_s directional_light, Program_s program, std::reference_wrapper<Camera> camera)
//#define UniformFunction_AmbiantLight_s (AmbiantLight_s ambiant_light, Program_s program, std::reference_wrapper<Camera> camera)
//#define UniformFunction_PointLight_s (PointLight_s point_light, Program_s program, std::reference_wrapper<Camera> camera)
//#define UniformFunction_SpotLight_s (SpotLight_s spot_light, Program_s program, std::reference_wrapper<Camera> camera)

template<typename T>
class UniformUpdater {
public:

	UniformUpdater(std::function<void(T, Program_s, Camera_s)> update_call = [](T, Program_s, Camera_s) {}) {
		_uniform_update_call = update_call;
	}

	void operator()(T object, Program_s program, Camera_s camera) {
		_uniform_update_call(object, program, camera);
	}

	void operator()(const std::vector<T>& objects, Program_s program, Camera_s camera) {
		for (auto& object : objects)
			_uniform_update_call(object, program, camera);
	}

	void set_uniform_call(std::function<void(T, Program_s, Camera_s)> update_call) {
		_uniform_update_call = update_call;
	}

	void operator=(std::function<void(T, Program_s, Camera_s)> update_call) {
		set_uniform_call(update_call);
	}

private:
	std::function<void(T, Program_s, Camera_s)> _uniform_update_call;

};

class RenderPipeline {
public:
	std::unordered_map<std::string, FrameBuffer_s> framebuffers;
	std::unordered_map<std::string, Graphic_s> graphics;
	std::unordered_map<std::string, Program_s> programs;
	std::unordered_map<std::string, AmbiantLight_s> ambiant_ligths;
	std::unordered_map<std::string, DirectionalLight_s> directional_ligths;
	std::unordered_map<std::string, PointLight_s> point_ligths;
	std::unordered_map<std::string, SpotLight_s> spot_ligths;
	std::unordered_map<std::string, Camera_s> cameras;

	std::unordered_map<std::string, UniformUpdater<Graphic_s>> graphic_uniforms;
	std::unordered_map<std::string, UniformUpdater<AmbiantLight_s>> ambiantlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<DirectionalLight_s>> directionallight_uniforms;
	std::unordered_map<std::string, UniformUpdater<PointLight_s>> pointlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<SpotLight_s>> spotlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<FrameBuffer_s>> framebuffer_uniforms;
	
	// uniform updating
	void activate_uniforms_graphic(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_graphic = uniform_updater_name;
	}
	void activate_uniforms_ambiant_light(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_ambiant_light = uniform_updater_name;
	}
	void activate_uniforms_directional_light(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_directional_light = uniform_updater_name;
	}
	void activate_uniforms_point_light(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_point_light = uniform_updater_name;
	}
	void activate_uniforms_spot_light(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_spot_light = uniform_updater_name;
	}
	void activate_uniforms_framebuffer(const std::string& uniform_updater_name) {
		_active_uniform_updater_name_framebuffer = uniform_updater_name;
	}

	// object activation

	void activate_program(const std::string& program_name) {
		_active_name_program = program_name;
	}
	void activate_framebuffer(const std::string& framebuffer_name) {
		_active_name_framebuffer = framebuffer_name;
	}
	void activate_camera(const std::string& camera_name) {
		_active_name_camera = camera_name;
	}

	void render() {

		auto camera_iterator = cameras.find(_active_name_camera);
		auto program_iterator = programs.find(_active_name_program);

		if (camera_iterator == cameras.end()) {
			std::cout << "[Graphics Pipline Error] Active Camera Name Not Found" << std::endl;
			ASSERT(false);
		}
		
		if (program_iterator == programs.end()) {
			std::cout << "[Graphics Pipline Error] Active Camera Name Not Found" << std::endl;
			ASSERT(false);
		}

		Camera_s active_camera = (*camera_iterator).second;
		Program_s active_program = (*program_iterator).second;

		// update all uniforms

		for (auto& name_ambiant_pair : ambiant_ligths)
			ambiantlight_uniforms[_active_uniform_updater_name_ambiant_light](name_ambiant_pair.second, active_program, active_camera);
		for (auto& name_directional_pair : directional_ligths) {
			name_directional_pair.second->update_matricies();
			directionallight_uniforms[_active_uniform_updater_name_directional_light](name_directional_pair.second, active_program, active_camera);
		}
		for (auto& name_spot_pair : spot_ligths)
			spotlight_uniforms[_active_uniform_updater_name_spot_light](name_spot_pair.second, active_program, active_camera);
		for (auto& name_point_pair : point_ligths)
			pointlight_uniforms[_active_uniform_updater_name_point_light](name_point_pair.second, active_program, active_camera);
		for (auto& name_framebuffer_pair : framebuffers)
			framebuffer_uniforms[_active_uniform_updater_name_framebuffer](name_framebuffer_pair.second, active_program, active_camera);

		for (auto& name_graphic_pair : graphics){
			Graphic_s graphic = name_graphic_pair.second;
			graphic->update_matrix();
			graphic_uniforms[_active_uniform_updater_name_graphic](graphic, active_program, active_camera);
			graphic->draw(false);
		}
	}

private:

	// uniform updating
	std::string _active_uniform_updater_name_graphic;
	std::string _active_uniform_updater_name_ambiant_light;
	std::string _active_uniform_updater_name_directional_light;
	std::string _active_uniform_updater_name_point_light;
	std::string _active_uniform_updater_name_spot_light;
	std::string _active_uniform_updater_name_framebuffer;


	// object activation

	std::string _active_name_program;
	std::string _active_name_framebuffer;
	std::string _active_name_camera;
};

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, true, false);
	Scene scene(frame);
	scene.camera->screen_width = 1920;
	scene.camera->screen_height = 1080;
	scene.camera->max_distance = 100.0f;

	Program_s solid_program = default_program::solid_program_s();
	Program_s depth_program(Shader("Custom Shaders/Depth.vert", "Custom Shaders/Depth.frag"));
	Program_s framebuffer_program = default_program::framebuffer_program_s();

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

	RenderPipeline pipeline;

	Graphic_s box(default_geometry::cube());
	box->load_material(bricks_material);

	Graphic_s plane(default_geometry::cube(glm::vec3(20, 0.1, 20)));
	plane->load_material(stone_tile_material);
	plane->set_position(glm::vec3(0, -2, 0));

	pipeline.graphics["box"] = box;
	pipeline.graphics["plane"] = plane;
	pipeline.ambiant_ligths["ambiance"] = ambiance;
	pipeline.directional_ligths["sun"] = sunlight;

	pipeline.programs["solid"] = solid_program;
	pipeline.programs["depth"] = depth_program;
	pipeline.programs["framebuffer"] = framebuffer_program;

	pipeline.graphic_uniforms["solid"] = [](Graphic_s graphic, Program_s program, Camera_s camera) {
		program->update_uniform("model", graphic->model_matrix);
		program->update_uniform("view", camera->view_matrix);
		program->update_uniform("projection", camera->projection_matrix);
		program->update_uniform("cube_map", 13);
		program->update_uniform("use_cube_map_reflection", 0);
		program->update_uniform("cube_map_reflection_strength", 0.85f);
		program->update_uniform("camera_coords", glm::vec3(camera->position.x, camera->position.y, camera->position.z));
		program->update_uniform("active_texture_indicies", graphic->unordered_material->get_active_textures_by_type());
		program->update_uniform("shadow_map", 2);
	};

	pipeline.graphic_uniforms["shadowmap"] = [](Graphic_s graphic, Program_s program, Camera_s camera) {
		program->update_uniform("model", graphic->model_matrix);
	};
	pipeline.directionallight_uniforms["shadowmap"] = [](DirectionalLight_s light, Program_s program, Camera_s camera) {
		program->update_uniform("view", light->light_view_matrix);
		program->update_uniform("projection", light->light_projection_matrix);
	};

	pipeline.cameras["default_camera"] = scene.camera;

	FrameBuffer_s shadowmap(1920, 1080, 0, false);
	shadowmap->texture_slot = 2;
	pipeline.framebuffers["shadowmap"] = shadowmap;

	while (frame.is_running()) {
		double frame_time = frame.handle_window();
		frame.display_performance();
		scene.camera->handle_movements(frame.window, frame_time);

		box->set_rotation(box->get_rotation() * glm::quat(glm::vec3(0, 0.0005 * frame_time, 0)));
		//plane->set_rotation(plane->get_rotation() * glm::quat(glm::vec3(0, -0.0002 * frame_time, 0)));

		//pipeline.activate_framebuffer("shadowmap");
		pipeline.activate_camera("default_camera");
		pipeline.activate_program("depth");
		pipeline.activate_uniforms_graphic("shadowmap");
		pipeline.activate_uniforms_directional_light("shadowmap");
		frame.clear_window(0.2, 0.2, 0.2);
		pipeline.render();
		
		//shadowmap->unbind();

		//shadowmap->color_texture.bind();
		//
		//frame.clear_window(1, 1, 1, 1);
		//box->load_program(solid_program);
		//box->set_uniform_all(box_solid_uniforms);
		//plane->load_program(solid_program);
		//plane->set_uniform_all(plane_solid_uniforms);
		//scene.render(false);

		//shadowmap->save().save_to_disc("shadowmap_image.png");

	}


}