#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Default_Programs.h"
#include "Debuger.h"
#include "Frame.h"
#include "Text.h"
#include "CubeMap.h"

#include "PhysicsScene.h"


Scene::Scene(const Frame& frame) {
	camera->screen_width = frame.window_width;
	camera->screen_height = frame.window_height;
}

void Scene::add(std::shared_ptr<Graphic> graphic) {
	_graphics.push_back(graphic);
}

void Scene::add(std::shared_ptr<Object> object) {
	_objects.push_back(object);
	PhysicsScene::get().add_actor(object->physics);
}

void Scene::add(std::shared_ptr<Vehicle> vehicle) {
	_vehicles.push_back(vehicle);
	PhysicsScene::get().add_actor(vehicle->physics_representation);
}

void Scene::add(std::shared_ptr<Text> text) {
	_texts.push_back(text);
}

void Scene::set_skybox(std::shared_ptr<CubeMapTexture> cubemap) {
	skybox = cubemap;
}

void Scene::remove(std::shared_ptr<Graphic> graphic) {
	_graphics.erase(std::find(_graphics.begin(), _graphics.end(), graphic));
}
void Scene::remove(std::shared_ptr<Object> object) {
	_objects.erase(std::find(_objects.begin(), _objects.end(), object));
}
void Scene::remove(std::shared_ptr<Vehicle> vehicle) {
	_vehicles.erase(std::find(_vehicles.begin(), _vehicles.end(), vehicle));
}
void Scene::remove(std::shared_ptr<Text> text) {
	_texts.erase(std::find(_texts.begin(), _texts.end(), text));
}

void Scene::render(bool show_warnings) {
	
	sync_with_physics();

	camera->update_matrixes();

	for(std::shared_ptr<Graphic> graphic : _graphics){
		graphic->update_matrix();

		graphic->update_default_uniforms(*graphic->renderer);
		camera->update_default_uniforms(*graphic->renderer);
		
		graphic->update_uniforms();

		AmbiantLight::temp_light_index = 0;
		DirectionalLight::temp_light_index = 0;
		PointLight::temp_light_index = 0;
		SpotLight::temp_light_index = 0;
		graphic->renderer->update_uniform("a_lights_count", 0);
		graphic->renderer->update_uniform("d_lights_count", 0);
		graphic->renderer->update_uniform("p_lights_count", 0);
		graphic->renderer->update_uniform("s_lights_count", 0);
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*graphic->renderer);

		graphic->draw(show_warnings);
	}
	for (std::shared_ptr<Object> object : _objects) {
		object->graphics.update_matrix();

		object->graphics.update_default_uniforms(*object->graphics.renderer);
		camera->update_default_uniforms(*object->graphics.renderer);

		object->graphics.update_uniforms();

		AmbiantLight::temp_light_index = 0;
		DirectionalLight::temp_light_index = 0;
		PointLight::temp_light_index = 0;
		SpotLight::temp_light_index = 0;
		object->graphics.renderer->update_uniform("a_lights_count", 0);
		object->graphics.renderer->update_uniform("d_lights_count", 0);
		object->graphics.renderer->update_uniform("p_lights_count", 0);
		object->graphics.renderer->update_uniform("s_lights_count", 0);
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*object->graphics.renderer);

		object->graphics.draw(show_warnings);
	}

	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		if (vehicle->chassis_graphic_initialized) {
			vehicle->chassis->update_matrix();
		
			vehicle->chassis->update_default_uniforms(*vehicle->chassis->renderer);
			camera->update_default_uniforms(*vehicle->chassis->renderer);
			
			vehicle->chassis->update_uniforms();

			AmbiantLight::temp_light_index = 0;
			DirectionalLight::temp_light_index = 0;
			PointLight::temp_light_index = 0;
			SpotLight::temp_light_index = 0;
			vehicle->chassis->renderer->update_uniform("a_lights_count", 0);
			vehicle->chassis->renderer->update_uniform("d_lights_count", 0);
			vehicle->chassis->renderer->update_uniform("p_lights_count", 0);
			vehicle->chassis->renderer->update_uniform("s_lights_count", 0);
			for (std::shared_ptr<Light> light : _lights)
				light->update_default_uniforms(*vehicle->chassis->renderer);

			vehicle->chassis->draw(show_warnings);
		}
		
		if (vehicle->wheel_graphic_initialized) {
			for (std::shared_ptr<Graphic> wheel : vehicle->wheels) {
				wheel->update_matrix();

				wheel->update_default_uniforms(*wheel->renderer);
				camera->update_default_uniforms(*wheel->renderer);

				wheel->update_uniforms();

				AmbiantLight::temp_light_index = 0;
				DirectionalLight::temp_light_index = 0;
				PointLight::temp_light_index = 0;
				SpotLight::temp_light_index = 0;
				wheel->renderer->update_uniform("a_lights_count", 0);
				wheel->renderer->update_uniform("d_lights_count", 0);
				wheel->renderer->update_uniform("p_lights_count", 0);
				wheel->renderer->update_uniform("s_lights_count", 0);
				for (std::shared_ptr<Light> light : _lights)
					light->update_default_uniforms(*wheel->renderer);
				
				wheel->draw(show_warnings);
			}
		}
	}

	for (std::shared_ptr<Text> text : _texts) {
		text->update_default_uniforms(*text->graphic->renderer);
		camera->update_default_uniforms(*text->graphic->renderer);
		text->graphic->update_uniforms();
		text->render();
	}

	if (skybox != nullptr) {
		skybox->texture_slot = 11;
		skybox->update_default_uniforms(*skybox->cube.renderer);
		skybox->draw();
	}
	
}

void Scene::render_to_framebuffer(FrameBuffer& frame_buffer, Frame& frame, bool show_warnings) {

	frame_buffer.bind(FrameBuffer::WRITE_TARGET);

	frame.set_viewport(frame_buffer.width, frame_buffer.height);
	frame.clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	render(show_warnings);

	frame_buffer.unbind();
	
	frame.set_viewport(frame.window_width, frame.window_height);
}

void Scene::sync_with_physics() {
	for (std::shared_ptr<Object> object : _objects) {
		object->sync_with_physics();
	}
	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		vehicle->sync_with_physics();
	}
}
