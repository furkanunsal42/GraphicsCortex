#include "Component_Skylight.h"

#include "Windowing/WindowBoundGlobalResources.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Component/Scene.h"

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
    program.update_uniform("cubemap_texture", *sky_texture);

    glDepthMask(GL_FALSE);
 
    primitive_renderer::render(
        program,
        *unit_cube.get_mesh(0),
        RenderParameters()
    );
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

