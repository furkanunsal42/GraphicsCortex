#include "ImmediateRenderer.h"
#include "PrimitiveRenderer.h"
#include "Math/GLMCout.h"

std::filesystem::path immidiate_renderer_shader_parent_path = "../GraphicsCortex/Source/GLSL/ImmediateRenderer/";

ImmediateRenderer::ImmediateRenderer(size_t max_command_count) :
	buffer_max_command_count(max_command_count)
{

}

void ImmediateRenderer::set_fill_color(glm::vec4 color)
{
	current_properties.fill_color = color;
}

void ImmediateRenderer::set_line_color(glm::vec4 color)
{
	current_properties.line_color = color;
}

void ImmediateRenderer::set_line_thickness(float thickness)
{
	current_properties.line_thickness = thickness;
}

void ImmediateRenderer::set_clip_area(glm::vec4 clip_area)
{
	current_properties.clip_area = clip_area;
}

void ImmediateRenderer::set_clip_area(glm::vec2 begin, glm::vec2 size)
{
	current_properties.clip_area = glm::vec4(begin, size);
}

void ImmediateRenderer::set_border_roundness(glm::vec4 roundness) {
	current_properties.border_roundness = roundness;
}

void ImmediateRenderer::set_border_thickness(glm::vec4 thickness) {
	current_properties.border_thickness = thickness;
}

void ImmediateRenderer::set_border_color0(glm::vec4 color) {
	current_properties.border_color0 = color;
}

void ImmediateRenderer::set_border_color1(glm::vec4 color) {
	current_properties.border_color1 = color;
}

void ImmediateRenderer::set_border_color2(glm::vec4 color) {
	current_properties.border_color2 = color;
}

void ImmediateRenderer::set_border_color3(glm::vec4 color) {
	current_properties.border_color3 = color;
}

void ImmediateRenderer::set_shadow_thickness(glm::vec4 shadow_thickness) {
	current_properties.border_shadow_thickness = shadow_thickness;
}

void ImmediateRenderer::set_shadow_color(glm::vec4 shadow_color) {
	current_properties.border_shadow_color = shadow_color;
}

ImmediateRenderer::DrawProperties ImmediateRenderer::get_current_properties()
{
	return current_properties;
}

