#pragma once
#include "Config.h"

#include <glm.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <functional>
#include <memory>

#include "Model.h"

#include "Buffer.h"
#include "ShaderCompiler.h"
#include "Texture.h"
#include "UnifromQueue.h"

#include "PhysicsObject.h"
#include "Mesh.h"

#include "SharedPtr_Mesh.h"
#include "SharedPtr_Materials.h"
#include "SharedPtr_Program.h"

class Graphic {
private:
	glm::vec3 _last_updated_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat _last_updated_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	uniform_update_queue _uniform_update_queue;
public:

	std::shared_ptr<Mesh> mesh = nullptr;
	std::shared_ptr<Material> material = nullptr;							// temp public, for default solid shader queue
	std::shared_ptr<UnorderedMaterial> unordered_material = nullptr;		// temp public, for default solid shader queue
	std::shared_ptr<Program> renderer = nullptr;

	bool use_unordered_material = false;									// temp public, for default solid shader queue
	
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1, 0, 0, 0);
	glm::mat4 model_matrix;
	
	unsigned int mode = GL_TRIANGLES;

	float cubemap_reflections_strength = 0.0f; // values between 1.0f - 0.0f (<0.1f means false)

	Graphic();
	Graphic(Mesh_s mesh, Material_s material, Program_s renderer);
	Graphic(Mesh_s mesh, UnorderedMaterial_s material, Program_s renderer);
	Graphic(const std::vector<float>& verticies, int data_dim); // legacy
	Graphic(Material_s material, Program_s renderer);
	Graphic(UnorderedMaterial_s material, Program_s renderer);

	void draw(bool show_warnings = true);
	void update_matrix();

	void load_model(Mesh_s mesh);
	void clear_mesh();

	void load_material(UnorderedMaterial_s material);
	void load_material(Material_s material);

	void load_program(Program_s program);

	bool is_uniform_queue_loaded();
	bool is_program_loaded();
	bool is_material_loaded();
	bool is_mesh_loaded();

	std::weak_ptr<Mesh> get_mesh();
	std::weak_ptr<Program> get_program();
	std::weak_ptr<UnorderedMaterial> get_unordered_material();		// temp, they have to be united
	std::weak_ptr<Material> get_material();							// temp, they have to be united

	bool _is_material_ordered();

	const glm::vec3& get_position();
	const glm::quat& get_rotation();
	const glm::mat4& get_model_matrix();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);
	void set_model_matrix(glm::mat4 model_matrix);

	template<typename T, typename... Ts>
	void set_uniform(const std::string& uniform_name, T uniform_value, Ts... uniform_values) {
		change_uniform_update_queue(uniform_update<T>(uniform_name, uniform_value, uniform_values...));
	}

	template<typename T, typename... Ts>
	void set_uniform(const std::string& uniform_name, T* uniform_value_pointer, Ts*... uniform_value_pointers) {
		change_uniform_update_queue(dynamic_uniform_update<T>(uniform_name, uniform_value_pointer, uniform_value_pointers...));
	}
	void set_uniform(const std::string& uniform_name, std::function<void()> update_function);
	
	void remove_uniform(const std::string& uniform_name);
	void remove_uniform(unsigned int uniform_id);
	
	void set_uniform_all(uniform_update_queue& new_update_queue);
	void set_uniform_all(uniform_update_queue&& new_update_queue);

	void update_uniforms();

