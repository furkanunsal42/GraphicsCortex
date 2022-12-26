#include "UnifromQueue.h"

#include <iostream>


template <typename T>
void uniform_update<T>::update_uniform() {
	if (data_amount == 1)
		program->update_uniform((unsigned int)uniform_id, data1);
	else if (data_amount == 2)
		program->update_uniform((unsigned int)uniform_id, data1, data2);
	else if (data_amount == 3)
		program->update_uniform((unsigned int)uniform_id, data1, data2, data3);
	else if (data_amount == 4)
		program->update_uniform((unsigned int)uniform_id, data1, data2, data3, data4);
	else {
		std::cout << "[Opengl Error] uniform_update.update_uniform(Program program) is called but uniform_update.data_amount is not between 1 and 4. \n";
		return;
	}
}


template<typename T>
void dynamic_uniform_update<T>::update_uniform() {
	if (data_amount == 1)
		program->update_uniform((unsigned int)uniform_id, *data1);
	else if (data_amount == 2)
		program->update_uniform((unsigned int)uniform_id, *data1, *data2);
	else if (data_amount == 3)
		program->update_uniform((unsigned int)uniform_id, *data1, *data2, *data3);
	else if (data_amount == 4)
		program->update_uniform((unsigned int)uniform_id, *data1, *data2, *data3, *data4);
	else {
		std::cout << "[Opengl Error] dynamic_uniform_update.update_uniform(Program program) is called but dynamic_uniform_update.data_amount is not between 1 and 4. \n";
		return;
	}
}

void uniform_update_queue::copy(const uniform_update_queue& original) {
	for (uniform_update<int> update : original.uniform_queue_int)
		add_uniform_update(update);
	for (uniform_update<char> update : original.uniform_queue_char)
		add_uniform_update(update);
	for (uniform_update<bool> update : original.uniform_queue_bool)
		add_uniform_update(update);
	for (uniform_update<float> update : original.uniform_queue_float)
		add_uniform_update(update);
	for (uniform_update<glm::mat4> update : original.uniform_queue_mat4)
		add_uniform_update(update);
	for (uniform_update<glm::mat3> update : original.uniform_queue_mat3)
		add_uniform_update(update);
	for (uniform_update<glm::mat2> update : original.uniform_queue_mat2)
		add_uniform_update(update);

	for (dynamic_uniform_update<int> update : original.dynamic_uniform_queue_int)
		add_uniform_update(update);
	for (dynamic_uniform_update<char> update : original.dynamic_uniform_queue_char)
		add_uniform_update(update);
	for (dynamic_uniform_update<bool> update : original.dynamic_uniform_queue_bool)
		add_uniform_update(update);
	for (dynamic_uniform_update<float> update : original.dynamic_uniform_queue_float)
		add_uniform_update(update);
	for (dynamic_uniform_update<glm::mat4> update : original.dynamic_uniform_queue_mat4)
		add_uniform_update(update);
	for (dynamic_uniform_update<glm::mat3> update : original.dynamic_uniform_queue_mat3)
		add_uniform_update(update);
	for (dynamic_uniform_update<glm::mat2> update : original.dynamic_uniform_queue_mat2)
		add_uniform_update(update);

	for (std::function<void()> update : original.functional_updates)
		add_uniform_update(update);
}

void uniform_update_queue::link_program(Program* program) {
	for (uniform_update<int>& update : uniform_queue_int)
		update.program = program;
	for (uniform_update<char>& update : uniform_queue_char)
		update.program = program;
	for (uniform_update<bool>& update : uniform_queue_bool)
		update.program = program;
	for (uniform_update<float>& update : uniform_queue_float)
		update.program = program;
	for (uniform_update<glm::mat4>& update : uniform_queue_mat4)
		update.program = program;
	for (uniform_update<glm::mat3>& update : uniform_queue_mat3)
		update.program = program;
	for (uniform_update<glm::mat2>& update : uniform_queue_mat2)
		update.program = program;

	for (dynamic_uniform_update<int>& update : dynamic_uniform_queue_int)
		update.program = program;
	for (dynamic_uniform_update<char>& update : dynamic_uniform_queue_char)
		update.program = program;
	for (dynamic_uniform_update<bool>& update : dynamic_uniform_queue_bool)
		update.program = program;
	for (dynamic_uniform_update<float>& update : dynamic_uniform_queue_float)
		update.program = program;
	for (dynamic_uniform_update<glm::mat4>& update : dynamic_uniform_queue_mat4)
		update.program = program;
	for (dynamic_uniform_update<glm::mat3>& update : dynamic_uniform_queue_mat3)
		update.program = program;
	for (dynamic_uniform_update<glm::mat2>& update : dynamic_uniform_queue_mat2)
		update.program = program;
}

