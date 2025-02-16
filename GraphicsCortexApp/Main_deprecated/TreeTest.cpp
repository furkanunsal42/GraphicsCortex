#include "GraphicsCortex.h"

#include <string>
#include <iostream>

int main() {

	NTree<int, std::string> tree;

	tree.set_root(0, "hello");
	std::optional<std::string> root_value = tree.get_root();

	tree.insert(0, 32, "haha");
	tree.insert(0, 30, "1234");
	tree.insert(0, 30, "xyz");

	std::cout << root_value.value_or("null") << std::endl;
	std::cout << tree.get_children_count(0) << std::endl;

	tree.remove(30);

	auto* children = tree.get_children(0);
	size_t count = tree.get_children_count(0);

	for (int i = 0; i < count; i++, children++)
		std::cout << children->key << " " << children->value << std::endl;


	std::cin.get();
}