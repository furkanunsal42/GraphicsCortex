#pragma once
#include "Config.h"

#include <glm.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Texture.h"
#include "UnifromQueue.h"

class Graphic {
private:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _last_updated_rotation = glm::vec3(0.0f, 0.0f, 0.0f);


public:
	uniform_update_queue uniform_update_queue;

	static Assimp::Importer asset_loader;
	ArrayBuffer vertex_buffer;
	IndexBuffer index_buffer;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::mat4 model_matrix;
	Program* renderer = nullptr;
	Material* material = nullptr;
	unsigned int mode = GL_TRIANGLES;
	Graphic();
	Graphic(const ArrayBuffer& buffer, const IndexBuffer& indicies, Material& material, Program& renderer);
	Graphic(const std::vector<float>& verticies , int data_dim);
	void draw(bool show_warnings = true, bool _ignore_default_uniforms = false);
	void update_matrix();

	void load_model(const std::string& file_path);
	
	template<typename T>
	void add_uniform_update_queue(uniform_update<T>* uniform_queue) {
		uniform_update_queue.add_uniform_update(*uniform_queue);
	}
	
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>* dynamic_uniform_queue) {
		uniform_update_queue.add_uniform_update(*dynamic_uniform_queue);
	}

	void update_uniform_queue(bool init);
};

