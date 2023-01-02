#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Debuger.h"
#include "Frame.h"

#include "PhysicsScene.h"

void Scene::add_graphic(Graphic_s graphic) {
	meshes.push_back(graphic.obj);
}

void Scene::add_object(Object_s object) {
	add_graphic(object.obj->graphics);
	PhysicsScene::get().add_actor(object.obj->physics);
}

void Scene::add_object(Vehicle_s vehicle) {
	add_graphic(vehicle.obj->chassis);
	for (Graphic_s& wheel : vehicle.obj->wheels) {
		add_graphic(wheel);
	}
	PhysicsScene::get().add_actor(vehicle.obj->physics_representation);
}

void Scene::set_framebuffer(FrameBuffer& framebuffer) {
	frame_buffer = std::move(framebuffer);
	_is_framebuffer_loaded = true;
}

void Scene::render(bool show_warnings) {

	camera.update_matrixes();

	for (std::shared_ptr<Light>& light : lights) {
		light->update_uniform_queue();
	}

	for(std::shared_ptr<Graphic>& mesh : meshes){
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