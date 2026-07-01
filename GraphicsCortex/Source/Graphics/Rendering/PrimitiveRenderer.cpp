#include "PrimitiveRenderer.h"

#include "GL/glew.h"
#include "Debuger.h"

#include "ShaderCompiler.h"
#include "Mesh.h"
#include "VertexAttributeBuffer.h"
#include "FrameBuffer.h"
#include "Text.h"

glm::ivec2 primitive_renderer::get_viewport_position()
{
	glm::ivec4 old;
	glGetIntegerv(GL_VIEWPORT, (int32_t*)&old);
	return glm::vec2(old.x, old.y);
}

glm::ivec2 primitive_renderer::get_viewport_size()
{
	glm::ivec4 old;
	glGetIntegerv(GL_VIEWPORT, (int32_t*)&old);
	return glm::vec2(old.z, old.a);
}

glm::ivec4 primitive_renderer::get_viewport_position_size()
{
	glm::ivec4 old;
	glGetIntegerv(GL_VIEWPORT, (int32_t*)&old);
	return glm::vec4(old.x, old.y, old.z, old.a);
}

void primitive_renderer::set_viewport_position(glm::ivec2 position)
{
	glm::ivec4 old;
	glGetIntegerv(GL_VIEWPORT, (int32_t*)&old);
	GLCall(glViewport(position.x, position.y, old.z, old.a));
}

void primitive_renderer::set_viewport_size(glm::ivec2 viewport)
{
	glm::ivec4 old;
	glGetIntegerv(GL_VIEWPORT, (int32_t*)&old);
	GLCall(glViewport(old.x, old.y, viewport.x, viewport.y));
}

void primitive_renderer::set_viewport(glm::ivec2 position, glm::ivec2 size)
{
	GLCall(glViewport(position.x, position.y, size.x, size.y));
}

void primitive_renderer::set_viewport(glm::ivec4 position_size) {
	GLCall(glViewport(position_size.x, position_size.y, position_size.z, position_size.a));
}

void primitive_renderer::render(
	Framebuffer& framebuffer, 
	Program& program, 
	VertexAttributeBuffer& vab, 
	Buffer& index_buffer, 
	PrimitiveType primitive, 
	IndexType index_type, 
	const RenderParameters& render_parameters, 
	size_t vertex_offset, 
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
		vertex_offset,
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
	size_t vertex_offset,
	size_t index_offset,
	size_t index_count,
	size_t instance_count,
	size_t instance_offset
) {

	if (vab.get_active_attribute_count() <= 0) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}
	
	render_parameters.apply();
	
	vab.bind();
	program.bind();
	index_buffer.bind_as_index_buffer();
	
	size_t ib_index_count = index_buffer.get_buffer_size_in_bytes() / get_IndexType_bytes_per_index(index_type);

	if (index_count == 0)
		index_count = ib_index_count;

	GLCall(glDrawElementsInstancedBaseVertexBaseInstance(PrimitiveType_to_GL(primitive), index_count, IndexType_to_GL(index_type), (void*)(index_offset * get_IndexType_bytes_per_index(index_type)), instance_count, vertex_offset, instance_offset));
}


void primitive_renderer::render(
	Framebuffer& framebuffer, 
	Program& program, 
	VertexAttributeBuffer& vab,
	PrimitiveType primitive, 
	const RenderParameters& render_parameters, 
	size_t vertex_offset, 
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
		vertex_offset,
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
	size_t vertex_offset, 
	size_t vertex_count, 
	size_t instance_count, 
	size_t instance_offset
) {
	render_parameters.apply();

	program.bind();
	vab.bind();

	if (vab.get_active_attribute_count() <= 0) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}
	
	if (vertex_count == 0)
		vertex_count = vab.get_min_vertex_count();

	GLCall(glDrawArraysInstancedBaseInstance(PrimitiveType_to_GL(primitive), vertex_offset, vertex_count, instance_count, instance_offset));
}

void primitive_renderer::render(Framebuffer& framebuffer, Program& program, Mesh::SingleMesh& single_mesh, const RenderParameters& render_parameters, size_t instance_count, size_t instance_offset)
{
	framebuffer.bind_draw();

	render(
		program,
		single_mesh,
		render_parameters,
		instance_count,
		instance_offset
	);

}

