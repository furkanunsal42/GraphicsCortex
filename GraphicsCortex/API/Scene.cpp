#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Default_Programs.h"
#include "Debuger.h"
#include "Frame.h"

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

void Scene::set_framebuffer(FrameBuffer& framebuffer) {
	frame_buffer = std::move(framebuffer);
	_is_framebuffer_loaded = true;
}

void Scene::render(bool show_warnings) {
	
	sync_with_physics();

	camera->update_matrixes();

	for (std::shared_ptr<Light> light : _lights) {
		light->update_uniform_queue();
	}

	for(std::shared_ptr<Graphic> graphic : _graphics){
		graphic->update_matrix();
		if (!graphic->is_uniform_queue_loaded()) {
			std::cout << "[Opengl Error] Graphic::update_uniforms() was called but no uniform_queue is specified, basic uniform queue is being loaded to graphic by the respected scene" << std::endl;
			graphic->set_uniform_all(default_program::basic_uniform_queue(*camera.obj, Graphic_s(graphic)));
		}
		graphic->update_uniforms();
		graphic->draw(show_warnings);
	}
	for (std::shared_ptr<Object> object : _objects) {
		object->graphics->update_matrix();
		if (!object->graphics->is_uniform_queue_loaded()) {
			std::cout << "[Opengl Error] Graphic::update_uniforms() was called but no uniform_queue is specified, basic uniform queue is being loaded to graphic by the respected scene" << std::endl;
			object->graphics->set_uniform_all(default_program::basic_uniform_queue(*camera.obj, Graphic_s(object->graphics)));
		}
		object->graphics->update_uniforms();
		object->graphics->draw(show_warnings);
	}
	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		if (vehicle->chassis_graphic_initialized) {
			vehicle->chassis->update_matrix();
			if (!vehicle->chassis->is_uniform_queue_loaded()) {
				std::cout << "[Opengl Error] Graphic::update_uniforms() was called but no uniform_queue is specified, basic uniform queue is being loaded to graphic by the respected scene" << std::endl;
				vehicle->chassis->set_uniform_all(default_program::basic_uniform_queue(*camera.obj, Graphic_s(vehicle->chassis)));
			}
			vehicle->chassis->update_uniforms();
			vehicle->chassis->draw(show_warnings);
		}
		
		if (vehicle->wheel_graphic_initialized) {
			for (Graphic_s wheel : vehicle->wheels) {
				wheel->update_matrix();
				if (!wheel->is_uniform_queue_loaded()) {
					std::cout << "[Opengl Error] Graphic::update_uniforms() was called but no uniform_queue is specified, basic uniform queue is being loaded to graphic by the respected scene" << std::endl;
					wheel->set_uniform_all(default_program::basic_uniform_queue(*camera.obj, wheel));
				}
				wheel->update_uniforms();
				wheel->draw(show_warnings);
			}
		}
	}
}

void Scene::render_to_framebuffer(Frame& frame, bool show_warnings) { 
	if (!_is_framebuffer_loaded) {
		std::cout << "[Opengl Error] Scene::render_to_framebuffer() is called but Scene::framebuffer is not set." << std::endl;
		return;
	}

	frame_buffer.bind();

	frame.clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	render(show_warnings);

	frame_buffer.unbind();
	frame_buffer.render();
}

void Scene::sync_with_physics() {
	for (std::shared_ptr<Object> object : _objects) {
		object->sync_with_physics();
	}
	for (std::shared_ptr<Vehicle> vehicle : _vehicles) {
		vehicle->sync_with_physics();
	}
}
