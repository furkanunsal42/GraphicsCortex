#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Debuger.h"

#include <iostream>

void Scene::render() {
	camera->update_matrixes();

	AmbiantLight::count = 0;
	DirectionalLight::count = 0;
	PointLight::count = 0;
	SpotLight::count = 0;
	for (Light* light : lights) {

		light->update_uniforms();
	}

	for(Graphic* mesh : meshes){
		mesh->update_matrix();
		mesh->renderer.update_uniform(model_uniform_name, mesh->model_matrix);
		mesh->renderer.update_uniform(view_uniform_name, camera->view_matrix);
		mesh->renderer.update_uniform(projection_uniform_name, camera->projection_matrix);
		
		mesh->draw();
	}
}