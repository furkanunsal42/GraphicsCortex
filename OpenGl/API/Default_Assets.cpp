#include "Default_Assets.h"


namespace default_geometry {



	Graphic rectangle(glm::vec2 scale) {
		std::vector<float> custom_verticies{
			// verticies							texture			normals	
			-0.5f * scale.x, -0.5f * scale.y,  0,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//forward
			 0.5f * scale.x, -0.5f * scale.y,  0,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			-0.5f * scale.x,  0.5f * scale.y,  0,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		};

		ArrayBuffer array_buffer(custom_verticies);
		array_buffer.push_attribute(3);
		array_buffer.push_attribute(2);
		array_buffer.push_attribute(3);

		std::vector<unsigned int> triangles{
			0, 1, 2, 0, 2, 3,

		};
		IndexBuffer index_buffer(triangles, 3);

		Graphic g;
		g.vertex_buffer = array_buffer;
		g.index_buffer = index_buffer;
		return g;
	}

	Graphic rectangle(Material& material, Program& renderer, glm::vec2 scale) {
		Graphic g = rectangle(scale);
		g.material = &material;
		g.renderer = &renderer;
		return g;
	}

	Graphic rectangle(Material& material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program& renderer, glm::vec2 scale) {
		float unit_width = 1.0f / texture_atlas_dim.x;
		float unit_height = 1.0f / texture_atlas_dim.y;
		std::vector<glm::vec2> texture_locations;
		for (unsigned int i = 0; i < face_texture_locations.size(); i++) {
			glm::vec2 texture_location;
			texture_location.x = face_texture_locations[i] % texture_atlas_dim.x;
			texture_location.y = texture_atlas_dim.y - (face_texture_locations[i] / texture_atlas_dim.x);
			texture_locations.push_back(texture_location);
		}
		/*
		bottom left
		bottom right
		top right
		top left

		0.0f, 0.0f
		1.0f, 0.0f
		1.0f, 1.0f
		0.0f, 1.0f

		unit_width * x,		unit_height * (y_max-y-1)
		unit_width * (x+1), unit_height * (y_max-y-1)
		unit_width * (x+1), unit_height * (y_max-y)
		unit_width * x,		unit_height * (y_max-y)

		0.0f, 0.5f
		0.5f, 0.5f
		0.5f, 1.0f
		0.0f, 1.0f
		*/

		std::vector<float> custom_verticies{
			// verticies							texture																					normals	
			-0.5f * scale.x, -0.5f * scale.y,  0,	unit_width * texture_locations[0].x,	   unit_height * (texture_locations[0].y - 1),	0.0f, 0.0f, 1.0f,	//forward
			 0.5f * scale.x, -0.5f * scale.y,  0,	unit_width * (texture_locations[0].x + 1), unit_height * (texture_locations[0].y - 1),	0.0f, 0.0f, 1.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0,	unit_width * (texture_locations[0].x + 1), unit_height * (texture_locations[0].y),		0.0f, 0.0f, 1.0f,
			-0.5f * scale.x,  0.5f * scale.y,  0,	unit_width * texture_locations[0].x,	   unit_height * (texture_locations[0].y),		0.0f, 0.0f, 1.0f,
		};



		ArrayBuffer array_buffer(custom_verticies);
		array_buffer.push_attribute(3);
		array_buffer.push_attribute(2);
		array_buffer.push_attribute(3);

		std::vector<unsigned int> triangles{
			0, 1, 2, 0, 2, 3,
		};
		IndexBuffer index_buffer(triangles, 3);

		return Graphic(array_buffer, index_buffer, material, renderer);
	}

