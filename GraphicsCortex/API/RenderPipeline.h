#pragma once

#include <unordered_map>
#include <string>
#include "UniformUpdater.h"
#include "Frame.h"

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
	std::unordered_map<std::string, Graphic_s> deattached_graphics;

	std::unordered_map<std::string, UniformUpdater<Graphic_s>> graphic_uniforms;
	std::unordered_map<std::string, UniformUpdater<AmbiantLight_s>> ambiantlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<DirectionalLight_s>> directionallight_uniforms;
	std::unordered_map<std::string, UniformUpdater<PointLight_s>> pointlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<SpotLight_s>> spotlight_uniforms;
	std::unordered_map<std::string, UniformUpdater<FrameBuffer_s>> framebuffer_uniforms;

	// uniform updating
	void activate_uniforms_graphic(const std::string& uniform_updater_name);
	void activate_uniforms_ambiant_light(const std::string& uniform_updater_name);
	void activate_uniforms_directional_light(const std::string& uniform_updater_name);
	void activate_uniforms_point_light(const std::string& uniform_updater_name);
	void activate_uniforms_spot_light(const std::string& uniform_updater_name);
	void activate_uniforms_framebuffer(const std::string& uniform_updater_name);

	// object activation

	void activate_program(const std::string& program_name);
	void activate_framebuffer(const std::string& framebuffer_name);
	void activate_camera(const std::string& camera_name);

	void reset_active_objects();
	void bind_active_objects();
	void render();

	void deattach_graphic(const std::string& graphic_name);
	void attach_garphic(const std::string& graphic_name);
	void render_single_graphic(const std::string& graphic_name);

	void set_rendering_sequence(std::function<void(RenderPipeline*, Frame&)> rendering_sequence);
	void render_sequence(Frame& frame);

	std::string get_active_program_name();
	std::string get_active_framebuffer_name();
	std::string get_active_camera_name();
	std::string get_active_uniform_updater_name_graphic();
	std::string get_active_uniform_updater_name_ambiant_light();
	std::string get_active_uniform_updater_name_directional_light();
	std::string get_active_uniform_updater_name_point_light();
	std::string get_active_uniform_updater_name_spot_light();
	std::string get_active_uniform_updater_name_framebuffer();

protected:

	std::function<void(RenderPipeline*, Frame&)> _rendering_sequence;

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
