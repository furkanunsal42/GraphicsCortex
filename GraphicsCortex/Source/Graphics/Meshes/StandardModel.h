#pragma once

#include <vector>
#include <string>
#include "glm.hpp"

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
	~Model2();
	void release();

	void load_model(const std::string& filepath);
	void load_model_async(const std::string& filepath);

	SingleModel2& operator[](uint32_t index);
	SingleModel2& get_submodel(uint32_t index);
	uint32_t get_submodel_count();

	void push_submodel(SingleModel2 submodel);
	void pop_submodel();

	void save_to_disc(const std::string& output_filepath);

private:
	
	std::vector<SingleModel2> _sub_models;

};