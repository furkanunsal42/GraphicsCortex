#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Debuger.h"
#include "Frame.h"

void Scene::render(GLFWwindow* window) {
	glfwPollEvents();
	frame::clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	camera->update_matrixes();
	AmbiantLight::count = 0;
	DirectionalLight::count = 0;
	PointLight::count = 0;
	SpotLight::count = 0;
	for (Light* light : lights) {

		light->update_uniforms();
	}

	for(Graphic* mesh : meshes){
		// temp
		camera->update_uniforms(*(mesh->renderer));

		mesh->update_matrix();
		mesh->renderer->update_uniform(model_uniform_name, mesh->model_matrix);
		mesh->renderer->update_uniform(view_uniform_name, camera->view_matrix);
		mesh->renderer->update_uniform(projection_uniform_name, camera->projection_matrix);
		
		mesh->renderer->update_uniform("use_color_map", (int)(mesh->material.color_map != nullptr));
		mesh->renderer->update_uniform("use_specular_map", (int)(mesh->material.specular_map != nullptr));
		mesh->renderer->update_uniform("use_normal_map", (int)(mesh->material.normal_map != nullptr));

		mesh->draw();
	}

	glfwSwapBuffers(window);
}

void Scene::render_frame_buffer(GLFWwindow* window) {
	frame_buffer->bind();
	
	// from render()
	glfwPollEvents();
	frame::clear_window(background_color.x, background_color.y, background_color.z, background_color.w);

	camera->update_matrixes();
	AmbiantLight::count = 0;
	DirectionalLight::count = 0;
	PointLight::count = 0;
	SpotLight::count = 0;
	for (Light* light : lights) {

		light->update_uniforms();
	}

	for (Graphic* mesh : meshes) {
		// temp
		camera->update_uniforms(*(mesh->renderer));

		mesh->update_matrix();
		mesh->renderer->update_uniform(model_uniform_name, mesh->model_matrix);
		mesh->renderer->update_uniform(view_uniform_name, camera->view_matrix);
		mesh->renderer->update_uniform(projection_uniform_name, camera->projection_matrix);

		mesh->renderer->update_uniform("use_color_map", (int)(mesh->material.color_map != nullptr));
		mesh->renderer->update_uniform("use_specular_map", (int)(mesh->material.specular_map != nullptr));
		mesh->renderer->update_uniform("use_normal_map", (int)(mesh->material.normal_map != nullptr));

		mesh->draw();
	}

	frame_buffer->unbind();
	frame_buffer->render();

	glfwSwapBuffers(window);
}