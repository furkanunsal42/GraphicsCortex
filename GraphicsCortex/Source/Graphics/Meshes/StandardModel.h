#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include "glm.hpp"

#include "NTree.h"

class Buffer;

struct SingleModel2 {
public:

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec2> texture_coordinates;
	std::vector<glm::vec4> vertex_colors;
	std::vector<uint32_t> indicies;
	uint32_t primitive;

	std::unique_ptr<Buffer> create_vertex_buffer();
	std::unique_ptr<Buffer> create_normal_buffer();
	std::unique_ptr<Buffer> create_uv_buffer();
	std::unique_ptr<Buffer> create_vertex_color_buffer();
	std::unique_ptr<Buffer> create_index_buffer();
};


class Model2 {
public:

	typedef uint32_t node_name;
	typedef uint32_t submodel_name;

	struct Node {
		node_name name;
		std::vector<submodel_name> submodels;
	};


	class _ProxyNode {
		friend Model2;
	public:
		
		void operator=(const _ProxyNode& other);
		bool operator==(const _ProxyNode& other);

		bool add_submodel(uint32_t submodel_name);
		bool remove_submodel(uint32_t submodel_name);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		std::vector<Node>& childnodes;
		std::vector<submodel_name>& submodels;
	
		std::unique_ptr<Buffer> create_vertex_buffer();
		std::unique_ptr<Buffer> create_normal_buffer();
		std::unique_ptr<Buffer> create_uv_buffer();
		std::unique_ptr<Buffer> create_vertex_color_buffer();
		std::unique_ptr<Buffer> create_index_buffer();

		void clear();

		void load_model();
		void load_model_async();
		//void save_to_disc(const std::string& output_filepath);

	private:
		_ProxyNode(Model2& owner_model, uint32_t name, std::vector<Node>& childnodes, std::vector<submodel_name>& submodels);

		Model2& _owner_model;
		uint32_t _node_name;
	};

	Model2(const std::string& filepath);
	Model2(std::shared_ptr<SingleModel2> single_model);
	Model2() = default;
	~Model2() = default;
	void clear();

	void load_model(const std::string& filepath);
	void load_model_async(const std::string& filepath);

	// submodels
	size_t get_submodel_count();
	bool does_submodel_exist(uint32_t submodel_name);
	bool does_submodel_exist(std::shared_ptr<SingleModel2> submodel);
	uint32_t insert_submodel(std::shared_ptr<SingleModel2> submodel);
	void erase_submodel(uint32_t submodel_name);
	void erase_submodel(std::shared_ptr<SingleModel2> submodel);
	std::shared_ptr<SingleModel2> get_submodel(uint32_t submodel_name);
	uint32_t get_submodel_name(std::shared_ptr<SingleModel2> submodel);
	bool set_submodel(uint32_t submodel_name, std::shared_ptr<SingleModel2> submodel);

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode operator[](uint32_t node_name);
	void create_node(uint32_t node_name);
	void delete_node(uint32_t node_name);

	void save_to_disc(const std::string& output_filepath);

	std::unique_ptr<Buffer> create_vertex_buffer();
	std::unique_ptr<Buffer> create_normal_buffer();
	std::unique_ptr<Buffer> create_uv_buffer();
	std::unique_ptr<Buffer> create_vertex_color_buffer();
	std::unique_ptr<Buffer> create_index_buffer();

private:

	uint32_t _next_submodel_name = 0;
	uint32_t _generate_submodel_name();

	std::unordered_map<node_name, std::pair<Node, std::vector<Node>>> _nodes;
	std::unordered_map<submodel_name, std::shared_ptr<SingleModel2>> _name_to_submodels;
	std::unordered_map<std::shared_ptr<SingleModel2>, submodel_name> _submodels_to_name;
};