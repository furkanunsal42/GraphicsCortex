#include "CurvedSurface.h"

#include "PrimitiveRenderer.h"

std::filesystem::path curved_surface_parent_shader_path = "../Gozdiscoptics/Source/GLSL/";

void CurvedSurface::define_surface(const std::string& curve_definition, glm::ivec2 resolution)
{
	compile_shaders();

	if (surface_height_map == nullptr || surface_height_map->get_size() != resolution)
		surface_height_map = std::make_shared<Texture2D>(resolution.x, resolution.y, surface_height_map_internal_format, 1, 0, 0);

	std::string operation_string =
		"float x = id_n.x - 0.5;"
		"float y = id_n.y - 0.5;"
		"imageStore(target_data, ivec2(id.xy), vec4(" + curve_definition + "));";

	operation->compute(
		*surface_height_map,
		operation_string
	);
}

void CurvedSurface::set_position(glm::vec3 position){
	this->position = position;
}

void CurvedSurface::set_rotation(glm::quat quaternion){
	this->quaternion = quaternion;
}

void CurvedSurface::set_scale(glm::vec3 scale){
	this->scale = scale;
}

glm::vec3 CurvedSurface::get_position() {
	return position;
}

glm::quat CurvedSurface::get_rotation() {
	return quaternion;
}

glm::vec3 CurvedSurface::get_scale() {
	return scale;
}

void CurvedSurface::compile_shaders()
{
	if (shaders_compiled)
		return;

	operation = std::make_shared<GraphicsOperation>();
	
	cp_texture_to_mesh = std::make_shared<ComputeProgram>(Shader(curved_surface_parent_shader_path / "Compute/texture2d_to_mesh_triangle.comp"));
	surface_program = std::make_shared<Program>(Shader(curved_surface_parent_shader_path / "CurvedSurfaceRenderer/basic.vert", curved_surface_parent_shader_path / "CurvedSurfaceRenderer/surface.frag"));

	vab = std::make_shared<VertexAttributeBuffer>();

	shaders_compiled = true;
}

void CurvedSurface::generate_mesh_from_texture()
{
	if (surface_height_map == nullptr) {
		std::cout << "[Gozdiscoptics Error] CurvedSurface::generate_mesh_from_texture() is called but no curve have been defined yet" << std::endl;
		ASSERT(false);
	}

	compile_shaders();

	glm::ivec2 resolution = surface_height_map->get_size();
	size_t vertex_buffer_size_in_bytes = resolution.x * resolution.y * sizeof(glm::vec3);
	size_t index_buffer_size_in_bytes = (resolution.x - 1) * (resolution.y - 1) * sizeof(int32_t) * 6;

	bool vertex_buffer_up_to_date = false;
	bool index_buffer_up_to_date = false;

	if (vab->get_buffer_slot(0).buffer == nullptr || vab->get_buffer_slot(0).buffer->get_buffer_size_in_bytes() != vertex_buffer_size_in_bytes) {
		vab->attach_vertex_buffer(0, std::make_shared<Buffer>(vertex_buffer_size_in_bytes), sizeof(glm::vec3), 0, 0);
		vab->set_attribute_format(0, 0, VertexAttributeBuffer::a_f32, 3, 0, true);
	}
	else
		vertex_buffer_up_to_date = true;
	
	if (index_buffer == nullptr || index_buffer->get_buffer_size_in_bytes() != index_buffer_size_in_bytes) {
		index_buffer = std::make_shared<Buffer>(index_buffer_size_in_bytes);
	}
	else
		index_buffer_up_to_date = true;

	if (vertex_buffer_size_in_bytes && index_buffer_up_to_date)
		return;

	ComputeProgram& kernel = *cp_texture_to_mesh;

	kernel.update_uniform("resolution", resolution);
	kernel.update_uniform_as_image("curve_heightmap", *surface_height_map, 0);
	kernel.update_uniform_as_storage_buffer("vertex_buffer", *vab->get_buffer_slot(0).buffer);
	kernel.update_uniform_as_storage_buffer("index_buffer", *index_buffer, 0);
	
	kernel.dispatch_thread(resolution.x, resolution.y, 1);
}

void CurvedSurface::render(Framebuffer& target_framebuffer, Camera& camera){
	Framebuffer previous_framebuffer = Framebuffer::get_current_draw();
	target_framebuffer.bind_draw();
	render(camera);
	previous_framebuffer.bind_draw();
}

void CurvedSurface::render(Camera& camera){
	
	generate_mesh_from_texture();

	Program& program = *surface_program;

	camera.update_matrixes();
	camera.update_default_uniforms(program);

	glm::mat4 model = glm::identity<glm::mat4>();
	model = glm::translate(model, position);
	model = glm::mat4_cast(quaternion) * model;
	model = glm::scale(model, scale);

	program.update_uniform("model", model);
	program.update_uniform("color", glm::vec4(1, 1, 1, 1));

	RenderParameters params(true);
	params.depth_test = true;
	params.cull_face = false;

	primitive_renderer::render(
		program,
		*vab,
		*index_buffer,
		PrimitiveType::triangle,
		IndexType::i_ui32,
		params
	);
}