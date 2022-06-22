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
	GLFWwindow* window = frame::create_window(width, height, "My Window", 4, 1, true, false);

	Texture color_texture;
	color_texture.load_image("Images/Stones/brickcolor.jpg");
	Texture specular_map;
	specular_map.load_image("Images/Stones/brickreflection.jpg");
	Texture normal_map;
	normal_map.load_image("Images/Stones/bricknormal.png", 3);
	Material material;
	material.color_map = &color_texture;
	material.specular_map = &specular_map;
	//material.normal_map = &normal_map;
	material.bind();
	Shader shader_file("Shaders/SolidVertex.glsl", "Shaders/SolidFragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);

	//Graphic cube = default_geometry::rectangle(material, program, glm::vec3(1.0f));
	Graphic cube = default_geometry::cube(
		material,
		glm::vec2(4, 4),
		std::vector<unsigned int> {2, 2, 3, 2, 2, 0},
		//std::vector<unsigned int> {0, 0, 0, 0, 0, 0},
		program,
		//glm::vec2((float)width / height, 1)
		glm::vec3(1, 1, 1)
		);
	
	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.perspective = true;

	AmbiantLight ambiant(glm::vec3(0.1f, 0.1f, 0.1f), program);
	DirectionalLight directional(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), program);
	PointLight point(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, 0.5f, 0.0f, program);
	SpotLight spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30, program);
	
	Scene scene;
	scene.meshes.push_back(&cube);
	scene.camera = &cam;
	
	scene.lights.push_back(&ambiant);
	scene.lights.push_back(&directional);
	//scene.lights.push_back(&point);
	//scene.lights.push_back(&spot);

	Shader fb_shader("Shaders/FrameBufferVertex.glsl", "Shaders/FrameBufferFragment.glsl");
	Program fb_program(fb_shader.vertex_shader, fb_shader.fragment_shader);
	FrameBuffer frame_buffer(width, height);
	frame_buffer.program = &fb_program;

	float t = 0;
	while (!glfwWindowShouldClose(window)){
		frame_buffer.bind();
		glfwPollEvents();
		
		frame::display_fps();
		frame::clear_window();
		t += 0.01f;
		//cube.position.x += 0.01f;
		cube.rotation.y += 0.1f;
		point.position.y = 5*glm::cos(t);
		
		scene.camera->handle_movements(window);
		scene.render();
		if (glfwGetKey(window, GLFW_KEY_E) == 1) {
			fb_program.update_uniform("edge_detect", 1);
		}
		else {
			fb_program.update_uniform("edge_detect", 0);
		}
		frame_buffer.unbind();
		frame_buffer.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
