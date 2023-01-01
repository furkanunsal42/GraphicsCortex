#pragma once

#include "Config.h"

#include <string>
#include <vector>
#include <any>
#include <iostream>
#include <functional>
#include <memory>

#include "glm.hpp"

#include "ShaderCompiler.h"

template <typename T>
class uniform_update{
private:
public:
	T data1{};// {} are for default initialization of type T
	T data2{};
	T data3{};	
	T data4{};	
	std::string uniform_name;
	int data_amount;

	std::weak_ptr<Program> program;
	unsigned int uniform_id;

	uniform_update(const std::string& name, T data) :
		uniform_name(name), data1(data), data_amount(1) {}

	uniform_update(const std::string& name, T data1, T data2) :
		uniform_name(name), data1(data1), data2(data2), data_amount(2) {}

	uniform_update(const std::string& name, T data1, T data2, T data3) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data_amount(3) {}

	uniform_update(const std::string& name, T data1, T data2, T data3, T data4) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data4(data4), data_amount(4) {}

	void update_uniform();
};

template <typename T>
class dynamic_uniform_update{
private:
public:
	T* data1{};// {} are for default initialization of type T
	T* data2{};
	T* data3{};
	T* data4{};
	std::string uniform_name;
	int data_amount;

	std::weak_ptr<Program> program;
	unsigned int uniform_id;

	dynamic_uniform_update(const std::string& name, T* data) :
		uniform_name(name), data1(data), data_amount(1) {}

	dynamic_uniform_update(const std::string& name, T* data1, T* data2) :
		uniform_name(name), data1(data1), data2(data2), data_amount(2) {}

	dynamic_uniform_update(const std::string& name, T* data1, T* data2, T* data3) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data_amount(3) {}

	dynamic_uniform_update(const std::string& name, T* data1, T* data2, T* data3, T* data4) :
		uniform_name(name), data1(data1), data2(data2), data3(data3), data4(data4), data_amount(4) {}

	void update_uniform();
	// add destructer too, that free's the data queue holds.
};


class uniform_update_queue {
private:
	std::vector<uniform_update<int>> uniform_queue_int;
	std::vector<uniform_update<char>> uniform_queue_char;
	std::vector<uniform_update<bool>> uniform_queue_bool;
	std::vector<uniform_update<float>> uniform_queue_float;
	std::vector<uniform_update<glm::mat4>> uniform_queue_mat4;
	std::vector<uniform_update<glm::mat3>> uniform_queue_mat3;
	std::vector<uniform_update<glm::mat2>> uniform_queue_mat2;

	// -------------------------------------------------------------

	std::vector<dynamic_uniform_update<int>> dynamic_uniform_queue_int;
	std::vector<dynamic_uniform_update<char>> dynamic_uniform_queue_char;
	std::vector<dynamic_uniform_update<bool>> dynamic_uniform_queue_bool;
	std::vector<dynamic_uniform_update<float>> dynamic_uniform_queue_float;
	std::vector<dynamic_uniform_update<glm::mat4>> dynamic_uniform_queue_mat4;
	std::vector<dynamic_uniform_update<glm::mat3>> dynamic_uniform_queue_mat3;
	std::vector<dynamic_uniform_update<glm::mat2>> dynamic_uniform_queue_mat2;

	// --------------------------------------------------------------

	std::vector<std::function<void()>> functional_updates;

public:

	void copy(const uniform_update_queue& original);
	void link_program(std::weak_ptr<Program> program);
	void update_uniform_ids();

	void add_uniform_update(uniform_update<int> uniform_update);
	void add_uniform_update(uniform_update<char> uniform_update);
	void add_uniform_update(uniform_update<bool> uniform_update);
	void add_uniform_update(uniform_update<float> uniform_update);
	void add_uniform_update(uniform_update<glm::mat4> uniform_update);
	void add_uniform_update(uniform_update<glm::mat3> uniform_update);
	void add_uniform_update(uniform_update<glm::mat2> uniform_update);

	void add_uniform_update(dynamic_uniform_update<int> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<char> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<bool> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<float> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<glm::mat4> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<glm::mat3> dynamcic_uniform_update);
	void add_uniform_update(dynamic_uniform_update<glm::mat2> dynamcic_uniform_update);

	void remove_uniform_update(const std::string& uniform_name);
	void remove_uniform_update(unsigned int uniform_id);

	void add_uniform_update(const std::function<void()>& update) {
		functional_updates.push_back(update);
	}
	
	void remove_uniform_update_functional(int index) {
		if (index < functional_updates.size())
			functional_updates.erase(functional_updates.begin() + index);
	}

	void update_uniforms();
};

// new functional design
//class functional_update_queue {
//public:
//	std::vector<std::function<void()>> updates;
//	void add_update(std::function<void()> update) {
//		updates.push_back(update);
//	}
//
//	void remove_update(int index) {
//		if (index < updates.size())
//			updates.erase(updates.begin() + index);
//	}
//
//	void update_uniforms() {
//		for (std::function<void()>& update : updates)
//			update();
//	}
//};