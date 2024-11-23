#pragma once

#include "NTree.h"
#include <queue>
#include <iostream>

template<typename key_type, typename value_type>
inline NTree<key_type, value_type>::Node::Node(key_type key, value_type value) :
	key(key), value(value) { }


template<typename key_type, typename value_type>
inline size_t NTree<key_type, value_type>::get_children_count(key_type key) {
	return _find_key(key)->children.size();
}

template<typename key_type, typename value_type>
inline bool NTree<key_type, value_type>::does_exist(key_type key)
{
	return _find_key(key) == nullptr ? false : true;
}

template<typename key_type, typename value_type>
inline void NTree<key_type, value_type>::set_root(key_type self_key, value_type value)
{
	typedef NTree<key_type, value_type>::Node _Node;

	_root = std::make_unique<_Node>(self_key, value);

}

template<typename key_type, typename value_type>
inline bool NTree<key_type, value_type>::set(key_type self_key, value_type value)
{
	typedef NTree<key_type, value_type>::Node _Node;

	_Node* result = _find_key(self_key);
	if (result == nullptr) return false;

	result->value = value;
	return true;
}

template<typename key_type, typename value_type>
inline bool NTree<key_type, value_type>::insert(key_type parent_key, key_type self_key, value_type value)
{
	typedef NTree<key_type, value_type>::Node _Node;

	_Node* parent = _find_key(parent_key);
	if (parent == nullptr) return false;

	for (_Node& child : parent->children) {
		if (child.key == self_key) {
			child.value = value;
			return true;
		}
	}

	parent->children.push_back(_Node(self_key, value));
	return true;
}

template<typename key_type, typename value_type>
inline bool NTree<key_type, value_type>::remove(key_type key)
{
	typedef NTree<key_type, value_type>::Node _Node;

	_Node* result = _find_parent(key);

	if (result == nullptr) return false;
	if (key == _root->key) {
		_root = nullptr;
		return true;
	}

	int i;
	for (i = 0; i < result->children.size(); i++) {
		if (result->children[i].key == key) break;
	}

	if (i >= result->children.size()) {
		std::cout << "WTF" << std::endl;
		return false;
	}

	result->children.erase(result->children.begin() + i);
	return true;
}

template<typename key_type, typename value_type>
inline std::optional<value_type> NTree<key_type, value_type>::get(key_type key)
{
	typedef NTree<key_type, value_type>::Node _Node;

	_Node* result = _find_key(key);

	return result == nullptr ? std::nullopt : result->value;
}

template<typename key_type, typename value_type>
inline std::optional<value_type> NTree<key_type, value_type>::get_root()
{
	if (_root == nullptr) return std::nullopt;
	else return _root->value;
}

template<typename key_type, typename value_type>
inline NTree<key_type, value_type>::Node* NTree<key_type, value_type>::get_children(key_type key)
{
	NTree<key_type, value_type>::Node* result = _find_key(key);
	return result == nullptr ? nullptr : &(*result->children.begin());
}

template<typename key_type, typename value_type>
inline std::optional<key_type> NTree<key_type, value_type>::get_parent(key_type key)
{
	NTree<key_type, value_type>::Node* result = _find_parent(key);
	return result == nullptr ? std::nullopt : result->key;
}

template<typename key_type, typename value_type>
inline NTree<key_type, value_type>::Node* NTree<key_type, value_type>::_find_key(key_type key)
{
	typedef NTree<key_type, value_type>::Node _Node;

	if (_root == nullptr) return nullptr;

	std::queue<std::pair<_Node*, _Node*>> queue;
	queue.push(std::pair(_root.get(), _root.get() + 1));

	while (!queue.empty()) {
		_Node* pointer = queue.front().first;
		_Node* end = queue.front().second;
		
		if (pointer->key == key) return pointer;

		if (pointer->children.size() != 0)
			queue.push(std::pair<_Node*, _Node*>(&(*pointer->children.begin()), &(*pointer->children.end())));
		
		pointer++;

		if (pointer >= end)
			queue.pop();
	}

	return nullptr;
}

template<typename key_type, typename value_type>
inline NTree<key_type, value_type>::Node* NTree<key_type, value_type>::_find_parent(key_type key)
{
	typedef NTree<key_type, value_type>::Node _Node;

	if (_root == nullptr) return nullptr;

	if (key == _root->key) return _root.get();

	std::queue<std::pair<_Node*, _Node*>> queue;
	queue.push(std::pair(_root.get(), _root.get() + 1));

	while (!queue.empty()) {
		_Node* pointer = queue.front().first;
		_Node* end = queue.front().second;

		for (_Node& child : pointer->children)
			if (child.key == key) return pointer;

		if (pointer->children.size() != 0)
			queue.push(std::pair<_Node*, _Node*>(&(*pointer->children.begin()), &(*pointer->children.end())));

		pointer++;

		if (pointer >= end)
			queue.pop();
	}

	return nullptr;
}

