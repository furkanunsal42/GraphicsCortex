#include "PrimitiveRenderer.h"

#include "GL/glew.h"
#include "Debuger.h"

#include "ShaderCompiler.h"
#include "Mesh2.h"
#include "VertexAttributeBuffer.h"

void primitive_renderer::render(Program& program, VertexAttributeBuffer& vab, PrimitiveType primitive)
{
	int32_t largest_slot = vab.get_largest_active_buffer_slot();
	if (largest_slot == -1) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given VertexAttributeBuffer is enabled" << std::endl;
		ASSERT(false);
	}

	Buffer& vertex_buffer = *vab.get_vertex_buffer(largest_slot);

	vab.bind();
	program.bind();

	GLCall(glDrawArrays(PrimitiveType_to_GL(primitive), 0, vertex_buffer.get_buffer_size_in_bytes()));
}
