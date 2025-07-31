#include "Component_Skylight.h"

#include "Windowing/WindowBoundGlobalResources.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Component/Scene.h"
#include "Tools/GraphicsOperation/GraphicsOperation.h"

namespace {

    void render_to_cubemap(Framebuffer& framebuffer, Program& program, Mesh& unit_cube, TextureCubeMap& target_cubemap, uint32_t mipmap_level = 0) {
        
        glm::ivec4 old_viewport = primitive_renderer::get_viewport_position_size();
        primitive_renderer::set_viewport(glm::ivec2(0), glm::ivec2(target_cubemap.get_size().x >> mipmap_level));

        framebuffer.attach_color_layered(0, target_cubemap, mipmap_level);
        framebuffer.activate_draw_buffer(0);
        framebuffer.bind_draw();

        primitive_renderer::render(
            framebuffer,
            program,
            *unit_cube.get_mesh(0),
            RenderParameters()
        );

        primitive_renderer::set_viewport(old_viewport);
        framebuffer.deattach_color(0);
    }
}


SkylightComponent::SkylightComponent(std::shared_ptr<TextureCubeMap> sky_texutre) :
    sky_texture(sky_texture)
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

    std::shared_ptr<TextureCubeMap> target = std::make_shared<TextureCubeMap>(resolution, cubemap_texture_format, std::floor(std::log2(resolution)), 0);
    target->mipmap_min_filter = TextureCubeMap::SamplingFilter::LINEAR;
    projector.project_to_cubemap(equirectangular_texture, *target, 0);
    target->generate_mipmap();
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

void SkylightComponent::set_sky_irradiance_texture(std::shared_ptr<TextureCubeMap> set_sky_irradiance_texture)
{
    this->sky_irradiance_texture = sky_irradiance_texture;
}

std::shared_ptr<TextureCubeMap> SkylightComponent::get_sky_irradiance_texture()
{
    return sky_irradiance_texture;
}

void SkylightComponent::set_sky_prefiltered_texture(std::shared_ptr<TextureCubeMap> prefiltered_texture)
{
    this->sky_prefiltered_texture = prefiltered_texture;
}

std::shared_ptr<TextureCubeMap> SkylightComponent::get_sky_prefiltered_texture()
{
    return sky_prefiltered_texture;
}

void SkylightComponent::set_sky_brdf_texture(std::shared_ptr<Texture2D> brdf_texture)
{
    this->sky_brdf_texture = brdf_texture;
}

std::shared_ptr<Texture2D> SkylightComponent::get_sky_brdf_texture()
{
    return sky_brdf_texture;
}

void SkylightComponent::calculate_sky_irradiance_texture(uint32_t irradiance_texture_resolution) {
    
    if (sky_texture == nullptr) {
        std::cout << "[OpenGL Error] SkylightComponent::calculate_sky_irradiance_texture() is called but sky_texture was nullptr" << std::endl;
        ASSERT(false);
    }

    Program& program = *active_global_resources->SkylightComponent_irradiance_convolver;

    if (sky_irradiance_texture == nullptr || sky_irradiance_texture->get_size().x != irradiance_texture_resolution) {
        sky_irradiance_texture = std::make_shared<TextureCubeMap>(irradiance_texture_resolution, TextureCubeMap::ColorTextureFormat::RGB32F, 1, 0);
    }

    program.update_uniform("source_texture", *sky_texture);

    render_to_cubemap(framebuffer, program, unit_cube, *sky_irradiance_texture);
}

void SkylightComponent::calculate_sky_prefiltered_texture(uint32_t prefiltered_texture_resolution, uint32_t mipmap_count)
{
    if (sky_texture == nullptr) {
        std::cout << "[OpenGL Error] SkylightComponent::calculate_sky_prefiltered_texture() is called but sky_texture was nullptr" << std::endl;
        ASSERT(false);
    }

    Program& program = *active_global_resources->SkylightComponent_prefiltered_convolver;

    if (sky_prefiltered_texture == nullptr || 
        sky_prefiltered_texture->get_size().x != prefiltered_texture_resolution || 
        sky_prefiltered_texture->get_mipmap_count() != mipmap_count) 
    {
        sky_prefiltered_texture = std::make_shared<TextureCubeMap>(prefiltered_texture_resolution, TextureCubeMap::ColorTextureFormat::RGB32F, mipmap_count, 0);
    }

    program.update_uniform("source_texture", *sky_texture);
    program.update_uniform("source_texture_resolution", sky_texture->get_size().x);
    
    for (int32_t i = 0; i < sky_prefiltered_texture->get_mipmap_count(); i++) {
        program.update_uniform("roughness", (float)i / (mipmap_count - 1));
        render_to_cubemap(framebuffer, program, unit_cube, *sky_prefiltered_texture, i);
    }
}

