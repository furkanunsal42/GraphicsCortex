#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include "glm.hpp"
#include <stdint.h>
#include <filesystem>
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
		Node(uint32_t node_name = null_node_name);

		uint32_t name;
		uint32_t parent;
		std::vector<uint32_t> childs;
		std::vector<uint32_t> submodels;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	struct _ProxyNode {
		friend Model2;
	public:

		bool add_submodel(int32_t submodel);
		bool remove_submodel(int32_t submodel);

		_ProxyNode add_childnode();

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

	Model2(const std::filesystem::path& filepath);
	Model2() = default;
	~Model2() = default;
	
	void clear();
	void load_model(const std::filesystem::path& filepath);
	
	std::vector<SingleModel2> single_models;
	IndexType index_buffer_type = IndexType::i_ui32;

	// nodes
	size_t get_node_count();
	bool does_node_exist(uint32_t node_name);
	_ProxyNode get_node(uint32_t node_name);

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

	uint32_t _generate_submodel_name();
	std::unordered_map<uint32_t, Node> _name_to_node;
};

