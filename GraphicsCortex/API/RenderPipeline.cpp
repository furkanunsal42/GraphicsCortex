#include "RenderPipeline.h"

// uniform updating
void RenderPipeline::activate_uniforms_graphic(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_graphic = uniform_updater_name;
}
void RenderPipeline::activate_uniforms_ambiant_light(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_ambiant_light = uniform_updater_name;
}
void RenderPipeline::activate_uniforms_directional_light(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_directional_light = uniform_updater_name;
}
void RenderPipeline::activate_uniforms_point_light(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_point_light = uniform_updater_name;
}
void RenderPipeline::activate_uniforms_spot_light(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_spot_light = uniform_updater_name;
}
void RenderPipeline::activate_uniforms_framebuffer(const std::string& uniform_updater_name) {
	_active_uniform_updater_name_framebuffer = uniform_updater_name;
}

// object activation

void RenderPipeline::activate_program(const std::string& program_name) {
	_active_name_program = program_name;
}
void RenderPipeline::activate_framebuffer(const std::string& framebuffer_name) {
	_active_name_framebuffer = framebuffer_name;
}
void RenderPipeline::activate_camera(const std::string& camera_name) {
	_active_name_camera = camera_name;
}

void RenderPipeline::reset_active_objects() {
	_active_uniform_updater_name_graphic = "";
	_active_uniform_updater_name_ambiant_light = "";
	_active_uniform_updater_name_directional_light = "";
	_active_uniform_updater_name_point_light = "";
	_active_uniform_updater_name_spot_light = "";
	_active_uniform_updater_name_framebuffer = "";
}

void RenderPipeline::render() {

	auto camera_iterator = cameras.find(_active_name_camera);
	auto program_iterator = programs.find(_active_name_program);

	if (camera_iterator == cameras.end()) {
		std::cout << "[Graphics Pipline Error] Active Camera Name Not Found" << std::endl;
		ASSERT(false);
	}

	if (program_iterator == programs.end()) {
		std::cout << "[Graphics Pipline Error] Active Program Name Not Found" << std::endl;
		ASSERT(false);
	}

	Camera_s active_camera = (*camera_iterator).second;
	Program_s active_program = (*program_iterator).second;

	active_program->bind();
	active_camera->update_matrixes();

	// update all uniforms
	int object_index = 0;
	for (auto& name_ambiant_pair : ambiant_ligths) {
		ambiantlight_uniforms[_active_uniform_updater_name_ambiant_light](name_ambiant_pair.second, active_program, active_camera, object_index++);
	}

	object_index = 0;
	for (auto& name_directional_pair : directional_ligths) {
		name_directional_pair.second->update_matricies();
		directionallight_uniforms[_active_uniform_updater_name_directional_light](name_directional_pair.second, active_program, active_camera, object_index++);
	}

	object_index = 0;
	for (auto& name_spot_pair : spot_ligths) {
		spotlight_uniforms[_active_uniform_updater_name_spot_light](name_spot_pair.second, active_program, active_camera, object_index++);
	}

	object_index = 0;
	for (auto& name_point_pair : point_ligths) {
		pointlight_uniforms[_active_uniform_updater_name_point_light](name_point_pair.second, active_program, active_camera, object_index++);
	}

	object_index = 0;
	for (auto& name_framebuffer_pair : framebuffers) {
		framebuffer_uniforms[_active_uniform_updater_name_framebuffer](name_framebuffer_pair.second, active_program, active_camera, object_index++);
	}

	object_index = 0;
	for (auto& name_graphic_pair : graphics) {
		Graphic_s graphic = name_graphic_pair.second;
		graphic->update_matrix();
		graphic_uniforms[_active_uniform_updater_name_graphic](graphic, active_program, active_camera, object_index++);
		graphic->draw(false);
	}
}

void RenderPipeline::set_rendering_sequence(std::function<void(RenderPipeline&, Frame&)> rendering_sequence) {
	_rendering_sequence = rendering_sequence;
}

void RenderPipeline::render_sequence(Frame& frame) {
	_rendering_sequence(*this, frame);
}