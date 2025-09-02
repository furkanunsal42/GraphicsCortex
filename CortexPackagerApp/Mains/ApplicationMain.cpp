#include "Package.h"
#include <iostream>

void main(int32_t argc, const char* argv[]) {

	Package p;

	p.add_content("Files\\test1.txt");
	p.add_content("Files\\test2.txt");

	std::string concatenated_file = p._get_concatenated_file();
	p._read_from_concatenated_file(concatenated_file);

	std::cout << p.get_content("Files\\test2.txt") << std::endl;

	std::cin.get();
}