	Graphic cube(glm::vec3 scale) {
		std::vector<float> custom_verticies{
			// verticies										texture			normals	
			-0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//forward
			 0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
			-0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,

			 0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//right
			 0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

			-0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//top
			-0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,

			 0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//backward
			-0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
			-0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

			 -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//left
			 -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
			 -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
			 -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,

			 0.5f * scale.x,  -0.5f * scale.y,  0.5f * scale.z,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//bottom
			-0.5f * scale.x,  -0.5f * scale.y,  0.5f * scale.z,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
			-0.5f * scale.x,  -0.5f * scale.y, -0.5f * scale.z,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
			 0.5f * scale.x,  -0.5f * scale.y, -0.5f * scale.z,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		};

		ArrayBuffer array_buffer(custom_verticies);
		array_buffer.push_attribute(3);
		array_buffer.push_attribute(2);
		array_buffer.push_attribute(3);

		std::vector<unsigned int> triangles{
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,

		};
		IndexBuffer index_buffer(triangles, 3);

		Graphic g;
		g.vertex_buffer = array_buffer;
		g.index_buffer = index_buffer;
		return g;
	}

	Graphic cube(Material& material, Program& renderer, glm::vec3 scale){
		Graphic g = cube(scale);
		g.material = &material;
		g.renderer = &renderer;
		return g;
	}

	Graphic cube(Material& material, glm::ivec2 texture_atlas_dim, const std::vector<unsigned int>& face_texture_locations, Program& renderer, glm::vec3 scale) {
		float unit_width = 1.0f / texture_atlas_dim.x;
		float unit_height = 1.0f / texture_atlas_dim.y;
		std::vector<glm::vec2> texture_locations;
		for (unsigned int i = 0; i < face_texture_locations.size(); i++) {
			glm::vec2 texture_location;
			texture_location.x = face_texture_locations[i] % texture_atlas_dim.x;
			texture_location.y = texture_atlas_dim.y - (face_texture_locations[i] / texture_atlas_dim.x);
			texture_locations.push_back(texture_location);
		}
		/*
		bottom left
		bottom right
		top right
		top left

		0.0f, 0.0f
		1.0f, 0.0f
		1.0f, 1.0f
		0.0f, 1.0f

		unit_width * x,		unit_height * (y_max-y-1)
		unit_width * (x+1), unit_height * (y_max-y-1)
		unit_width * (x+1), unit_height * (y_max-y)
		unit_width * x,		unit_height * (y_max-y)

		0.0f, 0.5f
		0.5f, 0.5f
		0.5f, 1.0f
		0.0f, 1.0f
		*/

		std::vector<float> custom_verticies{
			// verticies										texture																						normals	
			-0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[0].x,	   unit_height * (texture_locations[0].y - 1),		0.0f, 0.0f, 1.0f,	//forward
			 0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[0].x + 1), unit_height * (texture_locations[0].y - 1),		0.0f, 0.0f, 1.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[0].x + 1), unit_height * (texture_locations[0].y),			0.0f, 0.0f, 1.0f,
			-0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[0].x,	   unit_height * (texture_locations[0].y),			0.0f, 0.0f, 1.0f,