void uniform_update_queue::update_uniform_ids() {
	for (uniform_update<int>& update : uniform_queue_int) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (uniform_update<char>& update : uniform_queue_char) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (uniform_update<bool>& update : uniform_queue_bool) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for(uniform_update<float>&update : uniform_queue_float) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (uniform_update<glm::mat4>& update : uniform_queue_mat4) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (uniform_update<glm::mat3>& update : uniform_queue_mat3) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (uniform_update<glm::mat2>& update : uniform_queue_mat2) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}

	for (dynamic_uniform_update<int>& update : dynamic_uniform_queue_int) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<char>& update : dynamic_uniform_queue_char) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<bool>& update : dynamic_uniform_queue_bool) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<float>& update : dynamic_uniform_queue_float) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<glm::mat4>& update : dynamic_uniform_queue_mat4) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<glm::mat3>& update : dynamic_uniform_queue_mat3) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}
	for (dynamic_uniform_update<glm::mat2>& update : dynamic_uniform_queue_mat2) {
		update.program->define_uniform(update.uniform_name);
		update.uniform_id = update.program->uniforms[update.uniform_name];
	}

}

void uniform_update_queue::add_uniform_update(uniform_update<int> uniform_update) {
	uniform_queue_int.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<char> uniform_update) {
	uniform_queue_char.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<bool> uniform_update) {
	uniform_queue_bool.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<float> uniform_update) {
	uniform_queue_float.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<glm::mat4> uniform_update) {
	uniform_queue_mat4.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<glm::mat3> uniform_update) {
	uniform_queue_mat3.push_back(uniform_update);
}
void uniform_update_queue::add_uniform_update(uniform_update<glm::mat2> uniform_update) {
	uniform_queue_mat2.push_back(uniform_update);
}

// ------------------------------------------------------------------------------------------

void uniform_update_queue::add_uniform_update(dynamic_uniform_update<int> dynamic_uniform_update) {
	dynamic_uniform_queue_int.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<char> dynamic_uniform_update) {
	dynamic_uniform_queue_char.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<bool> dynamic_uniform_update) {
	dynamic_uniform_queue_bool.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<float> dynamic_uniform_update) {
	dynamic_uniform_queue_float.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<glm::mat4> dynamic_uniform_update) {
	dynamic_uniform_queue_mat4.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<glm::mat3> dynamic_uniform_update) {
	dynamic_uniform_queue_mat3.push_back(dynamic_uniform_update);
}
void uniform_update_queue::add_uniform_update(dynamic_uniform_update<glm::mat2> dynamic_uniform_update) {
	dynamic_uniform_queue_mat2.push_back(dynamic_uniform_update);
}

// ------------------------------------------------------------------------------------------

