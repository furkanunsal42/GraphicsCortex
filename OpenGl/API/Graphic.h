#pragma once
#include "Config.h"

#include <glm.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "Model.h"

#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Texture.h"
#include "UnifromQueue.h"

#include "PhysicsObject.h"

class Graphic {
private:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat _last_updated_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	uniform_update_queue _uniform_update_queue;
public:
	Model model;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1, 0, 0, 0);
	glm::mat4 model_matrix;
	Program* renderer = nullptr;
	unsigned int mode = GL_TRIANGLES;

	//PhysicsObject physics_representation;

	Graphic();
	Graphic(Model& model, Material& material, Program& renderer);
	Graphic(Model& model, UnorderedMaterial& material, Program& renderer);
	Graphic(Model&& model, Material& material, Program& renderer);
	Graphic(Model&& model, UnorderedMaterial& material, Program& renderer);
	Graphic(const std::vector<float>& verticies, int data_dim); // legacy
	Graphic(Material& material, Program& renderer);
	Graphic(UnorderedMaterial& material, Program& renderer);

	void draw(bool show_warnings = true);
	void update_matrix();

	void load_model(Model& model);
	void load_model(Model&& model);
	void clear_mesh();

	void load_material(UnorderedMaterial& material);
	void load_material(Material& material);
	void load_program(Program& program);

	glm::vec3 get_position();
	glm::quat get_rotation();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);

	template<typename T>
	void add_uniform_update_queue(uniform_update<T>& uniform_queue) {
		uniform_queue.program = renderer;
		renderer->define_uniform(uniform_queue.uniform_name);
		uniform_queue.uniform_id = renderer->uniforms[uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(uniform_queue);
	}
	template<typename T>
	void add_uniform_update_queue(uniform_update<T>&& uniform_queue) {
		uniform_queue.program = renderer;
		renderer->define_uniform(uniform_queue.uniform_name);
		uniform_queue.uniform_id = renderer->uniforms[uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(uniform_queue);
	}
	
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>& dynamic_uniform_queue) {
		dynamic_uniform_queue.program = renderer;
		renderer->define_uniform(dynamic_uniform_queue.uniform_name);
		dynamic_uniform_queue.uniform_id = renderer->uniforms[dynamic_uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(dynamic_uniform_queue);
	}
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>&& dynamic_uniform_queue) {
		dynamic_uniform_queue.program = renderer;
		renderer->define_uniform(dynamic_uniform_queue.uniform_name);
		dynamic_uniform_queue.uniform_id = renderer->uniforms[dynamic_uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(dynamic_uniform_queue);
	}

	void remove_uniform_update_queue(const std::string& uniform_name) {
		_uniform_update_queue.remove_uniform_update(uniform_name);
	}

	void remove_uniform_update_queue(unsigned int uniform_id) {
		_uniform_update_queue.remove_uniform_update(uniform_id);
	}

	void set_uniform_update_queue(uniform_update_queue& original) {
		_uniform_update_queue.copy(original);
		_uniform_update_queue.link_program(renderer);
		_uniform_update_queue.update_uniform_ids();
	}

	void set_uniform_update_queue(uniform_update_queue&& original) {
		_uniform_update_queue.copy(original);
		_uniform_update_queue.link_program(renderer);
		_uniform_update_queue.update_uniform_ids();
	}

	template<typename T>
	void change_uniform_update_queue(uniform_update<T>& uniform_queue) {
		remove_uniform_update_queue(uniform_queue.uniform_name);
		add_uniform_update_queue(uniform_queue);
	}
	template<typename T>
	void change_uniform_update_queue(uniform_update<T>&& uniform_queue) {
		remove_uniform_update_queue(uniform_queue.uniform_name);
		add_uniform_update_queue(uniform_queue);
	}

	template<typename T>
	void change_uniform_update_queue(dynamic_uniform_update<T>& dynamic_uniform_queue) {
		remove_uniform_update_queue(dynamic_uniform_queue.uniform_name);
		add_uniform_update_queue(dynamic_uniform_queue);
	}
	template<typename T>
	void change_uniform_update_queue(dynamic_uniform_update<T>&& dynamic_uniform_queue) {
		remove_uniform_update_queue(dynamic_uniform_queue.uniform_name);
		add_uniform_update_queue(dynamic_uniform_queue);
	}
	void update_uniform_queue();

	Material* material = nullptr;						// temp public, for default solid shader queue
	UnorderedMaterial* unordered_material = nullptr;	// temp public, for default solid shader queue
	bool use_unordered_material = false;				// temp public, for default solid shader queue
protected:

};

