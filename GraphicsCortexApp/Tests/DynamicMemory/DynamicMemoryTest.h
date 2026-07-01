#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class DynamicMemoryTest : public TestBench {
public:

	void print_all_blocks(DynamicBuffer& buffer, std::vector<void*>& virtual_addresses) {
		std::cout << "--------" << std::endl;
		std::cout << "total: " << buffer.size() << ", free: " << buffer.size_free() << ", used: " << buffer.size_used() << std::endl;
		for (void* address : virtual_addresses)
			std::cout << "offset: " << buffer.get_offset(address) << ", size: " << buffer.get_size(address) << std::endl;
		std::cout << "--------" << std::endl;
	}

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;

		default_init();

		std::vector<void*> addresses;
		DynamicBuffer dynamic_buffer;

		std::cout << "init" << std::endl;
		std::cout << "block0" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024));

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block1" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024 * 32));

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block2" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024 * 1024 * 128));

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block3" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024 * 1024 * 128));

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block1 freed" << std::endl;
		dynamic_buffer.gpu_free(addresses[1]);
		addresses.erase(addresses.begin() + 1);

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block2 freed" << std::endl;
		dynamic_buffer.gpu_free(addresses[1]);
		addresses.erase(addresses.begin() + 1);

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block4" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024 * 1024 * 128));

		print_all_blocks(dynamic_buffer, addresses);

		std::cout << "block5" << std::endl;
		addresses.push_back(dynamic_buffer.gpu_malloc(1024 * 1024 * 128));

		print_all_blocks(dynamic_buffer, addresses);

	}
};