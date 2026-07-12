#pragma once

#include "ECS/SystemPipeline.h"
#include "AdvancedRendering/UnifiedRenderer.h" // Fully decoupled from CortexRenderer!
#include "glm.hpp"
#include <unordered_set>
#include <functional>
#include <variant>

#include "TransformHierarchySystem.h"
#include "GLMCout.h"

#include "Tools/ImageBasedLightingSolver/ImageBasedLightingSolver.h"

struct MeshComponent2 {
    uint32_t mesh_id = UnifiedRenderer::invalid_id;
};

struct MaterialComponent2 {
    uint32_t material_id = UnifiedRenderer::invalid_id;
};

struct RendererComponent2 {
    bool is_transparent         = false;
    uint32_t render_object_id   = UnifiedRenderer::invalid_id;
    uint32_t current_layer_id   = UnifiedRenderer::invalid_id;
};

struct EnvironmentComponent2 {
    
    void set_skybox_texture(std::shared_ptr<TextureCubeMap> skybox_texture);
    void set_skybox_texture(std::shared_ptr<Texture2D> skybox_texture);

    std::variant<
        std::shared_ptr<Texture2D>,
        std::shared_ptr<TextureCubeMap>> skybox_texture;

    std::shared_ptr<TextureCubeMap> irradiance_texture;
    std::shared_ptr<TextureCubeMap> prefiltered_environment_texture;
    std::shared_ptr<Texture2D>      brdf_lut_texture;
};

class RendererSyncSystem : public ISystem {
public:
    using LayerResolver = std::function<uint32_t(Entity2, CortexScene&)>;

    RendererSyncSystem(CortexScene& scene, LayerResolver resolver);
    void update(SystemContext& ctx) override;

    ImageBasedLightingSolver image_based_lighting_solver;

private:

    std::unordered_set<Entity2> dirty_environments;
    std::unordered_set<Entity2> dirty_entities;
    LayerResolver layer_resolver;
};