void SkylightComponent::calculate_sky_brdf_texture(uint32_t view_direction_resolution, uint32_t roughness_resolution)
{
    if (sky_texture == nullptr) {
        std::cout << "[OpenGL Error] SkylightComponent::calculate_sky_brdf_texture() is called but sky_texture was nullptr" << std::endl;
        ASSERT(false);
    }
    
    GraphicsOperation& operation = *active_global_resources->SkylightComponent_operation;


    if (sky_brdf_texture == nullptr ||
        sky_brdf_texture->get_size().x != view_direction_resolution ||
        sky_brdf_texture->get_size().y != roughness_resolution)
    {
        sky_brdf_texture = std::make_shared<Texture2D>(view_direction_resolution, roughness_resolution, Texture2D::ColorTextureFormat::RG16F, 1, 0);
    }

    operation.push_constant("SAMPLE_COUNT", 1024);

    operation.set_precomputation_statement(
        "const float PI = 3.14159265359;"
        "float GeometrySchlickGGX(float NdotV, float roughness)"
        "{"
        "    float a = roughness;"
        "    float k = (a * a) / 2.0;"
        ""
        "    float nom   = NdotV;"
        "    float denom = NdotV * (1.0 - k) + k;"
        ""
        "    return nom / denom;"
        "}"
        "float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)"
        "{"
        "    float NdotV = max(dot(N, V), 0.0);"
        "    float NdotL = max(dot(N, L), 0.0);"
        "    float ggx2 = GeometrySchlickGGX(NdotV, roughness);"
        "    float ggx1 = GeometrySchlickGGX(NdotL, roughness);"
        ""
        "    return ggx1 * ggx2;"
        "}  "
        "float RadicalInverse_VdC(uint bits)"
        "{"
        "    bits = (bits << 16u) | (bits >> 16u);"
        "    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);"
        "    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);"
        "    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);"
        "    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);"
        "    return float(bits) * 2.3283064365386963e-10;"
        "}"
        "vec2 Hammersley(uint i, uint N)"
        "{"
        "    return vec2(float(i) / float(N), RadicalInverse_VdC(i));"
        "}"
        "vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)"
        "{"
        "   float a = roughness * roughness;"
        "   "
        "   float phi = 2.0 * PI * Xi.x;"
        "   float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));"
        "   float sinTheta = sqrt(1.0 - cosTheta * cosTheta);"
        "   "
        "   vec3 H;"
        "   H.x = cos(phi) * sinTheta;"
        "   H.y = sin(phi) * sinTheta;"
        "   H.z = cosTheta;"
        "   "
        "   vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);"
        "   vec3 tangent = normalize(cross(up, N));"
        "   vec3 bitangent = cross(N, tangent);"
        "   "
        "   vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;"
        "   return normalize(sampleVec);"
        "}"
        "vec2 IntegrateBRDF(float NdotV, float roughness)"
        "{"
        "   vec3 V;"
        "   V.x = sqrt(1.0 - NdotV * NdotV);"
        "   V.y = 0.0;"
        "   V.z = NdotV;"
        "   "
        "   float A = 0.0;"
        "   float B = 0.0;"
        "   "
        "   vec3 N = vec3(0.0, 0.0, 1.0);"
        "   "
        "   const uint SAMPLE_COUNT = 1024u;"
        "   for (uint i = 0u; i < SAMPLE_COUNT; ++i)"
        "   {"
        "       vec2 Xi = Hammersley(i, SAMPLE_COUNT);"
        "       vec3 H = ImportanceSampleGGX(Xi, N, roughness);"
        "       vec3 L = normalize(2.0 * dot(V, H) * H - V);"
        "   "
        "       float NdotL = max(L.z, 0.0);"
        "       float NdotH = max(H.z, 0.0);"
        "       float VdotH = max(dot(V, H), 0.0);"
        "   "
        "       if (NdotL > 0.0)"
        "       {"
        "           float G = GeometrySmith(N, V, L, roughness);"
        "           float G_Vis = (G * VdotH) / (NdotH * NdotV);"
        "           float Fc = pow(1.0 - VdotH, 5.0);"
        "   "
        "           A += (1.0 - Fc) * G_Vis;"
        "           B += Fc * G_Vis;"
        "       }"
        "   }"
        "   A /= float(SAMPLE_COUNT);"
        "   B /= float(SAMPLE_COUNT);"
        "   return vec2(A, B);"
        "}"
    );

    operation.compute(
        *sky_brdf_texture,
        *sky_texture, true,
        "vec4(IntegrateBRDF(id_n.x, id_n.y), 0, 1)"
    );
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
    if (active_global_resources->SkylightComponent_irradiance_convolver == nullptr)
        active_global_resources->SkylightComponent_irradiance_convolver = std::make_unique<Program>(Shader(
            default_paths::skylight / "irradiance_convolver.vert",
            default_paths::skylight / "irradiance_convolver.geom",
            default_paths::skylight / "irradiance_convolver.frag"
        ));
    if (active_global_resources->SkylightComponent_prefiltered_convolver == nullptr) {
        active_global_resources->SkylightComponent_prefiltered_convolver = std::make_unique<Program>(Shader(
            default_paths::skylight / "prefiltered_convolver.vert",
            default_paths::skylight / "prefiltered_convolver.geom",
            default_paths::skylight / "prefiltered_convolver.frag"
        ));
    }
    if (active_global_resources->SkylightComponent_operation == nullptr) {
        active_global_resources->SkylightComponent_operation = std::make_unique<GraphicsOperation>();
    }
    

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

