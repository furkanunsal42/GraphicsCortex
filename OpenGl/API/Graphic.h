#pragma once
#include <glm.hpp>
#include <vector>
#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Texture.h"

class Graphic {
public:
	ArrayBuffer vertex_buffer;
	IndexBuffer index_buffer;
	Texture texture;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::mat4 model_matrix;
	unsigned int mode = GL_TRIANGLES;
	Graphic();
	Graphic(ArrayBuffer buffer, IndexBuffer indicies, Texture texture);
	Graphic(std::vector<float> verticies, int data_dim);
	void draw();
	void draw(Program program);
	void update_matrix();
};