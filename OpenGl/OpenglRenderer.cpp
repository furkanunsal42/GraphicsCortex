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
#include "API\Default_Geometries.h"
#include "API\FrameBuffer.h"

int main() {
	
	int width = 640, height = 480;
	GLFWwindow* window = frame::create_window(width, height, "My Window", 0, 1, true, false);

	Texture color_texture;
	color_texture.load_image("Images/Stones/brickcolor.jpg");
	Texture specular_map;
	specular_map.load_image("Images/Stones/brickreflection.jpg");
	Texture normal_map;
	normal_map.load_image("Images/Stones/bricknormal.png", 3);
	//Texture white;
	//white.load_image("Images/full_white.png");
	Material material;
	material.color_map = &color_texture;
	material.specular_map = &specular_map;
	material.normal_map = &normal_map;
	material.bind();
	Shader shader_file("Shaders/SolidVertex.glsl", "Shaders/SolidFragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);

	//Graphic cube = default_geometry::rectangle(material, program, glm::vec3(1.0f));
	Graphic cube = default_geometry::rectangle(
		material,
		glm::vec2(4, 4),
		//std::vector<unsigned int> {2, 2, 3, 2, 2, 0},
		std::vector<unsigned int> {0, 0, 0, 0, 0, 0},
		program,
		glm::vec2((float)width / height, 1)
		);
	
	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.perspective = true;

	AmbiantLight ambiant(glm::vec3(0.1f, 0.1f, 0.1f), program);
	//DirectionalLight directional(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), program);
	PointLight point(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 0.0f, program);
	//SpotLight spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30, program);
	
	Scene scene;
	scene.meshes.push_back(&cube);
	scene.camera = &cam;
	scene.lights.push_back(&ambiant);
	//scene.lights.push_back(&directional);
	scene.lights.push_back(&point);
	//scene.lights.push_back(&spot);

	/*
	unsigned int frame_buffer_id;
	GLCall(glGenFramebuffers(1, &frame_buffer_id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id));

	unsigned int frame_buffer_texture_id;
	GLCall(glGenTextures(1, &frame_buffer_texture_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_id));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer_texture_id, 0));

	unsigned int render_buffer_id;
	GLCall(glGenRenderbuffers(1, &render_buffer_id));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_id));
	*/
	Shader fb_shader("Shaders/FrameBufferVertex.glsl", "Shaders/FrameBufferFragment.glsl");
	Program fb_program(fb_shader.vertex_shader, fb_shader.fragment_shader);
	FrameBuffer frame_buffer(width, height);
	frame_buffer.program = &fb_program;

	float t = 0;
	while (!glfwWindowShouldClose(window)){
		//frame_buffer.bind();
		glfwPollEvents();
		
		frame::display_fps();
		frame::clear_window();

		t += 0.01f;
		//cube.position.x += 0.01f;
		//cube.rotation.y += 0.4f;
		//glm::vec4 point = cube.model_matrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
		//std::cout << "x: " << point.x << " y: " << point.y << " z: " << point.z << " w: " << point.w << std::endl;
		point.position.y = 5*glm::cos(t);
		
		scene.camera->handle_movements(window);
		scene.render();
		
		//frame_buffer.unbind();

		//frame_buffer.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
