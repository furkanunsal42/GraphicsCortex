#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include "glm.hpp"
#include <stdint.h>

#include "IndexBufferEnums.h"
#include "SingleModel.h"

class Buffer;
class Mesh2;

class Model2 {
	friend Mesh2;
public:

	static const uint32_t null_submodel_name = -1;
	static const uint32_t null_node_name = -1;
	static const uint32_t root_node_name = 0;

	struct Node {
		friend Model2;
	public:
		//bool operator==(const Node& other) const;
		Node(uint32_t node_name = null_node_name);

		uint32_t name;
		std::vector<uint32_t> submodels;
		std::vector<uint32_t> childnodes;
		glm::mat4 transform = glm::mat4(1.0f);

	private:
		//uint64_t _childnodes_hash = 0;
	};

	struct _ProxyNode {
		friend Model2;
	public:

		//void operator=(const _ProxyNode& other);
		//bool operator==(const Node& other) const;
		//bool operator==(const _ProxyNode& other) const;

		bool add_submodel(std::shared_ptr<SingleModel2> submodel);
		bool add_submodel(uint32_t submodel_name);
		bool remove_submodel(std::shared_ptr<SingleModel2> submodel);
		bool remove_submodel(uint32_t submodel_name);

		bool add_childnode(uint32_t node_name);
		bool remove_childnode(uint32_t node_name);

		const std::vector<uint32_t>& childnodes;
		const std::vector<uint32_t>& submodels;
		glm::mat4& transform;

		//std::unique_ptr<Buffer> create_vertex_buffer();
		//std::unique_ptr<Buffer> create_normal_buffer();
		//std::unique_ptr<Buffer> create_uv_buffer();
		//std::unique_ptr<Buffer> create_vertex_color_buffer();
		//std::unique_ptr<Buffer> create_index_buffer();

		void clear();

		//void load_model(const std::string& path);
		//void load_model_async(const std::string& path);
		//void save_to_disc(const std::string& output_filepath);

	private:

		std::vector<uint32_t>& _childnodes;
		std::vector<uint32_t>& _submodels;

		_ProxyNode(Model2& owner_model, uint32_t name, std::vector<uint32_t>& childnodes, std::vector<uint32_t>& submodels, glm::mat4& transform);

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
	size_t get_submodel_count() const;
	bool does_submodel_exist(std::shared_ptr<SingleModel2> submodel) const;
	bool does_submodel_exist(uint32_t submodel_name) const;
	uint32_t insert_submodel(std::shared_ptr<SingleModel2> submodel);
	//uint32_t insert_submodel(SingleModel2&& submodel);
	void erase_submodel(std::shared_ptr<SingleModel2> submodel);
	void erase_submodel(uint32_t submodel_name);
	//bool set_submodel(uint32_t submodel_name, std::shared_ptr<SingleModel2> new_submodel);
	//bool set_submodel(uint32_t submodel_name, SingleModel2&& new_submodel);
	uint32_t get_submodel_name(std::shared_ptr<SingleModel2> submodel) const;
	std::shared_ptr<SingleModel2> get_submodel(uint32_t submodel_name);

	void set_index_type(IndexType index_type);
	IndexType get_index_type();

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode operator[](uint32_t node_name);
	_ProxyNode get_node(uint32_t node_name);
	void create_node(uint32_t node_name);
	void delete_node(uint32_t node_name);

	//void save_to_disc(const std::string& output_filepath);

	std::unique_ptr<Buffer> create_vertex_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_normal_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_tangent_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	//std::unique_ptr<Buffer> create_uv0_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	//std::unique_ptr<Buffer> create_uv1_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_uv_merged_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_vertex_color_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_bone_indicies_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_bone_weights_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;
	std::unique_ptr<Buffer> create_index_buffer(size_t buffer_left_padding = 0, size_t buffer_right_padding = 0) const;

private:

	uint32_t _next_submodel_name = 0;
	uint32_t _generate_submodel_name();
	//uint64_t _update_childnode_hahs();
	//uint64_t _update_childnode_hash(uint32_t node_name);

	std::unordered_map<std::shared_ptr<SingleModel2>, uint32_t> _submodel_to_name;
	std::unordered_map<uint32_t, std::shared_ptr<SingleModel2>> _name_to_submodel;
	IndexType _index_buffer_type = IndexType::i_ui32;

	std::unordered_map<uint32_t, Node> _name_to_node;
	//std::unordered_map<size_t, uint32_t> _nodehash_to_node;
};