void uniform_update_queue::remove_uniform_update(const std::string& uniform_name) {
	for (int i = 0; i < uniform_queue_int.size(); i++) {
		uniform_update<int>& update = uniform_queue_int[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_int.erase(uniform_queue_int.begin() + i);
	}
	for (int i = 0; i < uniform_queue_char.size(); i++) {
		uniform_update<char>& update = uniform_queue_char[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_char.erase(uniform_queue_char.begin() + i);
	}
	for (int i = 0; i < uniform_queue_bool.size(); i++) {
		uniform_update<bool>& update = uniform_queue_bool[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_bool.erase(uniform_queue_bool.begin() + i);
	}
	for (int i = 0; i < uniform_queue_float.size(); i++) {
		uniform_update<float>& update = uniform_queue_float[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_float.erase(uniform_queue_float.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat4.size(); i++) {
		uniform_update<glm::mat4>& update = uniform_queue_mat4[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_mat4.erase(uniform_queue_mat4.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat3.size(); i++) {
		uniform_update<glm::mat3>& update = uniform_queue_mat3[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_mat3.erase(uniform_queue_mat3.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat2.size(); i++) {
		uniform_update<glm::mat2>& update = uniform_queue_mat2[i];
		if (update.uniform_name == uniform_name)
			uniform_queue_mat2.erase(uniform_queue_mat2.begin() + i);
	}

	for (int i = 0; i < dynamic_uniform_queue_int.size(); i++) {
		dynamic_uniform_update<int>& update = dynamic_uniform_queue_int[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_int.erase(dynamic_uniform_queue_int.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_char.size(); i++) {
		dynamic_uniform_update<char>& update = dynamic_uniform_queue_char[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_char.erase(dynamic_uniform_queue_char.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_bool.size(); i++) {
		dynamic_uniform_update<bool>& update = dynamic_uniform_queue_bool[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_bool.erase(dynamic_uniform_queue_bool.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_float.size(); i++) {
		dynamic_uniform_update<float>& update = dynamic_uniform_queue_float[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_float.erase(dynamic_uniform_queue_float.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat4.size(); i++) {
		dynamic_uniform_update<glm::mat4>& update = dynamic_uniform_queue_mat4[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_mat4.erase(dynamic_uniform_queue_mat4.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat3.size(); i++) {
		dynamic_uniform_update<glm::mat3>& update = dynamic_uniform_queue_mat3[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_mat3.erase(dynamic_uniform_queue_mat3.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat2.size(); i++) {
		dynamic_uniform_update<glm::mat2>& update = dynamic_uniform_queue_mat2[i];
		if (update.uniform_name == uniform_name)
			dynamic_uniform_queue_mat2.erase(dynamic_uniform_queue_mat2.begin() + i);
	}
}

void uniform_update_queue::remove_uniform_update(unsigned int uniform_id) {
	for (int i = 0; i < uniform_queue_int.size(); i++) {
		uniform_update<int>& update = uniform_queue_int[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_int.erase(uniform_queue_int.begin() + i);
	}
	for (int i = 0; i < uniform_queue_char.size(); i++) {
		uniform_update<char>& update = uniform_queue_char[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_char.erase(uniform_queue_char.begin() + i);
	}
	for (int i = 0; i < uniform_queue_bool.size(); i++) {
		uniform_update<bool>& update = uniform_queue_bool[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_bool.erase(uniform_queue_bool.begin() + i);
	}
	for (int i = 0; i < uniform_queue_float.size(); i++) {
		uniform_update<float>& update = uniform_queue_float[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_float.erase(uniform_queue_float.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat4.size(); i++) {
		uniform_update<glm::mat4>& update = uniform_queue_mat4[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_mat4.erase(uniform_queue_mat4.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat3.size(); i++) {
		uniform_update<glm::mat3>& update = uniform_queue_mat3[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_mat3.erase(uniform_queue_mat3.begin() + i);
	}
	for (int i = 0; i < uniform_queue_mat2.size(); i++) {
		uniform_update<glm::mat2>& update = uniform_queue_mat2[i];
		if (update.uniform_id == uniform_id)
			uniform_queue_mat2.erase(uniform_queue_mat2.begin() + i);
	}

	for (int i = 0; i < dynamic_uniform_queue_int.size(); i++) {
		dynamic_uniform_update<int>& update = dynamic_uniform_queue_int[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_int.erase(dynamic_uniform_queue_int.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_char.size(); i++) {
		dynamic_uniform_update<char>& update = dynamic_uniform_queue_char[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_char.erase(dynamic_uniform_queue_char.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_bool.size(); i++) {
		dynamic_uniform_update<bool>& update = dynamic_uniform_queue_bool[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_bool.erase(dynamic_uniform_queue_bool.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_float.size(); i++) {
		dynamic_uniform_update<float>& update = dynamic_uniform_queue_float[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_float.erase(dynamic_uniform_queue_float.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat4.size(); i++) {
		dynamic_uniform_update<glm::mat4>& update = dynamic_uniform_queue_mat4[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_mat4.erase(dynamic_uniform_queue_mat4.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat3.size(); i++) {
		dynamic_uniform_update<glm::mat3>& update = dynamic_uniform_queue_mat3[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_mat3.erase(dynamic_uniform_queue_mat3.begin() + i);
	}
	for (int i = 0; i < dynamic_uniform_queue_mat2.size(); i++) {
		dynamic_uniform_update<glm::mat2>& update = dynamic_uniform_queue_mat2[i];
		if (update.uniform_id == uniform_id)
			dynamic_uniform_queue_mat2.erase(dynamic_uniform_queue_mat2.begin() + i);
	}
}

void uniform_update_queue::update_uniforms() {
	for (uniform_update<int> update : uniform_queue_int)
		update.update_uniform();
	for (uniform_update<char> update : uniform_queue_char)
		update.update_uniform();
	for (uniform_update<bool> update : uniform_queue_bool)
		update.update_uniform();
	for (uniform_update<float> update : uniform_queue_float)
		update.update_uniform();
	for (uniform_update<glm::mat4> update : uniform_queue_mat4)
		update.update_uniform();
	for (uniform_update<glm::mat3> update : uniform_queue_mat3)
		update.update_uniform();
	for (uniform_update<glm::mat2> update : uniform_queue_mat2)
		update.update_uniform();

	for (dynamic_uniform_update<int> update : dynamic_uniform_queue_int)
		update.update_uniform();
	for (dynamic_uniform_update<char> update : dynamic_uniform_queue_char)
		update.update_uniform();
	for (dynamic_uniform_update<bool> update : dynamic_uniform_queue_bool)
		update.update_uniform();
	for (dynamic_uniform_update<float> update : dynamic_uniform_queue_float)
		update.update_uniform();
	for (dynamic_uniform_update<glm::mat4> update : dynamic_uniform_queue_mat4)
		update.update_uniform();
	for (dynamic_uniform_update<glm::mat3> update : dynamic_uniform_queue_mat3)
		update.update_uniform();
	for (dynamic_uniform_update<glm::mat2> update : dynamic_uniform_queue_mat2)
		update.update_uniform();

	for (std::function<void()>& update : functional_updates)
		update();
}
