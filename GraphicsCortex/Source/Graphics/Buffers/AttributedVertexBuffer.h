#pragma once

#include "gl/glew.h"
#include <memory>
#include <unordered_map>

#include "StandardBuffer.h"

class AttributedVertexBuffer {
public:

	void attach_vertex_buffer(int slot, std::shared_ptr<Buffer> vertex_buffer);
	void deattach_vertex_buffer(int slot);
	std::shared_ptr<Buffer> get_vertex_buffer(int slot);

	void attach_index_buffer(std::shared_ptr<Buffer> index_buffer);
	void deattach_index_buffer();
	std::shared_ptr<Buffer> get_index_buffer();

	void enable_attribute(int slot);
	void disable_attribute(int slot);
	bool is_attribute_enabled(int slot);

	int get_attribute_count();
private:

	unsigned int id = 0;

	std::shared_ptr<Buffer> _index_buffer = nullptr;
	std::unordered_map<int, std::shared_ptr<Buffer>> _vertex_buffers;

};