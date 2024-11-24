#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <memory>

#include "glm.hpp"

#include "NTree.h"

struct SingleModel2 {
public:

	std::vector<glm::vec3> verticies;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec2> texture_coordinates;
	std::vector<glm::vec4> vertex_colors;
	std::vector<uint32_t> indicies;
	uint32_t primitive;
};


class Model2 {
public:

	Model2(const std::string& filepath);
	Model2(std::shared_ptr<SingleModel2> single_model);
	Model2() = default;
	~Model2() = default;
	void clear();

	void load_model(const std::string& filepath);
	//void load_model_async(const std::string& filepath);

	std::shared_ptr<SingleModel2>& operator[](uint32_t key);
	std::shared_ptr<SingleModel2> get_submodel(uint32_t key);
	
	size_t get_submodel_count(uint32_t parent_key);
	SingleModel2* get_submodels(uint32_t parent_key);
	NTree<uint32_t, std::shared_ptr<SingleModel2>>::Node* get_children(uint32_t parent_key);

	uint32_t insert_submodel(uint32_t parent_key, std::shared_ptr<SingleModel2> submodel);
	bool erase_submodel(uint32_t key);

	bool set_submodel(uint32_t key, std::shared_ptr<SingleModel2> submodel);
	std::shared_ptr<SingleModel2> get_submodel(uint32_t key);

	//void save_to_disc(const std::string& output_filepath);

	NTree<uint32_t, std::shared_ptr<SingleModel2>> sub_model_tree;
	
private:

	uint32_t _next_key = 1;
	uint32_t _generate_next_key();
};