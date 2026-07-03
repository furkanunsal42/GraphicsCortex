#pragma once

#include "ECS/SystemPipeline.h"
#include "AdvancedRendering/UnifiedRenderer.h" // Fully decoupled from CortexRenderer!
#include "glm.hpp"
#include <unordered_set>
#include <functional>

#include "TransformHierarchySystem.h"
#include "GLMCout.h"

struct MeshComponent2 {
    uint32_t mesh_id = UnifiedRenderer::invalid_id;
};

struct MaterialComponent2 {
    uint32_t material_id = UnifiedRenderer::invalid_id;
};

struct RendererComponent2 {
    bool is_transparent = false;
    bool does_cast_shadow = true;
    bool does_recieve_shadow = true;

    uint32_t render_object_id = UnifiedRenderer::invalid_id;
    uint32_t current_layer_id = UnifiedRenderer::invalid_id;
};

class RendererSyncSystem : public ISystem {
public:

    using LayerResolver = std::function<uint32_t(Entity2, CortexScene&)>;

private:
    LayerResolver layer_resolver;
    std::unordered_set<Entity2> dirty_entities;

public:
    RendererSyncSystem(CortexScene& scene, LayerResolver resolver)
        : layer_resolver(std::move(resolver))
    {
        scene.on_add<RendererComponent2>().subscribe([this](CortexScene& s, Entity2 e) {
            auto& renderer_comp = s.access<RendererComponent2>(e);

            renderer_comp.render_object_id = UnifiedRenderer::get().create_object(
                UnifiedRenderer::invalid_id,
                glm::mat4(1.0f)
            );

            // We intentionally defer assigning the layer to the update() loop 
            // so we can validate if the mesh and material exist first!
            renderer_comp.current_layer_id = UnifiedRenderer::invalid_id;

            dirty_entities.insert(e);
            });


        scene.on_remove<RendererComponent2>().subscribe([this](CortexScene& s, Entity2 e) {
            auto& renderer_comp = s.get<RendererComponent2>(e);
            if (renderer_comp.render_object_id != UnifiedRenderer::invalid_id) {


                if (renderer_comp.current_layer_id != UnifiedRenderer::invalid_id) {
                    UnifiedRenderer::get().get_render_layer(renderer_comp.current_layer_id).
                        object_remove(renderer_comp.render_object_id);
                }

                UnifiedRenderer::get().release_object(renderer_comp.render_object_id);
            }
            dirty_entities.erase(e);
            });

        auto mark_dirty = [this](CortexScene& s, Entity2 e) {
            if (s.has<RendererComponent2>(e)) {
                dirty_entities.insert(e);
            }
            };

        scene.on_replace<TransformComponent2>().subscribe(mark_dirty);
        scene.on_replace<MeshComponent2>().subscribe(mark_dirty);
        scene.on_replace<MaterialComponent2>().subscribe(mark_dirty);
        scene.on_replace<RendererComponent2>().subscribe(mark_dirty);
    }

    void update(SystemContext& ctx) override {
        if (dirty_entities.empty()) return;

        for (Entity2 e : dirty_entities) {
            if (!ctx.scene.has<RendererComponent2>(e)) continue;

            auto& renderer_comp = ctx.scene.access<RendererComponent2>(e);
            if (renderer_comp.render_object_id == UnifiedRenderer::invalid_id) continue;

            auto& render_obj = UnifiedRenderer::get().get_object(renderer_comp.render_object_id);

            if (ctx.scene.has<TransformComponent2>(e)) {
                auto& transform_comp = ctx.scene.get<TransformComponent2>(e);
                render_obj.set_model_matrix(transform_comp.global_matrix);
            }

            uint32_t target_mesh = UnifiedRenderer::invalid_id;
            if (ctx.scene.has<MeshComponent2>(e)) {
                auto& mesh_comp = ctx.scene.get<MeshComponent2>(e);
                target_mesh = mesh_comp.mesh_id;
                render_obj.set_mesh(target_mesh);
            }

            uint32_t target_layer = layer_resolver(e, ctx.scene);

            uint32_t target_material = ctx.scene.has<MaterialComponent2>(e)
                ? ctx.scene.get<MaterialComponent2>(e).material_id
                : UnifiedRenderer::invalid_id;

            // PREVENT SUBMISSION: If the mesh or material is invalid, force the layer to invalid.
            // This ensures it gets safely removed from any active pass and prevents rendering.
            if (target_mesh == UnifiedRenderer::invalid_id || target_material == UnifiedRenderer::invalid_id) {
                target_layer = UnifiedRenderer::invalid_id;
            }

            // Handle Dynamic Layer Switching (e.g. an object becomes transparent mid-game)
            if (renderer_comp.current_layer_id != target_layer) {

                // 1. Remove from old pass
                if (renderer_comp.current_layer_id != UnifiedRenderer::invalid_id) {
                    UnifiedRenderer::get().get_render_layer(renderer_comp.current_layer_id).object_remove(renderer_comp.render_object_id);
                }

                // 2. Add to new pass
                if (target_layer != UnifiedRenderer::invalid_id) {
                    UnifiedRenderer::get().get_render_layer(target_layer).object_add(renderer_comp.render_object_id, target_material);
                }

                renderer_comp.current_layer_id = target_layer;
            }
            else {
                // Pass didn't change, just update the material ID
                if (target_layer != UnifiedRenderer::invalid_id) {
                    UnifiedRenderer::get().get_render_layer(target_layer).object_set_material(
                        renderer_comp.render_object_id,
                        target_material
                    );
                }
            }
        }

        dirty_entities.clear();
    }
};