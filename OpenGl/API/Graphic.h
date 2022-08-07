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

template <typename T>
class uniform_queue {
private:
	T data1;
	T data2;
	T data3;
	T data4;
	std::string uniform_name;
public:
	uniform_queue(const std::string& name, T data);
	uniform_queue(const std::string& name, T data1, T data2);
	uniform_queue(const std::string& name, T data1, T data2, T data3);
	uniform_queue(const std::string& name, T data1, T data2, T data3, T data4);
};

template <typename T>
class dynamic_uniform_queue {
private:
	T* data1;
	T* data2;
	T* data3;
	T* data4;
	std::string uniform_name;
public:
	dynamic_uniform_queue(const std::string& name, T* data);
	dynamic_uniform_queue(const std::string& name, T* data1, T* data2);
	dynamic_uniform_queue(const std::string& name, T* data1, T* data2, T* data3);
	dynamic_uniform_queue(const std::string& name, T* data1, T* data2, T* data3, T* data4);
	// add destructer too, that free's the data queue holds.
};

class Graphic {
private:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _last_updated_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<uniform_queue<int>> _uniform_update_queue;

public:
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
	void add_uniform_update_queue(uniform_queue<T> uniform_queue);
	
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_queue<T*> dynamic_uniform_queue);

	void update_uniform_queue(bool init);
};

