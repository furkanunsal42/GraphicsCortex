#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Debuger.h"
#include "Frame.h"

#include "PhysicsScene.h"

void Scene::add_graphic(Graphic& graphic) {
	meshes.push_back(&graphic);
}

void Scene::add_object(Object& object) {
	add_graphic(object.graphics);
	PhysicsScene::get().add_actor(object.physics);
}

void Scene::add_object(Vehicle& vehicle) {
	add_graphic(vehicle.chassis);
	for (Graphic& wheel : vehicle.wheels) {
		add_graphic(wheel);
	}
	PhysicsScene::get().add_actor(vehicle.physics_representation);
}

void Scene::set_framebuffer(FrameBuffer& framebuffer) {
	frame_buffer = std::move(framebuffer);
	_is_framebuffer_loaded = true;
}

void Scene::render(bool show_warnings) {

	camera.update_matrixes();

	for (Light* light : lights) {
		light->update_uniform_queue();
	}

	for(Graphic* mesh : meshes){
		mesh->update_matrix();
		mesh->update_uniforms();
		mesh->draw(show_warnings);
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