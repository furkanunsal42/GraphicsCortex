#include "Component_Skylight.h"

#include "Windowing/WindowBoundGlobalResources.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Component/Scene.h"

namespace {

    void render_to_cubemap(Framebuffer& framebuffer, Program& program, Mesh& unit_cube, TextureCubeMap& target_cubemap) {
        // TODO !!!
        std::shared_ptr<TextureCubeMap> shared_cubemap_texture = std::make_shared<TextureCubeMap>(std::move(target_cubemap));

        Camera cam;
        cam.projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cam.update_default_uniforms(program);

        framebuffer.bind_draw();
        glm::ivec4 old_viewport = primitive_renderer::get_viewport_position_size();
        primitive_renderer::set_viewport(glm::ivec2(0), glm::ivec2(shared_cubemap_texture->get_size()));

        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::RIGHT, 0);
        framebuffer.activate_draw_buffer(0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cam.update_default_uniforms(program);
        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::LEFT, 0);
        framebuffer.activate_draw_buffer(0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        cam.update_default_uniforms(program);
        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::UP, 0);
        framebuffer.activate_draw_buffer(0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        cam.update_default_uniforms(program);
        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::DOWN, 0);
        framebuffer.activate_draw_buffer(0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cam.update_default_uniforms(program);
        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::FRONT, 0);
        framebuffer.activate_draw_buffer(0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        cam.view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        cam.update_default_uniforms(program);
        framebuffer.attach_color(0, shared_cubemap_texture, TextureCubeMap::BACK, 0);
        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        primitive_renderer::set_viewport(old_viewport);
        framebuffer.deattach_color(0);
        target_cubemap = std::move(*shared_cubemap_texture);
        shared_cubemap_texture = nullptr;
    }
}

SkylightComponent::SkylightComponent(std::shared_ptr<TextureCubeMap> sky_texutre, std::shared_ptr<TextureCubeMap> sky_texture_convoluted) :
    sky_texture(sky_texture), sky_texture_convoluted(sky_texture_convoluted) 
{
    
}

void SkylightComponent::make_primary()
{
    std::span<Scene*> scenes = get_entity()->get_scenes();

    // TODO, there might be a light that is primary for another scene which this light is not a part of, to fix it entities should not be part of multiple scenes
    for (Scene* scene : scenes) {
        std::span<SkylightComponent*> skylights = scene->get_components<SkylightComponent>();
        for (SkylightComponent* skylight : skylights)
            skylight->is_primary_skylight = false;
    }

    is_primary_skylight = true;
}

bool SkylightComponent::is_primary()
{
    return is_primary_skylight;
}

void SkylightComponent::set_sky_texture(Texture2D& equirectangular_texture, uint32_t resolution, TextureCubeMap::ColorTextureFormat cubemap_texture_format)
{
    init();

    EquirectangularProjector& projector = *active_global_resources->SkylightComponent_projector.get();

    std::shared_ptr<TextureCubeMap> target = std::make_shared<TextureCubeMap>(resolution, cubemap_texture_format, 1, 0);
    projector.project_to_cubemap(equirectangular_texture, *target);

    set_sky_texture(target);
}

void SkylightComponent::set_sky_texture(std::shared_ptr<TextureCubeMap> sky_texture)
{
    this->sky_texture = sky_texture;
}

std::shared_ptr<TextureCubeMap> SkylightComponent::get_sky_texture()
{
    return sky_texture;
}

void SkylightComponent::set_sky_texture_convoluted(std::shared_ptr<TextureCubeMap> sky_texture_convoluted)
{
    this->sky_texture_convoluted = sky_texture_convoluted;
}

std::shared_ptr<TextureCubeMap> SkylightComponent::get_sky_texture_convoluted()
{
    return sky_texture_convoluted;
}

void SkylightComponent::calculate_sky_texture_convoluted(uint32_t convoluted_resolution) {
    Program& program = *active_global_resources->SkylightComponent_convolver;

    if (sky_texture_convoluted == nullptr || sky_texture_convoluted->get_size().x != convoluted_resolution) {
        sky_texture_convoluted = std::make_shared<TextureCubeMap>(convoluted_resolution, TextureCubeMap::ColorTextureFormat::RGB16F, 1, 0);
    }

    program.update_uniform("source_texture", *sky_texture);

    render_to_cubemap(framebuffer, program, unit_cube, *sky_texture_convoluted);
}

void SkylightComponent::render(Framebuffer& framebuffer, Camera& camera)
{
    if (sky_texture == nullptr) return;

    framebuffer.bind_draw();
    render(camera);
}

void SkylightComponent::render(Camera& camera)
{
    if (sky_texture == nullptr) return;
    
    init();

    Program& program = *active_global_resources->SkylightComponent_render;

    camera.update_matrixes();
    camera.update_default_uniforms(program);
    program.update_uniform("cubemap_texture", *sky_texture_convoluted);

    // TEMP
    glDepthMask(GL_FALSE);
    bool face_culling = glIsEnabled(GL_CULL_FACE);
    int depth_function;
    glGetIntegerv(GL_DEPTH_FUNC, &depth_function);
    GLCall(glDisable(GL_CULL_FACE));
    GLCall(glDepthFunc(GL_LEQUAL));

    primitive_renderer::render(
        program,
        *unit_cube.get_mesh(0),
        RenderParameters()
    );

    glDepthMask(GL_TRUE);
    if (face_culling) {
        GLCall(glEnable(GL_CULL_FACE));
    }
    GLCall(glDepthFunc(depth_function));

}

void SkylightComponent::on_added_to_scene(Scene& scene) {
    make_primary();
}

void SkylightComponent::init()
{
    if (initialized) return;

    if (active_global_resources->SkylightComponent_projector == nullptr)
        active_global_resources->SkylightComponent_projector = std::make_unique<EquirectangularProjector>();
    if (active_global_resources->SkylightComponent_render == nullptr)
        active_global_resources->SkylightComponent_render = std::make_unique<Program>(Shader(
            default_paths::skylight / "cubemap.vert",
            default_paths::skylight / "cubemap.frag"
        ));
    if (active_global_resources->SkylightComponent_convolver == nullptr)
        active_global_resources->SkylightComponent_convolver = std::make_unique<Program>(Shader(
            default_paths::skylight / "convolver.vert",
            default_paths::skylight / "convolver.frag"
        ));

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

    initialized = true;
}

