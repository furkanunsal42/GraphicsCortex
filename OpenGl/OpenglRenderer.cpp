#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>

#include "API\Debuger.h"
#include "API\Frame.h"
#include "API\Buffer.h"
#include "API\ShaderCompiler.h"
#include "API\Graphic.h"
#include "API\Texture.h"
#include "API\Camera.h"
#include "API\Scene.h"
#include "API\Default_Geometries.h"

int main() {
	
	int width = 640, height = 480;
	GLFWwindow* window = frame::create_window(width, height, "My Window", 1, true, false);
	
	Texture texture;
	texture.internal_format = GL_RGB;
	texture.format = GL_RGB;
	texture.vertical_flip = true;
	texture.load_image("cobble.png");
	
	Shader shader_file("Shaders/SolidVertex.glsl", "Shaders/SolidFragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);

	Graphic cube = default_geometry::cube(texture, program);

	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;

	AmbiantLight ambiant(glm::vec3(0.1f, 0.0f, 0.0f), program);
	AmbiantLight ambiant2(glm::vec3(0.0f, 0.1f, 0.0f), program);
	AmbiantLight ambiant3(glm::vec3(0.0f, 0.0f, 0.1f), program);
	DirectionalLight directional(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), program);
	PointLight point(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 1.0f, 0.0f, program);

	Scene scene;
	scene.meshes.push_back(&cube);
	scene.camera = &cam;
	scene.lights.push_back(&ambiant);
	scene.lights.push_back(&ambiant2);
	scene.lights.push_back(&ambiant3);
	scene.lights.push_back(&directional);
	scene.lights.push_back(&point);

	program.update_uniform("texture_slot", 0);

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		frame::display_fps();
		frame::clear_window();
		
		cube.position.x += 0.01f;

		//glm::vec4 point = cube.model_matrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		//std::cout << "x: " << point.x << " y: " << point.y << " z: " << point.z << " w: " << point.w << std::endl;

		scene.camera->handle_movements(window);
		scene.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
