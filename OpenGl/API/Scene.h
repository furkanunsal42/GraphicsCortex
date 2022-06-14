#pragma once

#include <vector>
#include "Camera.h"
#include "Graphic.h"
#include "ShaderCompiler.h"

class Scene {
public:
	std::vector<Graphic*> meshes;
	Camera* camera;
	std::string model_uniform_name = "model";
	std::string view_uniform_name = "view";
	std::string projection_uniform_name = "projection";

	void render(Program program);
};