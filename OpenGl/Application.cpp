/*
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "iostream"

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

	// define fragment shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fragment_shader_code =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;\n"
		"void main(){\n"
		"	color = vec4(0.2f, 0.4f, 0.6f, 1.0f);\n"
		"}\n";
	const char* fragment_pointer = &fragment_shader_code[0];
	glShaderSource(fragment_shader, 1, &fragment_pointer, nullptr);
	glCompileShader(fragment_shader);

	// concatinate the shaders into single program
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glValidateProgram(program);

	// since shaders are now compiled into single program, we can remove them from memory
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

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

	// use compiled program in the gpu
	glUseProgram(program);

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