void primitive_renderer::render(Program& program, Mesh::SingleMesh& single_mesh, const RenderParameters& render_parameters, size_t instance_count, size_t instance_offset)
{
	Mesh* mesh = single_mesh.get_owner();
	if (mesh == nullptr) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called with a Mesh::SingleMesh& that doesn't have an owner Mesh" << std::endl;
		ASSERT(false);
	}

	std::shared_ptr<VertexAttributeBuffer> vab = mesh->get_vertex_attribute_buffer();
	if (vab == nullptr) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called with a Mesh::SingleMesh& whose Mesh has a nullptr VertexAttributeBuffer" << std::endl;
		ASSERT(false);
	}

	std::shared_ptr<Buffer> index_buffer = mesh->get_index_buffer();
	if (index_buffer == nullptr && single_mesh.index_count != 0) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called with a Mesh::SingleMesh& that is configured to be rendered with an index buffer but it's Mesh has a nullptr index buffer" << std::endl;
		ASSERT(false);
	}
	
	PrimitiveType primitive = single_mesh.primitive;
	IndexType index_buffer_type = single_mesh.index_type;

	if (single_mesh.index_count == 0) {
		// array rendering
		
		primitive_renderer::render(
			program,
			*vab,
			primitive,
			render_parameters,
			single_mesh.vertex_offset,	
			single_mesh.vertex_count,
			instance_count,
			instance_offset
		);
	}
	else {
		// indexed rendering

		primitive_renderer::render(
			program,
			*vab,
			*index_buffer,
			primitive,
			index_buffer_type,
			render_parameters,
			single_mesh.vertex_offset,
			single_mesh.index_offset,
			single_mesh.index_count,
			instance_count,
			instance_offset
		);
	}

	
}

void primitive_renderer::render(
	Framebuffer& framebuffer,
	Program& program,
	Text& text,
	const RenderParameters& render_parameters,
	size_t instance_count,
	size_t instance_offset
) {
	framebuffer.bind_draw();
	render(
		program,
		text,
		render_parameters,
		instance_count,
		instance_offset
	);
}

void primitive_renderer::render(
	Program& program,
	Text& text,
	const RenderParameters& render_parameters,
	size_t instance_count,
	size_t instance_offset
) {
	text._update_mesh();
	render(
		program,
		*text.mesh->get_mesh(0),
		render_parameters,
		instance_count,
		instance_offset
	);
}

void primitive_renderer::render(
	Framebuffer& framebuffer, 
	Program& program, 
	VertexAttributeBuffer& vab, 
	Buffer& index_buffer, 
	Buffer& draw_indirect_buffer, 
	Buffer& count_parameter_buffer, 
	PrimitiveType primitive, 
	IndexType index_type, 
	const RenderParameters& render_parameters, 
	size_t draw_indirect_buffer_offset, 
	size_t draw_indirect_buffer_size, 
	size_t draw_indirect_buffer_stride,
	size_t parameter_buffer_offset
) {
	framebuffer.bind_draw();
	render(
		program,
		vab,
		index_buffer,
		draw_indirect_buffer,
		count_parameter_buffer,
		primitive,
		index_type,
		render_parameters,
		draw_indirect_buffer_offset,
		draw_indirect_buffer_size,
		draw_indirect_buffer_stride,
		parameter_buffer_offset
	);
}

