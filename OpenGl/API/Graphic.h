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
	Material material;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::mat4 model_matrix;
	Program* renderer;
	unsigned int mode = GL_TRIANGLES;
	Graphic();
	Graphic(const ArrayBuffer& buffer, const IndexBuffer& indicies, const Material& material, Program& renderer);
	Graphic(const std::vector<float>& verticies , int data_dim);
	void draw();
	void update_matrix();
};