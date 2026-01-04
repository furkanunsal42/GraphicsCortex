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
#include "RenderParameters.h"

extern std::filesystem::path immidiate_renderer_shader_parent_path;

class ImmediateRenderer {
public:

	ImmediateRenderer(size_t max_command_count = 1024);

	struct DrawProperties;
	void set_fill_color(glm::vec4 color);
	void set_line_color(glm::vec4 color);
	void set_line_thickness(float thickness);
	void set_clip_area(glm::vec4 clip_area);
	void set_clip_area(glm::vec2 begin, glm::vec2 size);
	void set_border_roundness(glm::vec4 roundness);
	void set_border_thickness(glm::vec4 thickness);
	void set_border_color0(glm::vec4 color);
	void set_border_color1(glm::vec4 color);
	void set_border_color2(glm::vec4 color);
	void set_border_color3(glm::vec4 color);
	void set_shadow_thickness(glm::vec4 shadow_thickness);
	void set_shadow_color(glm::vec4 shadow_color);

	DrawProperties get_current_properties();

	void draw_point(glm::vec3 position0);
	void draw_line(glm::vec3 position0, glm::vec3 position1);
	
	void draw_triangle
	(
		glm::vec3 position0, glm::vec3 position1, glm::vec3 position2,
		size_t texture_handle = 0, glm::vec2 uv_begin = glm::vec2(0), glm::vec2 uv_end = glm::vec2(1)
	);

	void draw_triangle
	(
		glm::vec2 position0, glm::vec2 position1, glm::vec2 position2, float z = 0,
		size_t texture_handle = 0, glm::vec2 uv_begin = glm::vec2(0), glm::vec2 uv_end = glm::vec2(1)
	);

	void draw_quad
	(
		glm::vec3 position0, glm::vec3 position1, glm::vec3 position2, glm::vec3 position3,
		size_t texture_handle = 0, glm::vec2 uv_begin = glm::vec2(0), glm::vec2 uv_end = glm::vec2(1)
	);

	void draw_quad
	(
		glm::vec2 position0, glm::vec2 position1, glm::vec2 position2, glm::vec2 position3, float z = 0,
		size_t texture_handle = 0, glm::vec2 uv_begin = glm::vec2(0), glm::vec2 uv_end = glm::vec2(1)
	);
	
	void draw_rectangle
	(
		glm::vec2 position0, glm::vec2 position1, float z = 0,
		size_t texture_handle = 0, glm::vec2 uv_begin = glm::vec2(0), glm::vec2 uv_end = glm::vec2(1)
	);

	void render(Framebuffer& target_framebuffer, RenderParameters render_parameters = RenderParameters(false));
	void render(RenderParameters render_parameters = RenderParameters(false));
	void render_without_clear(Framebuffer& target_framebuffer, RenderParameters render_parameters = RenderParameters(false));
	void render_without_clear(RenderParameters render_parameters = RenderParameters(false));

	void render(Framebuffer& target_framebuffer, Camera& camera, RenderParameters render_parameters = RenderParameters(false));
	void render(Camera& camera, RenderParameters render_parameters = RenderParameters(false));
	void render_without_clear(Framebuffer& target_framebuffer, Camera& camera, RenderParameters render_parameters = RenderParameters(false));
	void render_without_clear(Camera& camera, RenderParameters render_parameters = RenderParameters(false));

	void clear();

private:

	const int32_t buffer_max_command_count;
	constexpr static size_t _draw_command_size_in_bytes =
		sizeof(glm::vec4) +		// fill_color,
		sizeof(glm::vec4) +		// line_color,
		sizeof(glm::vec4) +		// line_thickness(x)
		sizeof(glm::vec4) +		// position
		sizeof(glm::vec4) +		// clip_area
		sizeof(glm::vec2) +		// uv
		sizeof(glm::uvec2)+		// texture_handle
		sizeof(glm::vec4) +		// border_roundness
		sizeof(glm::vec4) + 	// border_thickness
		sizeof(glm::vec4) + 	// border_color0
		sizeof(glm::vec4) + 	// border_color1
		sizeof(glm::vec4) + 	// border_color2
		sizeof(glm::vec4) + 	// border_color3
		sizeof(glm::vec4) + 	// border_shadow_thickness
		sizeof(glm::vec4)		// border_shadow_color
		;

	void _compile_shaders();
	void _init_vab();

	struct DrawCommandGpu {
		glm::vec4	fill_color					= glm::vec4(0, 0, 0, 1);
		glm::vec4	line_color					= glm::vec4(0, 0, 0, 1);
		glm::vec4	line_thickness_with_padding = glm::vec4(1, 0, 0, 0);
		glm::vec4	position;
		glm::vec4	clip_area					= glm::vec4(0, 0, 1, 1);
		glm::vec2	uv							= glm::vec2(0);
		glm::uvec2	texture_handle				= glm::uvec2(0);
		glm::vec4	border_roundness			= glm::vec4(0);
		glm::vec4	border_thickness			= glm::vec4(0);
		glm::vec4	border_color0				= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color1				= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color2				= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color3				= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_shadow_thickness		= glm::vec4(0);
		glm::vec4	border_shadow_color			= glm::vec4(0, 0, 0, 1);
	};

	size_t point_commands_vertex_count = 0;
	size_t line_commands_vertex_count = 0;
	size_t triangle_commands_vertex_count = 0;
	
	void _update_gpu_buffers();

	enum Geometry {
		None = 0,
		Point,
		Line,
		Triangle,
		Quad,
	};

	struct DrawProperties {

		glm::vec4 fill_color	= glm::vec4(0, 0, 0, 1);
		glm::vec4 line_color	= glm::vec4(0, 0, 0, 1);
		float line_thickness	= 1.0f;
		glm::vec4 clip_area		= glm::vec4(0, 0, 1, 1);
	
		glm::vec4 border_roundness			= glm::vec4(0);
		glm::vec4 border_thickness			= glm::vec4(0);
		glm::vec4 border_color0				= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color1				= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color2				= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color3				= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_shadow_thickness	= glm::vec4(0);
		glm::vec4 border_shadow_color		= glm::vec4(0, 0, 0, 1);

	};

	struct DrawCommand {
		DrawCommand() = default;
		DrawCommand(
			DrawProperties properties,
			Geometry	geometry,
			glm::vec3	position0	= glm::vec3(0),
			glm::vec3	position1	= glm::vec3(0),
			glm::vec3	position2	= glm::vec3(0),
			glm::vec3	position3	= glm::vec3(0),
			size_t		texture_handle = 0,
			glm::vec2	uv_begin	= glm::vec2(0),
			glm::vec2	uv_end		= glm::vec2(1)
		);
		DrawProperties properties;
		Geometry	geometry = None;
		glm::vec3	position0;
		glm::vec3	position1;
		glm::vec3	position2;
		glm::vec3	position3;
		size_t		texture_handle;
		glm::vec2	uv_begin;
		glm::vec2	uv_end;
	};

	DrawProperties current_properties;

	std::vector<DrawCommand> commands;
	std::shared_ptr<VertexAttributeBuffer> vab;

	std::shared_ptr<Program> line_program = nullptr;
	std::shared_ptr<Program> triangle_program = nullptr;

	bool attribute_buffer_corrupted = false;
};