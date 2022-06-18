#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Debuger.h"

void Scene::render() {
	camera->update_matrixes();
	for(Graphic* mesh : meshes){
		mesh->update_matrix();
		mesh->renderer.update_uniform(model_uniform_name, mesh->model_matrix);
		mesh->renderer.update_uniform(view_uniform_name, camera->view_matrix);
		mesh->renderer.update_uniform(projection_uniform_name, camera->projection_matrix);
		
		(*mesh).draw();
	}
}