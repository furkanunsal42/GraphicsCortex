#pragma once

#include <vector>
#include "Camera.h"
#include "Graphic.h"
#include "ShaderCompiler.h"
#include "Lights.h";
#include "FrameBuffer.h"

class Scene {
public:
	std::vector<Graphic*> meshes;
	std::vector<Light*> lights;
	Camera* camera = nullptr;
	std::string model_uniform_name = "model";
	std::string view_uniform_name = "view";
	std::string projection_uniform_name = "projection";
	glm::vec4 background_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FrameBuffer* frame_buffer = nullptr;

	void render(GLFWwindow* window);
	void render_frame_buffer(GLFWwindow* window);
};