			 0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[1].x,	   unit_height * (texture_locations[1].y - 1),		1.0f, 0.0f, 0.0f,	//right
			 0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[1].x + 1), unit_height * (texture_locations[1].y - 1),		1.0f, 0.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[1].x + 1), unit_height * (texture_locations[1].y),			1.0f, 0.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[1].x,	   unit_height * (texture_locations[1].y),			1.0f, 0.0f, 0.0f,

			-0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[2].x,	   unit_height * (texture_locations[2].y - 1),		0.0f, 1.0f, 0.0f,	//top
			-0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[2].x + 1), unit_height * (texture_locations[2].y - 1),		0.0f, 1.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[2].x + 1), unit_height * (texture_locations[2].y),			0.0f, 1.0f, 0.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[2].x,	   unit_height * (texture_locations[2].y),			0.0f, 1.0f, 0.0f,

			 0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[3].x,	   unit_height * (texture_locations[3].y - 1),		0.0f, 0.0f, -1.0f,	//backward
			-0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[3].x + 1), unit_height * (texture_locations[3].y - 1),		0.0f, 0.0f, -1.0f,
			-0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[3].x + 1), unit_height * (texture_locations[3].y),			0.0f, 0.0f, -1.0f,
			 0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[3].x,	   unit_height * (texture_locations[3].y),			0.0f, 0.0f, -1.0f,

			 -0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[4].x,	   unit_height * (texture_locations[4].y - 1),		-1.0f, 0.0f, 0.0f,	//left
			 -0.5f * scale.x, -0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[4].x + 1), unit_height * (texture_locations[4].y - 1),		-1.0f, 0.0f, 0.0f,
			 -0.5f * scale.x,  0.5f * scale.y,  0.5f * scale.z,	unit_width * (texture_locations[4].x + 1), unit_height * (texture_locations[4].y),			-1.0f, 0.0f, 0.0f,
			 -0.5f * scale.x,  0.5f * scale.y, -0.5f * scale.z,	unit_width * texture_locations[4].x,	   unit_height * (texture_locations[4].y),			-1.0f, 0.0f, 0.0f,

			 0.5f * scale.x,  -0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[5].x,	   unit_height * (texture_locations[5].y - 1),		0.0f, -1.0f, 0.0f,	//bottom
			-0.5f * scale.x,  -0.5f * scale.y,  0.5f * scale.z,	unit_width * texture_locations[5].x,	   unit_height * (texture_locations[5].y),			0.0f, -1.0f, 0.0f,
			-0.5f * scale.x,  -0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[5].x + 1), unit_height * (texture_locations[5].y),			0.0f, -1.0f, 0.0f,
			 0.5f * scale.x,  -0.5f * scale.y, -0.5f * scale.z,	unit_width * (texture_locations[5].x + 1), unit_height * (texture_locations[5].y - 1),		0.0f, -1.0f, 0.0f,
		};

		ArrayBuffer array_buffer(custom_verticies);
		array_buffer.push_attribute(3);
		array_buffer.push_attribute(2);
		array_buffer.push_attribute(3);

		std::vector<unsigned int> triangles{
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,

		};
		IndexBuffer index_buffer(triangles, 3);

		return Graphic(array_buffer, index_buffer, material, renderer);
	}
}

namespace default_program {
	Program solid_program() {
		Shader default_shader("Shaders/Solid.vert", "Shaders/Solid.geom", "Shaders/Solid.frag");
		Program solid_program(default_shader.vertex_shader, default_shader.geometry_shader, default_shader.fragment_shader);
		return solid_program;
	}

