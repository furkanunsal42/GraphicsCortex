#include "PrimitiveRenderer.h"

#include "GL/glew.h"
#include "Debuger.h"

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "VertexAttributeBuffer.h"
#include "FrameBuffer.h"


void primitive_renderer::render(
	Framebuffer& framebuffer, 
	Program& program, 
	VertexAttributeBuffer& vab, 
	Buffer& index_buffer, 
	PrimitiveType primitive, 
	IndexType index_type, 
	const RenderParameters& render_parameters, 
	size_t attribute_offset, 
	size_t vertex_count, 
	size_t index_offset, 
	size_t index_count, 
	size_t instance_count, 
	size_t instance_offset
){
	framebuffer.bind_draw();

	primitive_renderer::render(
		program,
		vab,
		index_buffer,
		primitive,
		index_type,
		render_parameters,
		attribute_offset,
		vertex_count,
		index_offset,
		index_count,
		instance_count,
		instance_offset
	);
}

void primitive_renderer::render(
	Program& program,
	VertexAttributeBuffer& vab,
	Buffer& index_buffer,
	PrimitiveType primitive,
	IndexType index_type,
	const RenderParameters& render_parameters,
	size_t attribute_offset,
	size_t vertex_count,
	size_t index_offset,
	size_t index_count,
	size_t instance_count,
	size_t instance_offset
) {

	int32_t largest_slot = vab.get_largest_active_buffer_slot();
	if (largest_slot == -1) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}

	Buffer& vertex_buffer = *vab.get_vertex_buffer(largest_slot);

	vab.bind();
	program.bind();

	if (vertex_count == 0) {
		int32_t stride = vab.get_attribute_stride(largest_slot);
		vertex_count = vertex_buffer.get_buffer_size_in_bytes() / stride;
	}

	size_t ib_index_count = index_buffer.get_buffer_size_in_bytes() / get_IndexType_bytes_per_index(index_type);

	if (index_count == 0)
		index_count = ib_index_count;

	glDrawElementsInstancedBaseVertexBaseInstance(PrimitiveType_to_GL(primitive), index_count, IndexType_to_GL(index_type), (void*)index_offset, instance_count, attribute_offset, instance_offset);
}


void primitive_renderer::render(
	Framebuffer& framebuffer, 
	Program& program, 
	VertexAttributeBuffer& vab,
	PrimitiveType primitive, 
	const RenderParameters& render_parameters, 
	size_t attribute_offset, 
	size_t vertex_count, 
	size_t instance_count, 
	size_t instance_offset	
) {
	framebuffer.bind_draw();

	render(
		program,
		vab,
		primitive,
		render_parameters,
		attribute_offset,
		vertex_count,
		instance_count,
		instance_offset
	);
}

void primitive_renderer::render(
	Program& program, 
	VertexAttributeBuffer& vab, 
	PrimitiveType primitive, 
	const RenderParameters& render_parameters, 
	size_t attribute_offset, 
	size_t vertex_count, 
	size_t instance_count, 
	size_t instance_offset
) {
	
	int32_t largest_slot = vab.get_largest_active_buffer_slot();
	if (largest_slot == -1) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}

	Buffer& vertex_buffer = *vab.get_vertex_buffer(largest_slot);

	vab.bind();
	program.bind();

	if (vertex_count == 0) {
		int32_t stride = vab.get_attribute_stride(largest_slot);
		vertex_count = vertex_buffer.get_buffer_size_in_bytes() / stride;
	}

	GLCall(glDrawArraysInstancedBaseInstance(PrimitiveType_to_GL(primitive), attribute_offset, vertex_count, instance_count, instance_offset));
}

void primitive_renderer::clear(Framebuffer& framebuffer, float red, float green, float blue, float alpha)
{
	framebuffer.bind_draw();
	primitive_renderer::clear(red, green, blue, alpha);
}

void primitive_renderer::clear(float red, float green, float blue, float alpha)
{
	uint32_t clear_bits = GL_COLOR_BUFFER_BIT;

	int32_t bound_framebuffer_id;
	GLCall(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer_id));
	
	//uint32_t color_attachment_bit   = bound_framebuffer_id == 0 ? GL_STENCIL : GL_COLOR_ATTACHMENT0;
	uint32_t depth_attachment_bit   = bound_framebuffer_id == 0 ? GL_DEPTH : GL_DEPTH_ATTACHMENT;
	uint32_t stencil_attachment_bit = bound_framebuffer_id == 0 ? GL_STENCIL : GL_STENCIL_ATTACHMENT;
	
	int32_t bound_depth_type;
	int32_t bound_stencil_type;

	GLCall(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, depth_attachment_bit, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &bound_depth_type));
	GLCall(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, stencil_attachment_bit, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &bound_stencil_type));
	
	if (bound_depth_type != GL_NONE) 
		clear_bits |= GL_DEPTH_BUFFER_BIT;

	if (bound_stencil_type != GL_NONE)
		clear_bits |= GL_STENCIL_BUFFER_BIT;

	GLCall(glClearColor(red, green, blue, alpha));
	GLCall(glClear(clear_bits));
}
