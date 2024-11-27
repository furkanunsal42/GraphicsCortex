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
	friend std::hash<SingleModel2>;
public:

	bool operator==(const SingleModel2& other) const;
	void operator=(const SingleModel2& other);
	SingleModel2(const SingleModel2& other);

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec2> texture_coordinates;
	std::vector<glm::vec4> vertex_colors;
	std::vector<uint32_t> indicies;
	uint32_t primitive;

	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset, size_t vertex_count);
	std::unique_ptr<Buffer> create_vertex_buffer(size_t vertex_offset = 0);
	std::unique_ptr<Buffer> create_normal_buffer();
	std::unique_ptr<Buffer> create_uv_buffer();
	std::unique_ptr<Buffer> create_vertex_color_buffer();
	std::unique_ptr<Buffer> create_index_buffer();

	const size_t id = s_generate_next_id();
private:
	static size_t s_next_id;
	static size_t s_generate_next_id();
};

template<>
struct std::hash<SingleModel2> {
	size_t operator()(const SingleModel2& object) const;
};


class Model2 {
public:

	struct Node {
	public:
		uint32_t name;
		std::vector<size_t> submodels;
		std::vector<size_t> childnodes;

		size_t id;
	private:
		static size_t s_next_id;
		static size_t s_genearte_next_id();
	};


	struct _ProxyNode {
		friend Model2;
	public:
		
		void operator=(const _ProxyNode& other);

		bool add_submodel(uint32_t submodel_name);
		bool remove_submodel(uint32_t submodel_name);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		std::vector<node_name>& childnodes;
		std::vector<submodel_name>& submodels;
	
		//std::unique_ptr<Buffer> create_vertex_buffer();
		//std::unique_ptr<Buffer> create_normal_buffer();
		//std::unique_ptr<Buffer> create_uv_buffer();
		//std::unique_ptr<Buffer> create_vertex_color_buffer();
		//std::unique_ptr<Buffer> create_index_buffer();

		void clear();

		//void load_model();
		//void load_model_async();
		//void save_to_disc(const std::string& output_filepath);

	private:
		_ProxyNode(Model2& owner_model, uint32_t name, std::vector<node_name>& childnodes, std::vector<submodel_name>& submodels);

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
	bool does_submodel_exist(size_t submodel_id);
	bool does_submodel_exist(const SingleModel2& submodel);
	void insert_submodel(std::shared_ptr<SingleModel2> submodel);
	void insert_submodel(SingleModel2&& submodel);
	void erase_submodel(size_t submodel_id);
	void erase_submodel(const SingleModel2& submodel);
	std::shared_ptr<SingleModel2> get_submodel(uint32_t submodel_name);

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode operator[](uint32_t node_name);
	void create_node(uint32_t node_name);
	void delete_node(uint32_t node_name);

	//void save_to_disc(const std::string& output_filepath);

	//std::unique_ptr<Buffer> create_vertex_buffer();
	//std::unique_ptr<Buffer> create_normal_buffer();
	//std::unique_ptr<Buffer> create_uv_buffer();
	//std::unique_ptr<Buffer> create_vertex_color_buffer();
	//std::unique_ptr<Buffer> create_index_buffer();

private:

	uint32_t _next_submodel_name = 0;
	uint32_t _generate_submodel_name();

	std::unordered_map<size_t, std::shared_ptr<SingleModel2>> _id_to_singlemodel;
	std::unordered_map<uint32_t, Node> _name_to_nodes;
	std::unordered_map<size_t, uint32_t> _id_to_node_name;

};