#include "Scene.h"

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "Debuger.h"

void Scene::render(Program program) {
	program.bind();
	camera->update_matrixes();
	for(Graphic* mesh : meshes){
		mesh->update_matrix();
		program.update_uniform(model_uniform_name, mesh->model_matrix);
		program.update_uniform(view_uniform_name, camera->view_matrix);
		program.update_uniform(projection_uniform_name, camera->projection_matrix);
		
		(*mesh).draw();
	}
}