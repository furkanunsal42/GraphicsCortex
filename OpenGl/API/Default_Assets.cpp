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
		g.load_model(Mesh(array_buffer, index_buffer));
		//g.model.array_buffer= array_buffer;
		//g.model.index_buffer = index_buffer;
		return g;
	}

	Graphic rectangle(Material& material, Program& renderer, glm::vec2 scale) {
		Graphic g = rectangle(scale);
		g.load_material(material);
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

		return Graphic(Mesh(array_buffer, index_buffer), material, renderer);
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
		g.load_model(Mesh(array_buffer, index_buffer));
		return g;
	}

	Graphic cube(Material& material, Program& renderer, glm::vec3 scale){
		Graphic g = cube(scale);
		g.load_material(material);
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
		
		return Graphic(Mesh(array_buffer, index_buffer), material, renderer);
	}
}
