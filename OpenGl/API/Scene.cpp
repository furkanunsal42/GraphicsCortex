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

void Scene::render(GLFWwindow* window) {

	if (camera != nullptr)
		camera->update_matrixes();

	for (Light* light : lights) {
		light->update_uniform_queue();
	}

	for(Graphic* mesh : meshes){

		mesh->update_matrix();
		mesh->update_uniform_queue();
		mesh->draw(false);
	}
}

void Scene::render_to_framebuffer(GLFWwindow* window) {
	frame_buffer->bind();
	
	// from render()
	glfwPollEvents();
	frame::clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	render(window);

	frame_buffer->unbind();
	frame_buffer->render();

	glfwSwapBuffers(window);
}