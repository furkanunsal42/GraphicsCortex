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
		// temp
		mesh->update_matrix();

		mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::MODEL, mesh->model_matrix);
		
		if (once){
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::VIEW, camera->view_matrix);
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::PROJECTION, camera->projection_matrix);
			
			camera->update_uniforms(*(mesh->renderer));
			
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_COLOR_MAP, (int)(mesh->material->color_map != nullptr));
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_SPECULAR_MAP, (int)(mesh->material->specular_map != nullptr));
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_NORMAL_MAP, (int)(mesh->material->normal_map != nullptr));
		}
		
		mesh->draw(false, !once);
		once = false;
	}
}

void Scene::render_to_framebuffer(GLFWwindow* window) {
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

	//meshes[0]->renderer->bind();
	//meshes[0]->vertex_buffer.bind();
	//meshes[0]->index_buffer.bind();
	meshes[0]->material->bind();

	for (Graphic* mesh : meshes) {
		// temp
		camera->update_uniforms(*(mesh->renderer));

		mesh->update_matrix();
		mesh->renderer->update_uniform(model_uniform_name, mesh->model_matrix);
		mesh->renderer->update_uniform(view_uniform_name, camera->view_matrix);
		mesh->renderer->update_uniform(projection_uniform_name, camera->projection_matrix);

		mesh->renderer->update_uniform("use_color_map", (int)(mesh->material->color_map != nullptr));
		mesh->renderer->update_uniform("use_specular_map", (int)(mesh->material->specular_map != nullptr));
		mesh->renderer->update_uniform("use_normal_map", (int)(mesh->material->normal_map != nullptr));

		mesh->draw();
	}

	frame_buffer->unbind();
	frame_buffer->render();

	glfwSwapBuffers(window);
}