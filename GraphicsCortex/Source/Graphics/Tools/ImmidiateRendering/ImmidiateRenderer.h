#pragma once
#pragma once

#include <vector>
#include <memory>
#include <filesystem>
#include "glm.hpp"

#include "VertexAttributeBuffer.h"
#include "FrameBuffer.h"
#include "ShaderCompiler.h"
#include "Camera.h"

extern std::filesystem::path immidiate_renderer_shader_parent_path;

class ImmidiateRenderer {
public:

	ImmidiateRenderer(size_t max_command_count = 1024);

	struct DrawProperties;

	void set_fill_color(glm::vec4 color);
	void set_line_color(glm::vec4 color);
	void set_line_thickness(float thickness);
	DrawProperties get_current_properties();

	void draw_point(glm::vec3 position0);
	void draw_line(glm::vec3 position0, glm::vec3 position1);
	void draw_triangle(glm::vec3 position0, glm::vec3 position1, glm::vec3 position2);
	void draw_quad(glm::vec3 position0, glm::vec3 position1, glm::vec3 position2, glm::vec3 position3);

	void render(Framebuffer& target_framebuffer);
	void render();
	void render_without_clear(Framebuffer& target_framebuffer);
	void render_without_clear();

	void render(Framebuffer& target_framebuffer, Camera& camera);
	void render(Camera& camera);
	void render_without_clear(Framebuffer& target_framebuffer, Camera& camera);
	void render_without_clear(Camera& camera);

private:

	const int32_t buffer_max_command_count;
	constexpr static size_t _draw_command_size_in_bytes =
		sizeof(glm::vec4) * 2 + // fill_color, line_color,
		sizeof(glm::vec4) +		// line_thickness(x)
		sizeof(glm::vec4)		// position
		;

	void _compile_shaders();
	void _init_vab();
	
	struct DrawCommandGpu {
		glm::vec4 fill_color = glm::vec4(0, 0, 0, 1);
		glm::vec4 line_color = glm::vec4(0, 0, 0, 1);
		glm::vec4 line_thickness_with_padding = glm::vec4(1, 0, 0, 0);
		glm::vec4 position;
	};

	size_t point_commands_vertex_count = 0;
	size_t line_commands_vertex_count = 0;
	size_t triangle_commands_vertex_count = 0;
	
	void _update_gpu_buffers();

	void _clear();

	enum Geometry {
		None = 0,
		Point,
		Line,
		Triangle,
		Quad,
	};

	struct DrawProperties {
		glm::vec4 fill_color = glm::vec4(0, 0, 0, 1);
		glm::vec4 line_color = glm::vec4(0, 0, 0, 1);
		float line_thickness = 1.0f;
	};

	struct DrawCommand {
		DrawCommand() = default;
		DrawCommand(
			DrawProperties properties,
			Geometry geometry,
			glm::vec3 position0 = glm::vec3(0),
			glm::vec3 position1 = glm::vec3(0),
			glm::vec3 position2 = glm::vec3(0),
			glm::vec3 position3 = glm::vec3(0)
		);
		DrawProperties properties;
		Geometry geometry = None;
		glm::vec3 position0;
		glm::vec3 position1;
		glm::vec3 position2;
		glm::vec3 position3;
	};

	DrawProperties current_properties;

	std::vector<DrawCommand> commands;
	std::shared_ptr<VertexAttributeBuffer> vab;

	std::shared_ptr<Program> line_program = nullptr;
	std::shared_ptr<Program> triangle_program = nullptr;

	bool attribute_buffer_corrupted = false;
};