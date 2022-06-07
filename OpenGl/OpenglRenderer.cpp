#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Debuger.h"
#include "Frame.h"
#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Graphic.h"

int main() {
	
	GLFWwindow* window = frame::create_window(640, 480, "My Window");

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

	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	
	float r = 0;
	float change = 0.005f;
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		
		frame::clear_window();
		
		r += change;
		if (r > 1 || r < 0) 
			change *= -1;
		
		program.update_uniform4f("u_color", r, 0.25f + 0.5f * cos(5 * (1.6f + r * 6.28f)), 0.25f + 0.5f * sin(5 * (r * 6.28f)), 1.0f);
		
		square.draw(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
