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
public:
	T data1;
	T data2;
	T data3;
	T data4;
	std::string uniform_name;
	int data_amount;

	uniform_queue(const std::string& name, int data) :
		uniform_name(name), data1(data), data_amount(1) {}

	uniform_queue(const std::string& name, T data1, T data2) :
		uniform_name(name), data1(data1), data2(data2), data_amount(2) {}

	uniform_queue(const std::string& name, T data1, T data2, T data3) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data_amount(3) {}

	uniform_queue(const std::string& name, T data1, T data2, T data3, T data4) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data4(data4), data_amount(4) {}


};

template <typename T>
class dynamic_uniform_queue {
private:
public:
	T* data1;
	T* data2;
	T* data3;
	T* data4;
	std::string uniform_name;
	int data_amount;

	dynamic_uniform_queue(const std::string& name, T* data) :
		uniform_name(name), data1(data), data_amount(1) {}

	dynamic_uniform_queue(const std::string& name, T* data1, T* data2) :
		uniform_name(name), data1(data1), data2(data2), data_amount(2) {}

	dynamic_uniform_queue(const std::string& name, T* data1, T* data2, T* data3) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data_amount(3) {}

	dynamic_uniform_queue(const std::string& name, T* data1, T* data2, T* data3, T* data4) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data4(data4), data_amount(4) {}

	// add destructer too, that free's the data queue holds.
};

class Graphic {
private:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _last_updated_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<uniform_queue<int>> _uniform_update_queue_int;

	std::vector<dynamic_uniform_queue<int>> _dynamic_uniform_update_queue_int;

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
	void add_uniform_update_queue(uniform_queue<T>* uniform_queue) {
		_uniform_update_queue_int.push_back(*uniform_queue);
	}
	
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_queue<T>* dynamic_uniform_queue) {
		_dynamic_uniform_update_queue_int.push_back(*dynamic_uniform_queue);
	}

	void update_uniform_queue(bool init);
};

