#pragma once

#include <memory>
#include <vector>

#include "Text/Text.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "glm.hpp"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

class ComputeProgram;
class Program;
class Buffer;
class Texture2D;
class VertexAttributeBuffer;

class GraphRenderer2D {
public:

	GraphRenderer2D(glm::vec2 region_min = glm::vec2(-5, -5), glm::vec2 region_max = glm::vec2(5, 5), glm::vec2 scale = glm::vec2(1));
	GraphRenderer2D(glm::vec2 region_min = glm::vec2(-5, -5), glm::vec2 region_max = glm::vec2(5, 5), glm::vec2 scale = glm::vec2(1));

	void load_data(Buffer& buffer_source, float source_region_min, float source_region_max);
	void load_data(Texture1D& texture_source, float source_region_min, float source_region_max);
	
	void render(Framebuffer& target);
	void render();

private:

	void _allocate_resources();
	bool _is_resources_allocated = false;

	glm::vec2 region_min;
	glm::vec2 region_max;

	std::shared_ptr<Program> point_cloud_renderer;
	std::shared_ptr<Program> axis_line_renderer;
	
	GraphicsOperation op;
	//std::shared_ptr<ComputeProgram> generate_point_cloud_from_buffer;
	//std::shared_ptr<ComputeProgram> generate_point_cloud_from_texture;

	std::shared_ptr<Buffer> point_cloud;
	std::shared_ptr<Buffer> axis_lines;
	std::shared_ptr<VertexAttributeBuffer> vab;
};