void primitive_renderer::render(
	Program& program,
	VertexAttributeBuffer& vab,
	Buffer& index_buffer,
	Buffer& draw_indirect_buffer,
	Buffer& count_parameter_buffer,
	PrimitiveType primitive,
	IndexType index_type,
	const RenderParameters& render_parameters,
	size_t draw_indirect_buffer_offset,
	size_t draw_indirect_buffer_size,
	size_t draw_indirect_buffer_stride,
	size_t parameter_buffer_offset
) {
	struct DrawElementsIndirectCommand {
		uint32_t	count;
		uint32_t	instanceCount;
		uint32_t	firstIndex;
		int32_t		baseVertex;
		uint32_t	baseInstance;
	};

	render_parameters.apply();
	
	program.bind();
	
	draw_indirect_buffer.bind_as_draw_indirect_buffer();
	count_parameter_buffer.bind_as_parameter_buffer();
	
	vab.bind();
	index_buffer.bind_as_index_buffer();

	if (draw_indirect_buffer_size == 0)
		draw_indirect_buffer_size = draw_indirect_buffer.get_buffer_size_in_bytes();
	
	if (draw_indirect_buffer_stride == 0)
		draw_indirect_buffer_stride = sizeof(DrawElementsIndirectCommand);

	if (draw_indirect_buffer_stride < sizeof(DrawElementsIndirectCommand)) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called with indirect buffer but given stride is too low to tightly pack DrawElementsIndirectCommand" << std::endl;
		ASSERT(false);
	}

	size_t max_draw_count = draw_indirect_buffer_size / draw_indirect_buffer_stride;

	GLCall(glMultiDrawElementsIndirectCount(
		PrimitiveType_to_GL(primitive), 
		IndexType_to_GL(index_type), 
		std::bit_cast<void*>(draw_indirect_buffer_offset),
		parameter_buffer_offset,
		max_draw_count,
		draw_indirect_buffer_stride
	));
}

void primitive_renderer::render(
	Framebuffer& framebuffer,
	Program& program,
	VertexAttributeBuffer& vab,
	Buffer& index_buffer,
	Buffer& draw_indirect_buffer,
	size_t drawcall_count,
	PrimitiveType primitive,
	IndexType index_type,
	const RenderParameters& render_parameters,
	size_t draw_indirect_buffer_offset,
	size_t draw_indirect_buffer_size,
	size_t draw_indirect_buffer_stride
) {
	framebuffer.bind_draw();
	render(
		program,
		vab,
		index_buffer,
		draw_indirect_buffer,
		drawcall_count,
		primitive,
		index_type,
		render_parameters,
		draw_indirect_buffer_offset,
		draw_indirect_buffer_size,
		draw_indirect_buffer_stride
	);
}

void primitive_renderer::render(
	Program& program,
	VertexAttributeBuffer& vab,
	Buffer& index_buffer,
	Buffer& draw_indirect_buffer,
	size_t drawcall_count,
	PrimitiveType primitive,
	IndexType index_type,
	const RenderParameters& render_parameters,
	size_t draw_indirect_buffer_offset,
	size_t draw_indirect_buffer_size,
	size_t draw_indirect_buffer_stride
) {

	struct DrawElementsIndirectCommand {
		uint32_t	count;
		uint32_t	instanceCount;
		uint32_t	firstIndex;
		int32_t		baseVertex;
		uint32_t	baseInstance;
	};

	render_parameters.apply();
	
	program.bind();
	
	draw_indirect_buffer.bind_as_draw_indirect_buffer();
	
	vab.bind();
	index_buffer.bind_as_index_buffer();

	if (draw_indirect_buffer_size == 0)
		draw_indirect_buffer_size = draw_indirect_buffer.get_buffer_size_in_bytes();
	
	if (draw_indirect_buffer_stride == 0)
		draw_indirect_buffer_stride = sizeof(DrawElementsIndirectCommand);

	if (draw_indirect_buffer_stride < sizeof(DrawElementsIndirectCommand)) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called with indirect buffer but given stride is too low to tightly pack DrawElementsIndirectCommand" << std::endl;
		ASSERT(false);
	}

	if (drawcall_count == 0)
		drawcall_count = draw_indirect_buffer_size / draw_indirect_buffer_stride;

	GLCall(glMultiDrawElementsIndirect(
		PrimitiveType_to_GL(primitive), 
		IndexType_to_GL(index_type), 
		std::bit_cast<void*>(draw_indirect_buffer_offset),
		drawcall_count,
		draw_indirect_buffer_stride
	));
}

void primitive_renderer::clear(Framebuffer& framebuffer, float red, float green, float blue, float alpha)
{
	framebuffer.bind_draw();
	primitive_renderer::clear(red, green, blue, alpha);
}

void primitive_renderer::clear(float red, float green, float blue, float alpha)
{
	RenderParameters params(true);
	params.apply();

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
