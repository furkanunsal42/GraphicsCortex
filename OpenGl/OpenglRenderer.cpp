#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "API/Debuger.h"
#include "API/Frame.h"
#include "API/Buffer.h"
#include "API/ShaderCompiler.h"
#include "API/Graphic.h"
#include "API/Texture.h"

int main() {
	
	int width = 640, height = 480;
	GLFWwindow* window = frame::create_window(width, height, "My Window");

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
	
	Graphic orange(array_buffer, index_buffer);
	
	Texture texture;
	texture.vertical_flip = true;
	texture.load_image("orange.png");
	texture.bind();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.05f, -2.5f));;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);


	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	program.bind();

	program.update_uniform("texture_slot", 0);
	program.update_uniform("u_color", -0.3f, -0.3f, -0.3f, 1.0f);
	program.update_uniform("model", model);
	program.update_uniform("view", view);
	program.update_uniform("projection", projection);

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		
		frame::clear_window();

		model = glm::rotate(model, glm::radians(0.4f), glm::vec3(0.0f, 1.0f, 0.0f));
		program.update_uniform("model", model);
		//view = glm::translate(view, glm::vec3(0,0,-0.002f));
		//program.update_uniform("view", view);

		program.bind();

		orange.draw(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
