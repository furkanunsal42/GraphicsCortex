#pragma once

#include "RenderParameters.h"
#include "Mesh.h"
#include <vec2.hpp>

class Framebuffer;
class Program;
class Buffer;
class VertexAttributeBuffer;
class Framebuffer;

namespace primitive_renderer {

	glm::ivec2 get_viewport_position();
	glm::ivec2 get_viewport_size();
	glm::ivec4 get_viewport_position_size();
	void set_viewport_position(glm::ivec2 position);
	void set_viewport_size(glm::ivec2 viewport);
	void set_viewport(glm::ivec2 position, glm::ivec2 size);
	void set_viewport(glm::ivec4 position_size);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		VertexAttributeBuffer& vab,
		Buffer& index_buffer,
		PrimitiveType primitive = PrimitiveType::triangle,
		IndexType index_type = IndexType::i_ui32,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t attribute_offset = 0,
		size_t index_offset = 0,
		size_t index_count = 0,
		size_t instance_count = 1,
		size_t instance_offset = 0
	);

	void render(
		Program& program,
		VertexAttributeBuffer& vab,
		Buffer& index_buffer,
		PrimitiveType primitive = PrimitiveType::triangle,
		IndexType index_type = IndexType::i_ui32,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t attribute_offset = 0,
		size_t index_offset = 0,
		size_t index_count = 0,
		size_t instance_count = 1,
		size_t instance_offset = 0
	);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		VertexAttributeBuffer& vab,
		PrimitiveType primitive = PrimitiveType::triangle,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t attribute_offset = 0,
		size_t vertex_count = 0,
		size_t instance_count = 1,
		size_t instance_offset = 0
	);

	void render(
		Program& program,
		VertexAttributeBuffer& vab,
		PrimitiveType primitive = PrimitiveType::triangle,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t attribute_offset = 0,
		size_t vertex_count = 0,
		size_t instance_count = 1,
		size_t instance_offset = 0
	);

	void render(
		Framebuffer& framebuffer,
		Program& program,
		Mesh::SingleMesh& single_mesh,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t instance_count = 1,
		size_t instance_offset = 0
		);

	void render(
		Program& program,
		Mesh::SingleMesh& single_mesh,
		const RenderParameters& render_parameters = RenderParameters(),
		size_t instance_count = 1,
		size_t instance_offset = 0
	);
	
	void clear(Framebuffer& framebuffer, float red, float green, float blue, float alpha);
	void clear(float red = 1, float green = 0, float blue = 1, float alpha = 1);
}