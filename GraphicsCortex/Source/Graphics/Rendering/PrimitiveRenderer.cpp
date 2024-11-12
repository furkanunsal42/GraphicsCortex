#include "PrimitiveRenderer.h"

#include "GL/glew.h"
#include "Debuger.h"

#include "ShaderCompiler.h"
#include "AttributedVertexBuffer.h"

void primitive_renderer::render(Program& program, AttributedVertexBuffer& verticies)
{
	int32_t largest_slot = verticies.get_largest_active_buffer_slot();
	if (largest_slot == -1) {
		std::cout << "[OpenGL Error] primitive_renderer::render() is called but no attribute of given AttributedVertexBuffer is enabled" << std::endl;
		ASSERT(false);
	}

	Buffer& vertex_buffer = *verticies.get_vertex_buffer(largest_slot);

	verticies.bind();
	program.bind();

	GLCall(glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.get_buffer_size_in_bytes()));
}
