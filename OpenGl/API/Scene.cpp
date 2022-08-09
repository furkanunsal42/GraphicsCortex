#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Debuger.h"
#include "Frame.h"

bool once = true;
void Scene::render(GLFWwindow* window) {

	if (camera != nullptr)
		camera->update_matrixes();

	AmbiantLight::count = 0;
	DirectionalLight::count = 0;
	PointLight::count = 0;
	SpotLight::count = 0;
	
	for (Light* light : lights) {
		light->update_uniforms();
	}

	once = true;
	for(Graphic* mesh : meshes){

		mesh->update_matrix();
		mesh->update_uniform_queue(once);
		mesh->draw(false, false);
		once = false;
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