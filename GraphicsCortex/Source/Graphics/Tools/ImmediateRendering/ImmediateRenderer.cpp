#include "ImmediateRenderer.h"
#include "PrimitiveRenderer.h"

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

void ImmediateRenderer::draw_triangle(glm::vec3 position0, glm::vec3 position1, glm::vec3 position2)
{
	DrawCommand command(
		current_properties,
		Triangle,
		position0,
		position1,
		position2
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::draw_quad(glm::vec3 position0, glm::vec3 position1, glm::vec3 position2, glm::vec3 position3)
{
	DrawCommand command(
		current_properties,
		Quad,
		position0,
		position1,
		position2,
		position3
	);
	commands.push_back(command);
	attribute_buffer_corrupted = true;
}

void ImmediateRenderer::render(Framebuffer& target_framebuffer)
{
	render_without_clear(target_framebuffer);
	_clear();
}

void ImmediateRenderer::render()
{
	render_without_clear();
	_clear();
}

void ImmediateRenderer::render_without_clear(Framebuffer& target_framebuffer)
{
	Framebuffer previous_framebuffer = Framebuffer::get_current_draw();
	target_framebuffer.bind_draw();
	render_without_clear();
	previous_framebuffer.bind_draw();
}

void ImmediateRenderer::render_without_clear()
{
	_compile_shaders();

	_update_gpu_buffers();

	size_t previous_vertex_count = 0;

	if (point_commands_vertex_count != 0) {
		primitive_renderer::render(
			*line_program,
			*vab,
			PrimitiveType::line,
			RenderParameters(),
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
			RenderParameters(),
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
			RenderParameters(),
			previous_vertex_count,
			triangle_commands_vertex_count,
			1,
			0
		);

		previous_vertex_count += triangle_commands_vertex_count;
	}
}

void ImmidiateRenderer::render(Framebuffer& target_framebuffer, Camera& camera){
	render_without_clear(target_framebuffer, camera);
	_clear();
}

void ImmidiateRenderer::render(Camera& camera){
	render_without_clear(camera);
	_clear();
}

void ImmidiateRenderer::render_without_clear(Framebuffer& target_framebuffer, Camera& camera)
{
	Framebuffer previous_framebuffer = Framebuffer::get_current_draw();
	target_framebuffer.bind_draw();
	render_without_clear(camera);
	previous_framebuffer.bind_draw();
}

void ImmidiateRenderer::render_without_clear(Camera& camera)
{
	_compile_shaders();

	camera.update_matrixes();
	line_program->update_uniform("view", camera.view_matrix);
	line_program->update_uniform("projection", camera.projection_matrix);
	triangle_program->update_uniform("view", camera.view_matrix);
	triangle_program->update_uniform("projection", camera.projection_matrix);
	
	render_without_clear();

	line_program->update_uniform("view", glm::identity<glm::mat4>());
	line_program->update_uniform("projection", glm::identity<glm::mat4>());
	triangle_program->update_uniform("view", glm::identity<glm::mat4>());
	triangle_program->update_uniform("projection", glm::identity<glm::mat4>());
}

void ImmidiateRenderer::_compile_shaders()
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
	if (vab == nullptr) {
		vab = std::make_shared<VertexAttributeBuffer>();

		vab->attach_vertex_buffer(0, std::make_shared<Buffer>(buffer_max_command_count * _draw_command_size_in_bytes), _draw_command_size_in_bytes, 0, 0);
		
		vab->push_attribute_format(0, 0, VertexAttributeBuffer::a_f32, 4, true);
		vab->push_attribute_format(1, 0, VertexAttributeBuffer::a_f32, 4, true);
		vab->push_attribute_format(2, 0, VertexAttributeBuffer::a_f32, 4, true);
		vab->push_attribute_format(3, 0, VertexAttributeBuffer::a_f32, 4, true);
	}
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
		gpu_struct.fill_color = command.properties.fill_color;
		gpu_struct.line_color = command.properties.line_color;
		gpu_struct.line_thickness_with_padding = glm::vec4(command.properties.line_thickness, 0, 0, 0);
		
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

		if (command.geometry == Triangle || command.geometry == Quad) {
			DrawCommandGpu gpu_struct;
			gpu_struct.fill_color = command.properties.fill_color;
			gpu_struct.line_color = command.properties.line_color;
			gpu_struct.line_thickness_with_padding = glm::vec4(command.properties.line_thickness, 0, 0, 0);
			
			gpu_struct.position = glm::vec4(command.position0, 1);
			data.push_back(gpu_struct);

			gpu_struct.position = glm::vec4(command.position1, 1);
			data.push_back(gpu_struct);

			gpu_struct.position = glm::vec4(command.position2, 1);
			data.push_back(gpu_struct);

			if (command.geometry == Quad) {
				gpu_struct.position = glm::vec4(command.position0, 1);
				data.push_back(gpu_struct);

				gpu_struct.position = glm::vec4(command.position2, 1);
				data.push_back(gpu_struct);

				gpu_struct.position = glm::vec4(command.position3, 1);
				data.push_back(gpu_struct);
			}
		}
	}

	triangle_commands_vertex_count = data.size() - previous_vertex_count;
	previous_vertex_count = data.size();

	vab->get_buffer_slot(0).buffer->load_data(data);
	attribute_buffer_corrupted = false;
}

void ImmediateRenderer::_clear()
{
	commands.clear();
	vab->get_buffer_slot(0).buffer->clear();
	point_commands_vertex_count = 0;
	line_commands_vertex_count = 0;
	triangle_commands_vertex_count = 0;
}

ImmediateRenderer::DrawCommand::DrawCommand(DrawProperties properties, Geometry geometry, glm::vec3 position0, glm::vec3 position1, glm::vec3 position2, glm::vec3 position3) :
	properties(properties),
	geometry(geometry),
	position0(position0),
	position1(position1),
	position2(position2),
	position3(position3)
{

}
