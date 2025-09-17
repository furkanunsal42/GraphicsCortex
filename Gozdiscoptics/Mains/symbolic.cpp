
#include <cinttypes>
#include <iostream>
#include <vector>
#include <stdarg.h>

struct Expression;

struct Variable {

	static constexpr uint32_t null_id = 0;

	Variable() :
		id(generate_id()) {
	}
	Variable(const Variable& other) = delete;
	Variable(const Variable&& other) = delete;
	Variable& operator=(const Variable& other) = delete;
	Variable& operator=(Variable&& other) = delete;

	operator Expression() const;
	friend Expression operator+(Variable a, Variable b);

	uint32_t id;

private:

	static uint32_t next_id;
	static uint32_t generate_id() {
		uint32_t id_to_generate = next_id;
		next_id++;
		return id_to_generate;
	}
};

uint32_t Variable::next_id = 0;

enum Operation {
	Identity,
	Add,
};

int32_t get_operation_argument_count(Operation operation) {
	switch (operation) {
	case Identity:	return 1;
	case Add:		return 2;
	}

	std::cout << "[Enum Error] get_operation_argument_count() is called with invalid enum" << std::endl;
	return 0;
}

struct Expression {
	struct Entry {
		Operation operation;
		Entry* right = nullptr;
		Entry* left = nullptr;
		float value = 0;
		uint32_t variable_id = Variable::null_id;
	};

	Entry root_entry;

	float evaluate(const std::vector<std::pair<uint32_t, float>>& variable_value_table) {
		return variable_value_table[0].first;
	}

	float evaluate(Entry entry) {
		if (entry.operation == Identity) {
			if (entry.right != nullptr) {
				std::cout << "[Expression Error] evaluate() is called with invalid expression" << std::endl;
				exit(-1);
			}
			if (
				entry.left != nullptr && entry.variable_id != Variable::null_id &&
				entry.left == nullptr && entry.variable_id == Variable::null_id
				) {
				std::cout << "[Expression Error] evaluate() is called with invalid expression" << std::endl;
				exit(-1);
			}
			return entry.left == nullptr ? evaluate(*entry.left);
		}
		if (entry.operation == Add) {
			if (entry.left == nullptr || entry.right == nullptr) {
				std::cout << "[Expression Error] evaluate() is called with invalid expression" << std::endl;
				exit(-1);
			}
			return evaluate(*entry.left) + evaluate(*entry.right);
		}
	}
};

Variable::operator Expression() const {
	Expression e;
	e.root_entry.operation = Identity;
	e.root_entry.value = 3;
	return e;
}

//Expression operator+(Variable a, Variable b)
//{
//	Expression e;
//	e.variable_id0 = a.id;
//	e.variable_id1 = b.id;
//	e.operation = Add;
//	return e;
//}