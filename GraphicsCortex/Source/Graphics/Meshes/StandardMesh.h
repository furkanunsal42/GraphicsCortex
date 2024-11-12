#pragma once

#include "Buffer.h"
#include "AttributedVertexBuffer.h"

class Model2;

class Mesh2 {
public:

	enum PrimitiveType {
		point,
		line,
		line_strip,
		line_loop,
		triangle,
		triangle_strip,
		triangle_fan,
		
		line_strip_adjacency,
		line_adjacency,
		triangle_strip_adjacency,
		triangle_adjacency,
		patches,
	};

	enum IndexType {
		ui8,
		ui16,
		ui32,
	};

	Mesh2(const Mesh2& other) = delete;
	Mesh2();
	Mesh2(const Model2& model);
	~Mesh2();

	void clear();

	void load_model(const Model2& model);
	std::shared_ptr<Model2> get_model();

	void set_attributed_vertex_buffer(std::shared_ptr<AttributedVertexBuffer> attributed_vertex_buffer);
	void set_index_buffer(std::shared_ptr<Buffer> index_buffer, IndexType index_type);

	std::shared_ptr<AttributedVertexBuffer> get_attributed_vertex_buffer();
	std::shared_ptr<Buffer> get_index_buffer();
	IndexType get_index_buffer_type();

	uint32_t get_submesh_count();
	void push_submesh_description();
	void pop_submesh_description();

	size_t get_submesh_offset(uint32_t slot);
	size_t get_submesh_size(uint32_t slot);
	PrimitiveType get_submesh_primitive(uint32_t slot);

	void set_submesh_offset(uint32_t slot, size_t offset);
	void set_submesh_size(uint32_t slot, size_t size);
	void set_submesh_primitive(uint32_t slot, PrimitiveType primitive);
	
private:
	
	struct _single_mesh_description {
	public:

		size_t _offset;
		size_t _size;

		Mesh2::PrimitiveType _primitive;
	};

	std::vector<_single_mesh_description> _single_mesh_descriptions;

	std::shared_ptr<AttributedVertexBuffer> _attributed_vertex_buffer;
	std::shared_ptr<Buffer> _index_buffer;


};