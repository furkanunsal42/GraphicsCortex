#include "GraphRenderer.h"
#include "glm.hpp"
#include "Buffer.h"
#include "PrimitiveRenderer.h"
#include "VertexAttributeBuffer.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

GraphRenderer::GraphRenderer(glm::vec3 region_min, glm::vec3 region_max, glm::vec3 scale, glm::ivec2 sample_rate) :
	region_min(region_min), region_max(region_max), scale(scale), sample_rate(sample_rate) { }

void GraphRenderer::load_data(Texture2D& texture_source, glm::vec2 source_region_min, glm::vec2 source_region_max)
{
	glm::ivec2 sample_resolution = sample_rate * glm::ivec2(region_max - region_min);
	size_t point_count = sample_resolution.x * sample_resolution.y;
	
	if (point_cloud == nullptr || point_cloud->get_buffer_size_in_bytes() != 2 * point_count * sizeof(glm::vec3))
		point_cloud = std::make_shared<Buffer>(2 * point_count * sizeof(glm::vec3));
	
	op.set_constant("region_max", glm::vec2(region_max));
	op.set_constant("region_min", glm::vec2(region_min));
	op.set_constant("source_region_min", glm::vec2(source_region_min));
	op.set_constant("source_region_max", glm::vec2(source_region_max));
	op.set_constant("sample_resolution", glm::ivec2(sample_resolution));

	op.compute(
		*point_cloud, "float",
		texture_source, true,
		"int offset = int(id.x) % 3;"
		"int id_p = int(id.x) / 3;"
		"ivec2 id2d = ivec2(id_p % sample_resolution.y, mod(id_p / sample_resolution.y, sample_resolution.x));"
		"vec2 id2d_n_pre = vec2(id2d) / sample_resolution.yx;"
		"vec2 id2d_n = vec2(0);"
		"id2d_n += int(id_p < (sample_resolution.x * sample_resolution.y)) * id2d_n_pre.xy;"
		"id2d_n += int(id_p >= (sample_resolution.x * sample_resolution.y)) * id2d_n_pre.yx;"
		"vec2 target_coord = id2d_n.xy * (region_max - region_min) + region_min;"
		"vec2 source_coord = (target_coord - source_region_min) / (source_region_max - source_region_min);"
		"if(any(lessThan(source_coord, vec2(0))) || any(greaterThanEqual(source_coord, vec2(1)))) return;"
		"vec4 source_sample = texture(source_data, source_index_type(source_coord));"
		"float r = 0;"
		"r += int(offset == 0) * target_coord.x;"
		"r += int(offset == 1) * source_sample.x;"
		"r += int(offset == 2) * target_coord.y;"
		"target[id.x] = r;",
		glm::ivec3(0, 0, 0), glm::ivec3(2*3* sample_resolution.x * sample_resolution.y, 1, 1)
	);

	op.clear_constants();
}

void GraphRenderer::render(Framebuffer& target, Camera& camera)
{
	// get previous bound framebuffer
	target.bind_draw();

	render(camera);

	// bind previous buffer
}

void GraphRenderer::render(Camera& camera)
{
	_allocate_resources();

	axis_line_renderer->update_uniform("model", glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(1)/scale));
	axis_line_renderer->update_uniform("color", glm::vec4(1, 0.2, 0.2, 1));
	camera.update_matrixes();
	camera.update_default_uniforms(*axis_line_renderer);

	vab->attach_vertex_buffer(0, axis_lines, sizeof(glm::vec3), 0);
	primitive_renderer::render(*axis_line_renderer, *vab, PrimitiveType::line);

	if (point_cloud == nullptr)
		return;

	point_cloud_renderer->update_uniform("model", glm::scale(glm::identity<glm::mat4x4>(), glm::vec3(1)/scale));
	point_cloud_renderer->update_uniform("color", glm::vec4(0, 0, 0, 1));
	camera.update_default_uniforms(*point_cloud_renderer);

	vab->attach_vertex_buffer(0, point_cloud, VertexAttributeBuffer::a_f32, 3, sizeof(glm::vec3), 0, true);
	
	size_t line_count = (sample_rate.x * (region_max - region_min).x) * 2;
	size_t line_size = (sample_rate.y * (region_max - region_min).y);

	for (int i = 0; i < line_count; i++)
		primitive_renderer::render(*point_cloud_renderer, *vab, PrimitiveType::line_strip, i*line_size, line_size);
}

void GraphRenderer::_allocate_resources()
{
	if (_is_resources_allocated) return;

	axis_lines = std::make_shared<Buffer>(6 * sizeof(glm::vec3));
	std::vector<float> lines_data =
	{	0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 1, 0,
		0, 0, 0, 0, 0, 1	};

	axis_lines->load_data(lines_data);

	vab = std::make_shared<VertexAttributeBuffer>();
	vab->attach_vertex_buffer(0, axis_lines, VertexAttributeBuffer::a_f32, 3, sizeof(glm::vec3), 0, true);

	axis_line_renderer = std::make_shared<Program>(
		Shader(	"../GraphicsCortex/Source/GLSL/GraphRendering/basic.vert",
				"../GraphicsCortex/Source/GLSL/GraphRendering/flatcolor.frag"));

	point_cloud_renderer = std::make_shared<Program>(
		Shader(	"../GraphicsCortex/Source/GLSL/GraphRendering/basic.vert",
				"../GraphicsCortex/Source/GLSL/GraphRendering/flatcolor.frag"));

	_is_resources_allocated = true;
}
