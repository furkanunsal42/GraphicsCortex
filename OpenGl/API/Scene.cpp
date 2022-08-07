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
		
		default_program::solid_program_manager::get().lights = &lights;
		default_program::solid_program_manager::get().mesh = mesh;
		default_program::solid_program_manager::get().program = mesh->renderer;
		default_program::solid_program_manager::get().cam = camera;

		default_program::solid_program_manager::get().update_uniforms(true);
		/*
		mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::MODEL, mesh->model_matrix);
		
		mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::CUBE_MAP, 13);
		mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_CUBE_MAP_REFLECTION, 0);
		
		if (once){
			if (camera != nullptr){
				mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::VIEW, camera->view_matrix);
				mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::PROJECTION, camera->projection_matrix);
				camera->update_uniforms(*(mesh->renderer));
			}
		}

		if ((mesh->renderer != nullptr) && (mesh->material != nullptr)) {
			
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_COLOR_MAP, (int)(mesh->material->color_map != nullptr));
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_SPECULAR_MAP, (int)(mesh->material->specular_map != nullptr));
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_NORMAL_MAP, (int)(mesh->material->normal_map != nullptr));

			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::COLOR_MAP_SLOT, mesh->material->color_map_slot);
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::SPECULAR_MAP_SLOT, mesh->material->specular_map_slot);
			mesh->renderer->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::NORMAL_MAP_SLOT, mesh->material->normal_map_slot);
		}
		*/
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