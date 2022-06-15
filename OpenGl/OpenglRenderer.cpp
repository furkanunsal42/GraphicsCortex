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
	
	GLFWwindow* window = frame::create_window(640, 480, "My Window");
	
	glEnable(GL_DEPTH_TEST);
	
	Texture texture;
	texture.vertical_flip = true;
	texture.load_image("orange.png");
	
	Graphic cube = default_geometry::cube(texture);

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	Camera cam;
	std::cout << cam.screen_height;
	cam.screen_width = (float)w;
	cam.screen_height = (float)h;
	cam.fov = 90.0f;
	cam.position.z = 1.5f;

	Scene scene;
	scene.meshes.push_back(&cube);
	scene.camera = &cam;

	Shader shader_file("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	Program program(shader_file.vertex_shader, shader_file.fragment_shader);
	
	program.update_uniform("texture_slot", 0);
	program.update_uniform("light_sun", glm::vec3(0.0f, -0.7f, -0.7f));
	program.update_uniform("ambiant_light", glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();
		frame::display_fps();
		frame::clear_window();
		
		cube.rotation += glm::vec3(0.0f, 0.4f, 0.0f);
		
		scene.camera->handle_movements(window);
		scene.render(program);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
