#pragma once

#include "RenderPipeline.h"

class RenderPipeline_MultiTextured : public RenderPipeline {
public:
	std::unordered_map<std::string, Graphic_s> multitextued_graphics;
	
	void deattach_graphic(const std::string& graphic_name) {
		auto graphic = graphics.find(graphic_name);
		if (graphic != graphics.end()) {
			deattached_graphics[graphic->first] = graphic->second;
			graphics.erase(graphic);
		}

		graphic = multitextued_graphics.find(graphic_name);
		if (graphic != multitextued_graphics.end()) {
			deattached_graphics[graphic->first] = graphic->second;
			multitextued_graphics.erase(graphic);
		}

	}

	void attach_garphic(const std::string& graphic_name) {
		auto graphic = deattached_graphics.find(graphic_name);
		if (graphic != deattached_graphics.end()) {
			graphics[graphic->first] = graphic->second;
			deattached_graphics.erase(graphic);
		}
	}

	void attach_garphic_multitextured(const std::string& graphic_name) {
		auto graphic = deattached_graphics.find(graphic_name);
		if (graphic != deattached_graphics.end()) {
			multitextued_graphics[graphic->first] = graphic->second;
			deattached_graphics.erase(graphic);
		}
	}

	void render_single_graphic(const std::string& graphic_name) {
		bind_active_objects();

		{
			auto graphic_iterator_attached = graphics.find(graphic_name);
			if (graphic_iterator_attached != graphics.end()) {
				Graphic_s& graphic = graphic_iterator_attached->second;
				graphic->update_matrix();
				graphic_uniforms[_active_uniform_updater_name_graphic](graphic, programs[_active_name_program], cameras[_active_name_camera], 0);
				graphic->draw(false);
			}
		}

		{
			auto graphic_iterator_deattached = deattached_graphics.find(graphic_name);
			if (graphic_iterator_deattached != deattached_graphics.end()) {
				Graphic_s& graphic = graphic_iterator_deattached->second;
				graphic->update_matrix();
				graphic_uniforms[_active_uniform_updater_name_graphic](graphic, programs[_active_name_program], cameras[_active_name_camera], 0);
				graphic->draw(false);
			}
		}

		{
			auto graphic_iterator_multitextured = multitextued_graphics.find(graphic_name);
			if (graphic_iterator_multitextured != multitextued_graphics.end()) {
				Graphic_s& graphic = graphic_iterator_multitextured->second;
				graphic->update_matrix();
				graphic_uniforms[_active_uniform_updater_name_graphic](graphic, programs[_active_name_program], cameras[_active_name_camera], 0);
				graphic->draw(false);
			}
		}
	}

	void render_multitextured() {

		bind_active_objects();

		int object_index = 0;
		for (auto& name_graphic_pair : multitextued_graphics) {
			Graphic_s graphic = name_graphic_pair.second;
			graphic->update_matrix();
			graphic_uniforms[_active_uniform_updater_name_graphic](graphic, programs[_active_name_program], cameras[_active_name_camera], object_index++);
			graphic->draw(false);
		}
	}
};