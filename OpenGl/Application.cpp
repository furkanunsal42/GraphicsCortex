/*
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "iostream"
#include "ShaderCompiler.h"

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Winodow", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();
	
	// define vertex shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vertex_shader_code =
		"#version 330 core\n"
		"layout(location = 0) in vec4 position;\n"
		"void main(){\n"
		"	gl_Position = position;\n"
		"}\n";
	const char* vertex_pointer = &vertex_shader_code[0];
	glShaderSource(vertex_shader, 1, &vertex_pointer, nullptr);
	glCompileShader(vertex_shader);

	Shader shader = read_shader("Shaders\Shader.shdr");
	Program program(shader.vertex_shader, shader.fragment_shader);
	program.bind();
	
	// create float array for triangle's verticies
	float positions[] =
	{	-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	// create buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// select buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// fill buffer with data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions, GL_STATIC_DRAW);
	// define the structure of the data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	
	// enable the buffer to be used when draw functions are called
	glEnableVertexAttribArray(0);

	unsigned int index = 0;
	while (!glfwWindowShouldClose(window)) {
		// handle windows events
		glfwPollEvents();
		
		// clear the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// update window buffer to render the window
		glfwSwapBuffers(window);
		index++;
		std::cout << index << "\n";
	}

	// upon program exit, destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
};
*/