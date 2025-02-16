#include "PrimitiveRenderer.h"

#include "GL/glew.h"
#include "Debuger.h"

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "VertexAttributeBuffer.h"
#include "FrameBuffer.h"

void primitive_renderer::render(Program& program, VertexAttributeBuffer& vab, PrimitiveType primitive, size_t first, size_t count)
{
	int32_t largest_slot = vab.get_largest_active_buffer_slot();
	if (largest_slot == -1) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}

	Buffer& vertex_buffer = *vab.get_vertex_buffer(largest_slot);

	vab.bind();
	program.bind();

	if (count == 0) {
		int32_t stride = vab.get_attribute_stride(largest_slot);
		count = vertex_buffer.get_buffer_size_in_bytes() / stride;
	}

	GLCall(glDrawArrays(PrimitiveType_to_GL(primitive), first, count));
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
