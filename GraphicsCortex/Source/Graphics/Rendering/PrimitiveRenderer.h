#pragma once

#include "RenderParameters.h"
#include "Mesh2.h"

class Framebuffer;
class Program;
class Buffer;
class VertexAttributeBuffer;
class Framebuffer;

namespace primitive_renderer {

	void render(
		Framebuffer& framebuffer,
		Program& program,
		VertexAttributeBuffer& vab,
		Buffer& index_buffer,
		PrimitiveType primitive = PrimitiveType::triangle,
		IndexType index_type = IndexType::i_ui32,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t attribute_offset = 0,
		size_t vertex_count = 0,
		size_t index_offset = 0,
		size_t primitive_count = 0,
		size_t instance_count = 1,
		size_t instance_offset = 0
	);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		VertexAttributeBuffer& vab,
		PrimitiveType primitive = PrimitiveType::triangle
	);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		VertexAttributeBuffer& vab,
		Buffer& index_buffer,
		PrimitiveType primitive = PrimitiveType::triangle,
		IndexType index_type = IndexType::i_ui32
	);

	void render(
		Program& program, 
		VertexAttributeBuffer& vab,
		PrimitiveType primitive = PrimitiveType::triangle
	);
	
	void render(
		Program& program, 
		VertexAttributeBuffer& vab, 
		Buffer& index_buffer, 
		PrimitiveType primitive = PrimitiveType::triangle,
		IndexType index_type = IndexType::i_ui32
	);
	
	void render(
		Framebuffer& framebuffer,
		Program& program,
		Mesh2::_ProxyNode& mesh
		);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		Mesh2& mesh
	);

	void render(
		Program& program, 
		Mesh2::_ProxyNode& mesh
	);
	
	void render(
		Program& program, 
		Mesh2& mesh
	);

	void clear(Framebuffer& framebuffer, float red, float green, float blue, float alpha);
	void clear(float red, float green, float blue, float alpha);
}