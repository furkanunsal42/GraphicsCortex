#pragma once

#include <vector>
#include <optional>
#include <memory>

template<typename key_type, typename value_type>
class NTree {
public:

	struct Node {
		Node(key_type key, value_type value);
		key_type key;
		value_type value;
		std::vector<Node> children;
	};

	size_t get_children_count(key_type key);
	bool does_exist(key_type key);
	
	void set_root(key_type self_key, value_type value);
	bool set(key_type self_key, value_type value);
	bool insert(key_type parent_key, key_type self_key, value_type value);
	bool remove(key_type key);

	std::optional<value_type> get(key_type key);
	std::optional<value_type> get_root();
	Node* get_children(key_type key);
	std::optional<key_type> get_parent(key_type key);

private:

	Node* _find_key(key_type key);
	Node* _find_parent(key_type key);

	std::unique_ptr<Node> _root = nullptr; 
};

#include "NTreeTemplated.h"

