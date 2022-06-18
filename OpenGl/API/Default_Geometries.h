#pragma once

#include "Graphic.h"

namespace default_geometry{
	Graphic cube(const Texture& texture, const Program& renderer,  float scale = 1) {
	std::vector<float> custom_verticies{
		// verticies									texture			normals	
		-0.5f * scale, -0.5f * scale,  0.5f * scale,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	//forward
		 0.5f * scale, -0.5f * scale,  0.5f * scale,	1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		 0.5f * scale,  0.5f * scale,  0.5f * scale,	1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-0.5f * scale,  0.5f * scale,  0.5f * scale,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f,

		 0.5f * scale, -0.5f * scale,  0.5f * scale,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f,	//right
		 0.5f * scale, -0.5f * scale, -0.5f * scale,	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.5f * scale,  0.5f * scale, -0.5f * scale,	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		 0.5f * scale,  0.5f * scale,  0.5f * scale,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		-0.5f * scale,  0.5f * scale, -0.5f * scale,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	//top
		-0.5f * scale,  0.5f * scale,  0.5f * scale,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.5f * scale,  0.5f * scale,  0.5f * scale,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 0.5f * scale,  0.5f * scale, -0.5f * scale,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,

		 0.5f * scale, -0.5f * scale, -0.5f * scale,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,	//backward
		-0.5f * scale, -0.5f * scale, -0.5f * scale,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-0.5f * scale,  0.5f * scale, -0.5f * scale,	1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		 0.5f * scale,  0.5f * scale, -0.5f * scale,	0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		 -0.5f * scale, -0.5f * scale, -0.5f * scale,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	//left
		 -0.5f * scale, -0.5f * scale,  0.5f * scale,	1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		 -0.5f * scale,  0.5f * scale,  0.5f * scale,	1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		 -0.5f * scale,  0.5f * scale, -0.5f * scale,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,

		 0.5f * scale,  -0.5f * scale,  0.5f * scale,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	//bottom
		 0.5f * scale,  -0.5f * scale, -0.5f * scale,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-0.5f * scale,  -0.5f * scale, -0.5f * scale,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f * scale,  -0.5f * scale,  0.5f * scale,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
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

	return Graphic(array_buffer, index_buffer, texture, renderer);
}
}