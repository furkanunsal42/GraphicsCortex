#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

#include "Default_Assets.h"
#include "Debuger.h"
#include "Frame.h"

#include "assimp/camera.h"

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

	render(window);

	frame_buffer->unbind();
	frame_buffer->render();

	glfwSwapBuffers(window);
}