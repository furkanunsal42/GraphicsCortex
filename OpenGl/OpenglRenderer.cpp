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
#include "API\Default_Assets.h"
#include "API\FrameBuffer.h"
#include "API\CubeMap.h"

int main() {
	int width = 1024, height = 768;
	GLFWwindow* window = frame::create_window(width, height, "My Window", 4, 0, true, false, true);
	Scene scene;
	Material material;
	Texture color_texture;
	Texture specular_map;
	Texture normal_map;
	color_texture.queue_image("Images/GoldBlock.png", 4, false);
	//color_texture.queue_image("Images/Bricks/brickcolor.jpg", 4, false);
	//specular_map.queue_image("Images/Bricks/brickreflection.jpg", 4, false);
	//normal_map.queue_image("Images/Bricks/bricknormal.png", 3, false);
	//color_texture.queue_image("Images/StoneTiles/tiles_color.jpg", 4, true);
	//specular_map.queue_image("Images/StoneTiles/tiles_specular.jpg", 4, true);
	//normal_map.queue_image("Images/StoneTiles/tiles_normal.jpg", 3, true);
	
	bool compression = false;
	color_texture.compress_image = compression;
	specular_map.compress_image = compression;
	normal_map.compress_image = compression;
	
	material.color_map = &color_texture;
	//material.specular_map = &specular_map;
	//material.normal_map = &normal_map;
	
	material.bind();

	//Shader normal_shader("Shaders/Solid.vert", "Shaders/solid.geom", "Shaders/NormalTest.frag");
	//Program soild_program(normal_shader.vertex_shader, normal_shader.geometry_shader, normal_shader.fragment_shader);

	Program soild_program = default_program::solid_program();
	
	Graphic cube = default_geometry::cube(material, soild_program, glm::vec3(1.0f));
	/*
	Graphic cube = default_geometry::cube(
		material,
		glm::vec2(4, 4),
		std::vector<unsigned int> {2, 2, 3, 2, 2, 0},
		soild_program,
		//glm::vec2((float)width / height, 1)
		glm::vec3(1, 1, 1)
	);
	*/
	
	scene.meshes.push_back(&cube);
	
	Camera cam;
	cam.screen_width = (float)width;
	cam.screen_height = (float)height;
	cam.position.z = 1.5f;
	cam.perspective = true;
	scene.camera = &cam;
	
	AmbiantLight ambiant(glm::vec3(0.1f, 0.1f, 0.1f), soild_program);
	DirectionalLight directional(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(0.3f, 0.3f, 0.3f), soild_program);
	PointLight point(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(3.0f, 3.0f, 3.0f), 0.5f, 0.5f, 0.0f, soild_program);
	SpotLight spot(glm::vec3(-2.0f, -1.0f, -2.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.2f, 0.0f, 30, soild_program);
	
	scene.lights.push_back(&ambiant);
	scene.lights.push_back(&directional);
	//scene.lights.push_back(&point);
	//scene.lights.push_back(&spot);

	Program framebuffer_program = default_program::framebuffer_program();
	FrameBuffer frame_buffer(width, height, frame::multisample);
	frame_buffer.program = &framebuffer_program;
	scene.frame_buffer = &frame_buffer;
	
	Program cubemap_program = default_program::cubemap_program();
	CubeMapTexture cube_map;
	cube_map.program = &cubemap_program;
	cube_map.camera = scene.camera;
	/*
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Sky/px.jpg";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Sky/nx.jpg";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Sky/py.jpg";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Sky/ny.jpg";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Sky/pz.jpg";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Sky/nz.jpg";
	*/
	cube_map.face_texture_filepaths[RIGHT] = "Images/CubeMap/Street/px.png";
	cube_map.face_texture_filepaths[LEFT] = "Images/CubeMap/Street/nx.png";
	cube_map.face_texture_filepaths[TOP] = "Images/CubeMap/Street/py.png";
	cube_map.face_texture_filepaths[BOTTOM] = "Images/CubeMap/Street/ny.png";
	cube_map.face_texture_filepaths[FRONT] = "Images/CubeMap/Street/pz.png";
	cube_map.face_texture_filepaths[BACK] = "Images/CubeMap/Street/nz.png";

	cube_map.read_queue(3);
	cube_map.load_queue(true);
	
	float t = 0;
	while (!glfwWindowShouldClose(window)){
		frame_buffer.bind();
		
		glfwPollEvents();
		frame::clear_window(0, 0, 0, 1);
		frame::display_performance(180);
		
		double frame_time = frame::get_interval_ms();
		scene.camera->frame_time_ms = frame_time;
		scene.camera->handle_movements(window);
		
		/*
		if (glfwGetKey(window, GLFW_KEY_E) == 1) {
			frame_buffer.program->update_uniform("edge_detect", 1);
		}
		else {
			frame_buffer.program->update_uniform("edge_detect", 0);
		}
		*/
		
		cube_map.draw();

		//cube.position.x += 0.01f;
		//cube.rotation.y += 0.04f * frame_time;
		t += 0.001f * frame_time;
		point.position.y = 5*glm::cos(t);
		
		cube_map.texture_slot = 13;
		cube_map.bind();
		scene.render(window);
		cube_map.texture_slot = 11;
		
		frame_buffer.unbind();
		frame_buffer.render();

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
