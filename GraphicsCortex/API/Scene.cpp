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

void Scene::add_graphic(Graphic_s graphic) {
	_graphics.push_back(graphic.obj);
}

void Scene::add_object(Object_s object) {
	_objects.push_back(object.obj);
	PhysicsScene::get().add_actor(object->physics);
}

void Scene::add_object(Vehicle_s vehicle) {
	_vehicles.push_back(vehicle.obj);
	PhysicsScene::get().add_actor(vehicle->physics_representation);
}

void Scene::add_text(std::shared_ptr<Text> text) {
	_texts.push_back(text);
}

void Scene::set_skybox(std::shared_ptr<CubeMapTexture> cubemap) {
	skybox = cubemap;
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
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*graphic->renderer);

		graphic->draw(show_warnings);
	}
	for (std::shared_ptr<Object> object : _objects) {
		object->graphics->update_matrix();

		object->graphics->update_default_uniforms(*object->graphics->renderer);
		camera->update_default_uniforms(*object->graphics->renderer);

		object->graphics->update_uniforms();

		AmbiantLight::temp_light_index = 0;
		DirectionalLight::temp_light_index = 0;
		PointLight::temp_light_index = 0;
		SpotLight::temp_light_index = 0;
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*object->graphics->renderer);

		object->graphics->draw(show_warnings);
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
			for (std::shared_ptr<Light> light : _lights)
				light->update_default_uniforms(*vehicle->chassis->renderer);

			vehicle->chassis->draw(show_warnings);
		}
		
		if (vehicle->wheel_graphic_initialized) {
			for (Graphic_s wheel : vehicle->wheels) {
				wheel->update_matrix();

				wheel->update_default_uniforms(*wheel->renderer);
				camera->update_default_uniforms(*wheel->renderer);

				wheel->update_uniforms();

				AmbiantLight::temp_light_index = 0;
				DirectionalLight::temp_light_index = 0;
				PointLight::temp_light_index = 0;
				SpotLight::temp_light_index = 0;
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
	frame_buffer.bind();

	frame.clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	render(show_warnings);

	frame_buffer.unbind();
}

void Scene::sync_with_physics() {
	for (std::shared_ptr<Object> object : _objects) {
		object->sync_with_physics();
	}
	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		vehicle->sync_with_physics();
	}
}
