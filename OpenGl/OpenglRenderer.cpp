#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Debuger.h"
#include "ShaderCompiler.h"
#include "Buffer.h";

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "Opengl_Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	// define verticies of geometry
	float verticies[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	ArrayBuffer buffer(verticies, 2, sizeof(verticies)/sizeof(float));
	buffer.bind();

	// define triangles to create the geometry
	unsigned int triangles[] = {
		0, 1, 2,
		0, 3, 2
	};
	IndexBuffer index_buffer(triangles, 3, sizeof(triangles) / sizeof(*triangles));
	index_buffer.bind();
	
	// compile the shaders
	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	program.bind();
	
	// access uniform to set color from cpu and transfer it to gpu's vram real-time
	unsigned int unform_color = glGetUniformLocation(program.id, "u_color");
	
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
		GLCall(glUniform4f(unform_color, r, 0.25f + 0.5f * cos(5*(1.6f + r*6.28f)), 0.25f + 0.5f * sin(5*(r * 6.28f)), 1.0f));
		
		// draw currently binded data
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}