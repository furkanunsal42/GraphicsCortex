#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Debuger.h"
#include "ShaderCompiler.h"
#include "Buffer.h";
#include "Graphic.h"

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "Opengl_Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	std::vector<float> verticies = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	std::vector<unsigned int> triangles = {
		0, 1, 2,
		0, 3, 2
	};
	ArrayBuffer buffer(verticies, 2);
	IndexBuffer index_buffer(triangles, 3);
	Graphic square = Graphic(buffer, index_buffer);

	// compile the shaders
	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	
	// access uniform to set color from cpu and transfer it to gpu's vram real-time
	program.define_uniform("u_color");

	// parameters to change color
	float r = 0.2f;
	float change = 0.005f;
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		
		// update color 
		r += change;
		if (r > 1){ 
			r = 1;
			change *= -1;
		}
		else if (r < 0) {
			r = 0;
			change *= -1;
		}
		
		// send the uniform to vram
		program.update_uniform4f("u_color", r, 0.25f + 0.5f * cos(5 * (1.6f + r * 6.28f)), 0.25f + 0.5f * sin(5 * (r * 6.28f)), 1.0f);
		
		// draw currently binded data using program shader
		square.draw(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
