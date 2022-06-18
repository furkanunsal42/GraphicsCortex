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

int main() {
	
	GLFWwindow* window = frame::create_window(640, 480, "My Window", 1, true, false);
	
	Texture texture;
	texture.internal_format = GL_RGB;
	texture.format = GL_RGB;
	texture.vertical_flip = true;
	texture.load_image("cobble.png");
	
	Shader shader_file("Shaders/SolidVertex.glsl", "Shaders/SolidFragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);

	Graphic cube = default_geometry::cube(texture, program);
	Graphic light_source = default_geometry::cube(texture, program, 0.3f);

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	Camera cam;
	std::cout << cam.screen_height;
	cam.perspective = true;
	cam.min_distance = 0.1f;
	cam.max_distance = 10.0f;
	cam.screen_width = (float)w;
	cam.screen_height = (float)h;
	cam.fov = 45.0f;
	cam.position.z = 1.5f;

	Scene scene;
	scene.meshes.push_back(&cube);
	scene.meshes.push_back(&light_source);
	scene.camera = &cam;

	/*
	directional lighting
	Shader shader_file("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	
	program.update_uniform("texture_slot", 0);
	program.update_uniform("light_sun", glm::vec3(0.0f, -0.7f, -0.7f));
	program.update_uniform("ambiant_light", glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));
	*/

	/*
	flat color shader
	Shader shader_file("Shaders/FlatColorVertex.glsl", "Shaders/FlatColorFragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	program.update_uniform("color", 0.0f, 0.8f, 0.8f, 1.0f);
	*/

	

	program.update_uniform("texture_slot", 0);

	program.update_uniform("p_light.color", 1.0f, 1.0f, 1.0f);
	glm::vec3 light_position(0.0f, 0.0f, 1.50f);
	program.update_uniform("p_light.position", light_position.x, light_position.y, light_position.z);
	program.update_uniform("p_light.constant_term", 1.0f);
	program.update_uniform("p_light.linear_term", 0.10f);
	program.update_uniform("p_light.exponential_term", 0.10f);

	program.update_uniform("a_light.color", 0.1f, 0.1f, 0.1f);

	program.update_uniform("d_light.direction", 1.0f, -1.0f, 1.0f);
	program.update_uniform("d_light.color", 1.0f, 1.0f, 1.0f);
	//int i = 0;
	
	while (!glfwWindowShouldClose(window)){
		//i++;
		glfwPollEvents();
		frame::display_fps();
		frame::clear_window();
		
		cube.rotation += glm::vec3(0.0f, 0.4f, 0.0f);
		light_source.rotation.x += 0.4f;
		//cube.position.z = 2*std::cos(i/100.0f);
		//cube.update_matrix();

		//light_position.z -= 0.01f;

		scene.camera->handle_movements(window);
		scene.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