void ImmediateRenderer::draw_point(glm::vec3 position0)
{
	DrawCommand command(
		current_properties,
		Point,
		position0
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_line(glm::vec3 position0, glm::vec3 position1)
{
	DrawCommand command(
		current_properties,
		Line,
		position0, 
		position1
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_triangle(
	glm::vec3 position0, glm::vec3 position1, glm::vec3 position2,
	size_t texture_handle, glm::vec2 uv_begin, glm::vec2 uv_end
) {
	DrawCommand command
	(
		current_properties,
		Triangle,
		position0,
		position1,
		position2,
		glm::vec3(0),
		texture_handle,
		uv_begin,
		uv_end
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_triangle(
	glm::vec2 position0, glm::vec2 position1, glm::vec2 position2, float z,
	size_t texture_handle, glm::vec2 uv_begin, glm::vec2 uv_end
) {
	DrawCommand command(
		current_properties,
		Triangle,
		glm::vec3(position0, z),
		glm::vec3(position1, z),
		glm::vec3(position2, z),
		glm::vec3(0),
		texture_handle,
		uv_begin,
		uv_end
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_quad(
	glm::vec3 position0, glm::vec3 position1, glm::vec3 position2, glm::vec3 position3,
	size_t texture_handle, glm::vec2 uv_begin, glm::vec2 uv_end
) {
	DrawCommand command(
		current_properties,
		Quad,
		position0,
		position1,
		position2,
		position3,
		texture_handle,
		uv_begin,
		uv_end
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_quad
(
	glm::vec2 position0, glm::vec2 position1, glm::vec2 position2, glm::vec2 position3, float z,
	size_t texture_handle, glm::vec2 uv_begin, glm::vec2 uv_end
) {
	DrawCommand command(
		current_properties,
		Quad,
		glm::vec3(position0, z),
		glm::vec3(position1, z),
		glm::vec3(position2, z),
		glm::vec3(position3, z),
		texture_handle,
		uv_begin,
		uv_end
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}


void ImmediateRenderer::draw_rectangle(
	glm::vec2 position0, glm::vec2 position1, float z,
	size_t texture_handle, glm::vec2 uv_begin, glm::vec2 uv_end
)
{
	draw_quad(
		glm::vec3(position0.x, position0.y, z),
		glm::vec3(position1.x, position0.y, z),
		glm::vec3(position1.x, position1.y, z),
		glm::vec3(position0.x, position1.y, z),
		texture_handle,
		uv_begin,
		uv_end
	);
}

void ImmediateRenderer::render(Framebuffer& target_framebuffer, RenderParameters render_parameters)
{
	render_without_clear(target_framebuffer, render_parameters);
	clear();
}

void ImmediateRenderer::render(RenderParameters render_parameters)
{
	render_without_clear(render_parameters);
	clear();
}

void ImmediateRenderer::render_without_clear(Framebuffer& target_framebuffer, RenderParameters render_parameters)
{
	Framebuffer previous_framebuffer = Framebuffer::get_current_draw();
	target_framebuffer.bind_draw();
	render_without_clear(render_parameters);
	previous_framebuffer.bind_draw();
}

void ImmediateRenderer::render_without_clear(RenderParameters render_parameters)
{
	_compile_shaders();

	_update_gpu_buffers();

	size_t previous_vertex_count = 0;

	if (point_commands_vertex_count != 0) {
		primitive_renderer::render(
			*line_program,
			*vab,
			PrimitiveType::line,
			render_parameters,
			previous_vertex_count,
			point_commands_vertex_count,
			1,
			0
		);

		previous_vertex_count += point_commands_vertex_count;
	}

	if (line_commands_vertex_count != 0) {

		primitive_renderer::render(
			*line_program,
			*vab,
			PrimitiveType::line,
			render_parameters,
			previous_vertex_count,
			line_commands_vertex_count,
			1,
			0
		);

		previous_vertex_count += line_commands_vertex_count;
	}

	if (triangle_commands_vertex_count != 0) {
		primitive_renderer::render(
			*triangle_program,
			*vab,
			PrimitiveType::triangle,
			render_parameters,
			previous_vertex_count,
			triangle_commands_vertex_count,
			1,
			0
		);

		previous_vertex_count += triangle_commands_vertex_count;
	}
}

void ImmediateRenderer::render(Framebuffer& target_framebuffer, Camera& camera, RenderParameters render_parameters){
	render_without_clear(target_framebuffer, camera, render_parameters);
	clear();
}

void ImmediateRenderer::render(Camera& camera, RenderParameters render_parameters){
	render_without_clear(camera, render_parameters);
	clear();
}

void ImmediateRenderer::render_without_clear(Framebuffer& target_framebuffer, Camera& camera, RenderParameters render_parameters)
{
	Framebuffer previous_framebuffer = Framebuffer::get_current_draw();
	target_framebuffer.bind_draw();
	render_without_clear(camera, render_parameters);
	previous_framebuffer.bind_draw();
}

void ImmediateRenderer::render_without_clear(Camera& camera, RenderParameters render_parameters)
{
	_compile_shaders();

	//camera.update_matrixes();
	line_program->update_uniform("view", camera.view_matrix);
	line_program->update_uniform("projection", camera.projection_matrix);
	triangle_program->update_uniform("view", camera.view_matrix);
	triangle_program->update_uniform("projection", camera.projection_matrix);
	
	render_without_clear(render_parameters);

	line_program->update_uniform("view", glm::identity<glm::mat4>());
	line_program->update_uniform("projection", glm::identity<glm::mat4>());
	triangle_program->update_uniform("view", glm::identity<glm::mat4>());
	triangle_program->update_uniform("projection", glm::identity<glm::mat4>());
}

void ImmediateRenderer::_compile_shaders()
{
	if (line_program != nullptr && triangle_program != nullptr)
		return;
	
	line_program		= std::make_shared<Program>(Shader(immidiate_renderer_shader_parent_path / "vertex.vert", immidiate_renderer_shader_parent_path / "line.frag"));
	triangle_program	= std::make_shared<Program>(Shader(immidiate_renderer_shader_parent_path / "vertex.vert", immidiate_renderer_shader_parent_path / "triangle.frag"));

	line_program->update_uniform("view", glm::identity<glm::mat4>());
	line_program->update_uniform("projection", glm::identity<glm::mat4>());
	triangle_program->update_uniform("view", glm::identity<glm::mat4>());
	triangle_program->update_uniform("projection", glm::identity<glm::mat4>());
}

void ImmediateRenderer::_init_vab()
{
	if (vab != nullptr)
		return;

	vab = std::make_shared<VertexAttributeBuffer>();

	vab->attach_vertex_buffer(0, std::make_shared<Buffer>(buffer_max_command_count * _draw_command_size_in_bytes), _draw_command_size_in_bytes, 0, 0);
		
	vab->push_attribute_format(0, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(1, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(2, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(3, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(4, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(5, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(6, 0, VertexAttributeBuffer::a_ui32, 4, true);

	vab->push_attribute_format(7, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(8, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(9, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(10, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(11, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(12, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(13, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(14, 0, VertexAttributeBuffer::a_f32, 4, true);
	vab->push_attribute_format(15, 0, VertexAttributeBuffer::a_f32, 4, true);
}

void ImmediateRenderer::_update_gpu_buffers() 
{
	_init_vab();

	if (!attribute_buffer_corrupted)
		return;

	vab->get_buffer_slot(0).buffer->clear();

	std::vector<DrawCommandGpu> data;
	
	point_commands_vertex_count = 0;
	line_commands_vertex_count = 0;
	triangle_commands_vertex_count = 0;

	size_t previous_vertex_count = 0;

	size_t next_handle_index = 0;

	// Point Commands
	for (DrawCommand& command : commands) {
		if (command.geometry != Point)
			continue;

		DrawCommandGpu gpu_struct;
		gpu_struct.fill_color = command.properties.fill_color;
		gpu_struct.line_color = command.properties.line_color;
		gpu_struct.line_thickness_with_padding = glm::vec4(command.properties.line_thickness, 0, 0, 0);

		gpu_struct.position = glm::vec4(command.position0, 1);
		data.push_back(gpu_struct);
	}
	point_commands_vertex_count = data.size() - previous_vertex_count;
	previous_vertex_count = data.size();

	// Line Commands
	for (DrawCommand& command : commands) {
		if (command.geometry != Line)
			continue;

		DrawCommandGpu gpu_struct;
		gpu_struct.fill_color					= command.properties.fill_color;
		gpu_struct.line_color					= command.properties.line_color;
		gpu_struct.line_thickness_with_padding	= glm::vec4(command.properties.line_thickness, 0, 0, 0);
	
		gpu_struct.position = glm::vec4(command.position0, 1);
		data.push_back(gpu_struct);

		gpu_struct.position = glm::vec4(command.position1, 1);
		data.push_back(gpu_struct);
	}
	line_commands_vertex_count = data.size() - previous_vertex_count;
	previous_vertex_count = data.size();

	// Triangle/Quad Commands
	for (DrawCommand& command : commands) {
		if (command.geometry != Triangle && command.geometry != Quad)
			continue;

		glm::uvec2 texture_handle = *(glm::uvec2*)&command.texture_handle;

		DrawCommandGpu gpu_struct;
		gpu_struct.fill_color = command.properties.fill_color;
		gpu_struct.line_color = command.properties.line_color;
		gpu_struct.line_thickness_with_padding = glm::vec4(command.properties.line_thickness, 0, 0, 0);
		
		gpu_struct.fill_color = command.properties.fill_color;
		gpu_struct.line_color = command.properties.line_color;
		gpu_struct.line_thickness_with_padding = glm::vec4(command.properties.line_thickness, 0, 0, 0);
		gpu_struct.border_roundness = command.properties.border_roundness;
		gpu_struct.border_thickness = command.properties.border_thickness;
		gpu_struct.border_color0 = command.properties.border_color0;
		gpu_struct.border_color1 = command.properties.border_color1;
		gpu_struct.border_color2 = command.properties.border_color2;
		gpu_struct.border_color3 = command.properties.border_color3;
		gpu_struct.border_shadow_thickness = command.properties.border_shadow_thickness;
		gpu_struct.border_shadow_color = command.properties.border_shadow_color;
		gpu_struct.position_size = glm::vec4(command.position2.x, command.position2.y, command.position0.x - command.position2.x, command.position0.y - command.position2.y);

		gpu_struct.texture_handle = glm::uvec4(texture_handle, 0, 0);

		gpu_struct.position = glm::vec4(command.position0, 1);
		gpu_struct.uv		= glm::vec4(command.uv_begin.x, command.uv_begin.y, glm::vec2(0));
		data.push_back(gpu_struct);

		gpu_struct.position = glm::vec4(command.position1, 1);
		gpu_struct.uv = glm::vec4(command.uv_end.x, command.uv_begin.y, glm::vec2(0));
		data.push_back(gpu_struct);

		gpu_struct.position = glm::vec4(command.position2, 1);
		gpu_struct.uv = glm::vec4(command.uv_end.x, command.uv_end.y, glm::vec2(0));
		data.push_back(gpu_struct);

		if (command.geometry == Quad) {
			gpu_struct.position = glm::vec4(command.position0, 1);
			gpu_struct.uv = glm::vec4(command.uv_begin.x, command.uv_begin.y, glm::vec2(0));
			data.push_back(gpu_struct);

			gpu_struct.position = glm::vec4(command.position2, 1);
			gpu_struct.uv = glm::vec4(command.uv_end.x, command.uv_end.y, glm::vec2(0));
			data.push_back(gpu_struct);

			gpu_struct.position = glm::vec4(command.position3, 1);
			gpu_struct.uv = glm::vec4(command.uv_begin.x, command.uv_end.y, glm::vec2(0));
			data.push_back(gpu_struct);
		}

	}
	
	triangle_commands_vertex_count = data.size() - previous_vertex_count;
	previous_vertex_count = data.size();

	vab->get_buffer_slot(0).buffer->load_data(data);
	attribute_buffer_corrupted = false;

}

void ImmediateRenderer::clear()
{
	commands.clear();
	vab->get_buffer_slot(0).buffer->clear();
	point_commands_vertex_count = 0;
	line_commands_vertex_count = 0;
	triangle_commands_vertex_count = 0;
}

ImmediateRenderer::DrawCommand::DrawCommand(
	DrawProperties properties, 
	Geometry geometry, 
	glm::vec3	position0, 
	glm::vec3	position1, 
	glm::vec3	position2, 
	glm::vec3	position3,
	size_t		texture_handle,
	glm::vec2	uv_begin,
	glm::vec2	uv_end
) :
	properties(properties),
	geometry(geometry),
	position0(position0),
	position1(position1),
	position2(position2),
	position3(position3),
	texture_handle(texture_handle),
	uv_begin(uv_begin),
	uv_end(uv_end)
{

}
