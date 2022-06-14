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

int main() {
	
	GLFWwindow* window = frame::create_window(640, 480, "My Window");
	
	std::vector<float> custom_verticies {
		// verticies			colors				texture
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		0.2f, 1.0f, 0.5f,	0.0f, 1.0f
	};
	ArrayBuffer array_buffer(custom_verticies);
	array_buffer.push_attribute(3);
	array_buffer.push_attribute(3);
	array_buffer.push_attribute(2);

	std::vector<unsigned int> triangles{
		0, 1, 2,
		0, 2, 3
	};
	IndexBuffer index_buffer(triangles, 3);
	
	Texture texture;
	texture.vertical_flip = true;
	texture.load_image("orange.png");
	
	Graphic orange(array_buffer, index_buffer, texture);

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	Camera cam;
	std::cout << cam.screen_height;
	cam.screen_width = w;
	cam.screen_height = h;
	cam.position.z = 1.5f;

	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);

	Scene scene;
	scene.meshes.push_back(&orange);
	scene.camera = &cam;

	program.update_uniform("texture_slot", 0);
	program.update_uniform("u_color", -0.3f, -0.3f, -0.3f, 1.0f);


	int fps = 0;
	long old_time = std::time(nullptr);

	while (!glfwWindowShouldClose(window)){
		fps++;
		long now = std::time(nullptr);
		if (now - old_time >= 1) {
			std::cout << fps << std::endl;
			fps = 0;
			old_time = now;
		}
		glfwPollEvents();
		
		frame::clear_window();
		
		orange.rotation += glm::vec3(0.0f, 0.4f, 0.0f);
		orange.position += glm::vec3(0.0f, 0.0f, -0.003f);
		//scene.camera->rotation += glm::vec3(0.0f, 2.0f, 0.0f);
		//scene.camera->position += glm::vec3(0.0f, 0.0f, -0.003f);
		
		scene.camera->handle_movements(window);
		scene.render(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
