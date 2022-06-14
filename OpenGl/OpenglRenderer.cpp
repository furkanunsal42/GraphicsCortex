#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

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

unsigned int temp_select_image(std::string file_path) {
	int height, width, channels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* image_bytes = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
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
	return texture_id;
}

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
	
	temp_select_image("orange.png");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.05f, -2.5f));;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);


	Shader shader_file = read_shader("Shaders/Shader.shdr");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	program.bind();
	program.define_uniform("texture_slot");
	GLCall(glUniform1i(program.uniforms["texture_slot"], 0));
	program.update_uniformf("u_color", -0.3f, -0.3f, -0.3f, 1.0f);

	program.define_uniform("model");
	GLCall(glUniformMatrix4fv(program.uniforms["model"], 1, GL_FALSE, glm::value_ptr(model)));
	program.define_uniform("view");
	GLCall(glUniformMatrix4fv(program.uniforms["view"], 1, GL_FALSE, glm::value_ptr(view)));
	program.define_uniform("projection");
	GLCall(glUniformMatrix4fv(program.uniforms["projection"], 1, GL_FALSE, glm::value_ptr(projection)));

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		
		frame::clear_window();
		view = glm::rotate(view, glm::radians(0.4f), glm::vec3(0.0f, 1.0f, 0.0f));
		GLCall(glUniformMatrix4fv(program.uniforms["view"], 1, GL_FALSE, glm::value_ptr(view)));

		program.bind();

		orange.draw(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