protected:

	template<typename T>
	void add_uniform_update_queue(uniform_update<T>& uniform_queue) {
		if (!_is_program_loaded) {
			std::cout << "[Opengl Error] Graphic::add_uniform_update_queue() is called but Graphic::program is not initialized" << std::endl;
			return;
		}
		uniform_queue.program = renderer;
		renderer->define_uniform(uniform_queue.uniform_name);
		uniform_queue.uniform_id = renderer->uniforms[uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(uniform_queue);
		_is_uniforms_loaded = true;
	}
	template<typename T>
	void add_uniform_update_queue(uniform_update<T>&& uniform_queue) {
		if (!_is_program_loaded) {
			std::cout << "[Opengl Error] Graphic::add_uniform_update_queue() is called but Graphic::program is not initialized" << std::endl;
			return;
		}
		uniform_queue.program = renderer;
		renderer->define_uniform(uniform_queue.uniform_name);
		uniform_queue.uniform_id = renderer->uniforms[uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(uniform_queue);
		_is_uniforms_loaded = true;
	}

	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>& dynamic_uniform_queue) {
		if (!_is_program_loaded) {
			std::cout << "[Opengl Error] Graphic::add_uniform_update_queue() is called but Graphic::program is not initialized" << std::endl;
			return;
		}
		dynamic_uniform_queue.program = renderer;
		renderer->define_uniform(dynamic_uniform_queue.uniform_name);
		dynamic_uniform_queue.uniform_id = renderer->uniforms[dynamic_uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(dynamic_uniform_queue);
		_is_uniforms_loaded = true;
	}
	template<typename T>
	void add_uniform_update_queue(dynamic_uniform_update<T>&& dynamic_uniform_queue) {
		if (!_is_program_loaded) {
			std::cout << "[Opengl Error] Graphic::add_uniform_update_queue() is called but Graphic::program is not initialized" << std::endl;
			return;
		}
		dynamic_uniform_queue.program = renderer;
		renderer->define_uniform(dynamic_uniform_queue.uniform_name);
		dynamic_uniform_queue.uniform_id = renderer->uniforms[dynamic_uniform_queue.uniform_name];
		_uniform_update_queue.add_uniform_update(dynamic_uniform_queue);
		_is_uniforms_loaded = true;
	}

	void add_uniform_update_queue(std::function<void()> functional_update_queue) {
		_uniform_update_queue.add_uniform_update(functional_update_queue);
		_is_uniforms_loaded = true;
	}

	void remove_uniform_update_queue(const std::string& uniform_name) {
		_uniform_update_queue.remove_uniform_update(uniform_name);
	}

	void remove_uniform_update_queue(unsigned int uniform_id) {
		_uniform_update_queue.remove_uniform_update(uniform_id);
	}

	void set_uniform_update_queue(uniform_update_queue& original) {
		_uniform_update_queue.clear();

		if (_is_program_loaded) {
			_uniform_update_queue.copy(original);
			_uniform_update_queue.link_program(renderer);
			_uniform_update_queue.update_uniform_ids();
		}
		else {
			_uniform_update_queue.copy(original);
		}
		_is_uniforms_loaded = true;
	}

	void set_uniform_update_queue(uniform_update_queue&& original) {
		_uniform_update_queue.clear();

		if (_is_program_loaded) {
			_uniform_update_queue.copy(original);
			_uniform_update_queue.link_program(renderer);
			_uniform_update_queue.update_uniform_ids();
		}
		else {
			_uniform_update_queue.copy(original);
		}
		_is_uniforms_loaded = true;
	}

	template<typename T>
	void change_uniform_update_queue(uniform_update<T>& uniform_queue) {
		remove_uniform_update_queue(uniform_queue.uniform_name);
		add_uniform_update_queue(uniform_queue);
		_is_uniforms_loaded = true;
	}
	template<typename T>
	void change_uniform_update_queue(uniform_update<T>&& uniform_queue) {
		remove_uniform_update_queue(uniform_queue.uniform_name);
		add_uniform_update_queue(uniform_queue);
		_is_uniforms_loaded = true;
	}

	template<typename T>
	void change_uniform_update_queue(dynamic_uniform_update<T>& dynamic_uniform_queue) {
		remove_uniform_update_queue(dynamic_uniform_queue.uniform_name);
		add_uniform_update_queue(dynamic_uniform_queue);
		_is_uniforms_loaded = true;
	}
	template<typename T>
	void change_uniform_update_queue(dynamic_uniform_update<T>&& dynamic_uniform_queue) {
		remove_uniform_update_queue(dynamic_uniform_queue.uniform_name);
		add_uniform_update_queue(dynamic_uniform_queue);
		_is_uniforms_loaded = true;
	}

	void update_uniform_queue();

	bool _is_mesh_loaded = false;
	bool _is_material_loaded = false;
	bool _is_program_loaded = false;
	bool _is_uniforms_loaded = false;
};