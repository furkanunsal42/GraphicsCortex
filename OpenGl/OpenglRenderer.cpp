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
#include "External\stb_image\stb_image.h"


int main() {
	
	GLFWwindow* window = frame::create_window(640, 480, "My Window");

	std::vector<float> custom_verticies {
		// verticies	colors				texture
		-0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,	0.2f, 1.0f, 0.5f,	0.0f, 1.0f
	};
	
	/*
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*custom_verticies.size(), (float*)&custom_verticies[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0*sizeof(float)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2*sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5*sizeof(float)));
	*/

	ArrayBuffer array_buffer(custom_verticies);
	array_buffer.push_attribute(2);
	array_buffer.push_attribute(3);
	array_buffer.push_attribute(2);

	std::vector<unsigned int> triangles{
		0, 1, 2,
		0, 2, 3
	};
	/*
	unsigned int index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), (unsigned int*)&triangles[0], GL_STATIC_DRAW);
	*/

	IndexBuffer index_buffer(triangles, 3);
	
	Graphic orange(array_buffer, index_buffer);
	
	int height, width, channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* image_bytes = stbi_load("orange.png", &width, &height, &channels, 0);	
	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_bytes));
	glGenerateMipmap(GL_TEXTURE_2D);
	
	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	program.bind();
	program.define_uniform("texture_slot");
	GLCall(glUniform1i(program.uniforms["texture_slot"], 0));
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		
		frame::clear_window();

		program.bind();

		//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		orange.draw(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
