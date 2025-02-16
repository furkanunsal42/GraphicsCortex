#pragma once

#include <memory>
#include <vector>

#include "FrameBuffer.h"
#include "Camera.h"
#include "glm.hpp"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

class ComputeProgram;
class Program;
class Buffer;
class Texture2D;
class VertexAttributeBuffer;

class GraphRenderer {
public:

	GraphRenderer(glm::vec3 region_min = glm::vec3(-5, -5, -5), glm::vec3 region_max = glm::vec3(5, 5, 5), glm::vec3 scale = glm::vec3(1), glm::ivec2 sample_rate = glm::ivec2(16, 4));

	void load_data(Buffer& buffer_source);
	void load_data(Texture2D& texture_source, glm::vec2 source_region_min, glm::vec2 source_region_max);
	
	void render(Framebuffer& target, Camera& camera);
	void render(Camera& camera);

private:

	void _allocate_resources();
	bool _is_resources_allocated = false;

	glm::vec3 region_min;
	glm::vec3 region_max;
	glm::vec3 scale;
	glm::ivec2 sample_rate;

	std::shared_ptr<Program> point_cloud_renderer;
	std::shared_ptr<Program> axis_line_renderer;
	
	GraphicsOperation op;
	//std::shared_ptr<ComputeProgram> generate_point_cloud_from_buffer;
	//std::shared_ptr<ComputeProgram> generate_point_cloud_from_texture;

	std::shared_ptr<Buffer> point_cloud;
	std::shared_ptr<Buffer> axis_lines;
	std::shared_ptr<VertexAttributeBuffer> vab;
};