	uniform_update_queue solid_default_uniform_queue(Scene& scene, Graphic& mesh) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &mesh.model_matrix));
		queue.add_uniform_update(uniform_update<int>("cube_map", 13));
		queue.add_uniform_update(uniform_update<int>("use_cube_map_reflection", 1));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &scene.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &scene.camera->projection_matrix));
		queue.add_uniform_update(dynamic_uniform_update<float>("camera_coords", &scene.camera->position.x, &scene.camera->position.y, &scene.camera->position.z));
		queue.add_uniform_update(uniform_update<int>("use_color_map", (int)(mesh.material->color_map != nullptr)));
		queue.add_uniform_update(uniform_update<int>("use_specular_map", (int)(mesh.material->specular_map != nullptr)));
		queue.add_uniform_update(uniform_update<int>("use_normal_map", (int)(mesh.material->normal_map != nullptr)));
		queue.add_uniform_update(dynamic_uniform_update<int>("color_map_slot", &mesh.material->color_map_slot));
		queue.add_uniform_update(dynamic_uniform_update<int>("specular_map_slot", &mesh.material->specular_map_slot));
		queue.add_uniform_update(dynamic_uniform_update<int>("normal_map_slot", &mesh.material->normal_map_slot));
		return queue;
	}

	uniform_update_queue flat_default_uniform_queue(Scene& scene, Graphic& mesh) {
		uniform_update_queue queue;
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("model", &mesh.model_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("view", &scene.camera->view_matrix));
		queue.add_uniform_update(dynamic_uniform_update<glm::mat4>("projection", &scene.camera->projection_matrix));

		queue.add_uniform_update(uniform_update<float>("color", 0.8f, 0.7f, 0.6f, 1.0f));
		return queue;
	}

	uniform_update_queue ambiant_light_default_uniform_queue(Scene& scene, AmbiantLight& ambiant_light) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("a_lights[0].color", &ambiant_light.color.x, &ambiant_light.color.y, &ambiant_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<int>("a_lights_count", &AmbiantLight::count));

		return queue;

	}
	uniform_update_queue directional_light_default_uniform_queue(Scene& scene, DirectionalLight& directional_light) {
		uniform_update_queue queue;
		
		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[0].color", &directional_light.color.x, &directional_light.color.y, &directional_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("d_lights[0].direction", &directional_light.direction.x, &directional_light.direction.y, &directional_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<int>("d_lights_count", &DirectionalLight::count));
		
		return queue;
	}
	uniform_update_queue point_light_default_uniform_queue(Scene& scene, PointLight& point_light) {
		uniform_update_queue queue;
		
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[0].position", &point_light.position.x, &point_light.position.y, &point_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[0].color", &point_light.color.x, &point_light.color.y, &point_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[0].constant_term", &point_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[0].linear_term", &point_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("p_lights[0].exponential_term", &point_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<int>("p_lights_count", &PointLight::count));

		return queue;
	}
	uniform_update_queue spot_light_default_uniform_queue(Scene& scene, SpotLight& spot_light) {
		uniform_update_queue queue;

		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].position", &spot_light.position.x, &spot_light.position.y, &spot_light.position.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].direction", &spot_light.direction.x, &spot_light.direction.y, &spot_light.direction.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].color", &spot_light.color.x, &spot_light.color.y, &spot_light.color.z));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].constant_term", &spot_light.constant_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].linear_term", &spot_light.linear_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].exponential_term", &spot_light.exponential_term));
		queue.add_uniform_update(dynamic_uniform_update<float>("s_lights[0].cos_angle", &spot_light.cos_angle));
		queue.add_uniform_update(dynamic_uniform_update<int>("s_lights_count", &SpotLight::count));

		return queue;
	}

	/*
	DEFAULT UNIFORM UPDATE QUEUE

		program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::MODEL, mesh->model_matrix);
		program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::CUBE_MAP, 13);
		program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_CUBE_MAP_REFLECTION, 1);

		if (init) {
			if (cam != nullptr) {
				program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::VIEW, cam->view_matrix);
				program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::PROJECTION, cam->projection_matrix);
				cam->update_uniforms(*(program));
			}
		}

		if ((mesh->renderer != nullptr) && (mesh->material != nullptr)) {

			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_COLOR_MAP, (int)(mesh->material->color_map != nullptr));
			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_SPECULAR_MAP, (int)(mesh->material->specular_map != nullptr));
			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::USE_NORMAL_MAP, (int)(mesh->material->normal_map != nullptr));
			
			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::COLOR_MAP_SLOT, mesh->material->color_map_slot);
			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::SPECULAR_MAP_SLOT, mesh->material->specular_map_slot);
			program->update_uniform(default_program::SOLID_UNIFORM_SHORTCUTS::NORMAL_MAP_SLOT, mesh->material->normal_map_slot);
	*/

	Program flatcolor_program() {
		Shader default_shader("Shaders/FlatColor.vert", "Shaders/FlatColor.frag");
		Program flatcolor(default_shader.vertex_shader, default_shader.fragment_shader);
		return flatcolor;
	}

	Program framebuffer_program() {
		Shader default_shader("Shaders/FrameBuffer.vert", "Shaders/FrameBuffer.frag");
		Program framebuffer (default_shader.vertex_shader, default_shader.fragment_shader);
		return framebuffer;
	}
	Program cubemap_program() {
		Shader default_shader("Shaders/CubeMap.vert", "Shaders/CubeMap.frag");
		Program cubemap(default_shader.vertex_shader, default_shader.fragment_shader);
		return cubemap;
	}

}