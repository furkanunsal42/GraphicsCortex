#include "EquirectengularProjector.h"
#include "PrimitiveRenderer.h"
#include "Camera.h"

EquirectangularProjector::EquirectangularProjector()
{

}

void EquirectangularProjector::project_to_cubemap(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture)
{
	init();

	Program& program = *to_cubemap_projection_program;
	program.update_uniform("equirectangular_texture", equirectangular_texture);

	Camera cam;
	cam.projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cam.update_default_uniforms(program);

	framebuffer->bind_draw();
	glm::ivec4 old_viewport = primitive_renderer::get_viewport_position_size();
	primitive_renderer::set_viewport(glm::ivec2(0), glm::ivec2(cubemap_texture.get_size()));

	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::RIGHT, 0);
	framebuffer->activate_draw_buffer(0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);

	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cam.update_default_uniforms(program);
	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::LEFT, 0);
	framebuffer->activate_draw_buffer(0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);
	
	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	cam.update_default_uniforms(program);
	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::UP, 0);
	framebuffer->activate_draw_buffer(0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);
	
	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cam.update_default_uniforms(program);
	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::DOWN, 0);
	framebuffer->activate_draw_buffer(0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);
	
	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cam.update_default_uniforms(program);
	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::FRONT, 0);
	framebuffer->activate_draw_buffer(0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);
	
	cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	cam.update_default_uniforms(program);
	framebuffer->attach_color(0, cubemap_texture, TextureCubeMap::BACK, 0);
	primitive_renderer::render(
		*framebuffer,
		program,
		*unit_cube.get_mesh(0),
		RenderParameters()
	);

	primitive_renderer::set_viewport(old_viewport);
	framebuffer->deattach_color(0);
}

void EquirectangularProjector::init()
{
	if (initialized) return;

	framebuffer = std::make_shared<Framebuffer>();

	SingleModel model;
    model.verticies = {
        glm::vec3(-1, -1,  1), //0
        glm::vec3( 1, -1,  1), //1
        glm::vec3(-1,  1,  1), //2
        glm::vec3( 1,  1,  1), //3
        glm::vec3(-1, -1, -1), //4
        glm::vec3( 1, -1, -1), //5
        glm::vec3(-1,  1, -1), //6
        glm::vec3( 1,  1, -1)  //7
    };
    model.indicies = {
        //Top
        2, 6, 7,
        2, 7, 3,
        //Bottom
        0, 5, 4,
        0, 1, 5,
        //Left
        0, 6, 2,
        0, 4, 6,
        //Right
        1, 3, 7,
        1, 7, 5,
        //Front
        0, 2, 3,
        0, 3, 1,
        //Back
        4, 7, 6,
        4, 5, 7
    };
    model.primitive = triangle;

    unit_cube.load_model(std::move(model), IndexType::i_ui32);

	to_cubemap_projection_program = std::make_shared<Program>(Shader(
		default_paths::equirectangular_projection / "equirectangular_to_cubemap_projection.vert",
		default_paths::equirectangular_projection / "equirectangular_to_cubemap_projection.frag"));

	// to_equiengular_projection_program = nullptr;

	initialized = true;
}
