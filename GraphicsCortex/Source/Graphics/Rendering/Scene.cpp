#include "Scene.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Default_Programs.h"
#include "Debuger.h"
#include "Frame.h"
#include "Text.h"
#include "SkyBox.h"
#include "PhysicsScene.h"


Scene::Scene(const Frame& frame) {
	camera->screen_width = frame.window_width;
	camera->screen_height = frame.window_height;
}

void Scene::add(std::shared_ptr<Graphic> graphic) {
	_graphics.push_back(graphic);
	_new_graphic_added = true;
}

void Scene::add(std::shared_ptr<Object> object) {
	_objects.push_back(object);
	PhysicsScene::get().add_actor(object->physics);
	_new_graphic_added = true;
}

void Scene::add(std::shared_ptr<Vehicle> vehicle) {
	_vehicles.push_back(vehicle);
	PhysicsScene::get().add_actor(vehicle->physics_representation);
	_new_graphic_added = true;
}

void Scene::add(std::shared_ptr<Text> text) {
	_texts.push_back(text);
	// NOT PIPELINE SOURCE
}

void Scene::set_skybox(std::shared_ptr<SkyBox> cubemap) {
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

	if (skybox != nullptr) {
		skybox->render(*camera);
	}

	for(std::shared_ptr<Graphic> graphic : _graphics){
		graphic->update_matrix();

		graphic->update_default_uniforms(*graphic->material->program);
		camera->update_default_uniforms(*graphic->material->program);
		
		AmbiantLight::temp_light_index = 0;
		DirectionalLight::temp_light_index = 0;
		PointLight::temp_light_index = 0;
		SpotLight::temp_light_index = 0;
		graphic->material->program->update_uniform("a_lights_count", 0);
		graphic->material->program->update_uniform("d_lights_count", 0);
		graphic->material->program->update_uniform("p_lights_count", 0);
		graphic->material->program->update_uniform("s_lights_count", 0);
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*graphic->material->program);

		graphic->draw(show_warnings);
	}
	for (std::shared_ptr<Object> object : _objects) {
		object->graphics.update_matrix();

		object->graphics.update_default_uniforms(*object->graphics.material->program);
		camera->update_default_uniforms(*object->graphics.material->program);

		AmbiantLight::temp_light_index = 0;
		DirectionalLight::temp_light_index = 0;
		PointLight::temp_light_index = 0;
		SpotLight::temp_light_index = 0;
		object->graphics.material->program->update_uniform("a_lights_count", 0);
		object->graphics.material->program->update_uniform("d_lights_count", 0);
		object->graphics.material->program->update_uniform("p_lights_count", 0);
		object->graphics.material->program->update_uniform("s_lights_count", 0);
		for (std::shared_ptr<Light> light : _lights)
			light->update_default_uniforms(*object->graphics.material->program);

		object->graphics.draw(show_warnings);
	}

	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		if (vehicle->chassis_graphic_initialized) {
			vehicle->chassis->update_matrix();
		
			vehicle->chassis->update_default_uniforms(*vehicle->chassis->material->program);
			camera->update_default_uniforms(*vehicle->chassis->material->program);
			
			AmbiantLight::temp_light_index = 0;
			DirectionalLight::temp_light_index = 0;
			PointLight::temp_light_index = 0;
			SpotLight::temp_light_index = 0;
			vehicle->chassis->material->program->update_uniform("a_lights_count", 0);
			vehicle->chassis->material->program->update_uniform("d_lights_count", 0);
			vehicle->chassis->material->program->update_uniform("p_lights_count", 0);
			vehicle->chassis->material->program->update_uniform("s_lights_count", 0);
			for (std::shared_ptr<Light> light : _lights)
				light->update_default_uniforms(*vehicle->chassis->material->program);

			vehicle->chassis->draw(show_warnings);
		}
		
		if (vehicle->wheel_graphic_initialized) {
			for (std::shared_ptr<Graphic> wheel : vehicle->wheels) {
				wheel->update_matrix();

				wheel->update_default_uniforms(*wheel->material->program);
				camera->update_default_uniforms(*wheel->material->program);

				AmbiantLight::temp_light_index = 0;
				DirectionalLight::temp_light_index = 0;
				PointLight::temp_light_index = 0;
				SpotLight::temp_light_index = 0;
				wheel->material->program->update_uniform("a_lights_count", 0);
				wheel->material->program->update_uniform("d_lights_count", 0);
				wheel->material->program->update_uniform("p_lights_count", 0);
				wheel->material->program->update_uniform("s_lights_count", 0);
				for (std::shared_ptr<Light> light : _lights)
					light->update_default_uniforms(*wheel->material->program);
				
				wheel->draw(show_warnings);
			}
		}
	}

	for (std::shared_ptr<Text> text : _texts) {
		text->update_default_uniforms(*text->graphic->material->program);
		camera->update_default_uniforms(*text->graphic->material->program);
		text->render();
	}
}

void Scene::render_pipeline()
{
	ASSERT(pipeline != nullptr);

	sync_with_physics();
	camera->update_matrixes();

	_update_pipeline_rendering_list();
	if (pipeline_rendering_list.size() == 0) return;
	
	pipeline->render(*this);
}

void Scene::sync_with_physics() {
	for (std::shared_ptr<Object> object : _objects) {
		object->sync_with_physics();
	}
	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		vehicle->sync_with_physics();
	}
}

void Scene::_update_pipeline_rendering_list()
{
	if (_new_graphic_added) {
		for (std::shared_ptr<Graphic> graphic : _graphics) {
			pipeline_rendering_list.push_back(*graphic);
		}
		for (std::shared_ptr<Object> object : _objects) {
			pipeline_rendering_list.push_back(object->graphics);
		}

		for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
			if (vehicle->chassis_graphic_initialized)
				pipeline_rendering_list.push_back(*vehicle->chassis);

			if (vehicle->wheel_graphic_initialized) {
				for (std::shared_ptr<Graphic> wheel : vehicle->wheels)
					pipeline_rendering_list.push_back(*wheel);
			}
		}
		_new_graphic_added = false;
	}
}
