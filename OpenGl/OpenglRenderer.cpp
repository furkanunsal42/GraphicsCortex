#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Debuger.h"
#include "ShaderCompiler.h"

int main() {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(640, 480, "Opengl_Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();

	float verticies[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};
	unsigned int vertex_array;
	GLCall(glGenVertexArrays(1, &vertex_array));
	GLCall(glBindVertexArray(vertex_array));
	
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verticies, GL_STATIC_DRAW));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	unsigned int triangles[] = {
		0, 1, 2,
		0, 3, 2
	};
	unsigned int index_buffer;
	GLCall(glGenBuffers(1, &index_buffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), triangles, GL_STATIC_DRAW));

	Shader shader_file = read_shader("Shaders/Shader.shdr");

	unsigned int program = compile_program(shader_file.vertex_shader, shader_file.fragment_shader);
	GLCall(glUseProgram(program));
	
	unsigned int unform_color = glGetUniformLocation(program, "u_color");
	float r = 0.2f;
	float change = 0.005f;
	int index = 0;
	while (!glfwWindowShouldClose(window)){
		std::cout << index << '\n';
		index++;
		glfwPollEvents();

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		r += change;
		if (r > 1){ 
			r = 1;
			change *= -1;
		}
		else if (r < 0) {
			r = 0;
			change *= -1;
		}

		GLCall(glUniform4f(unform_color, r, 0.25f + 0.5f * cos(5*(1.6f + r*6.28f)), 0.25f + 0.5f * sin(5*(r * 6.28f)), 1.0f));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}