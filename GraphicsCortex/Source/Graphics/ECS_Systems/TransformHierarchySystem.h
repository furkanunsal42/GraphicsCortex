#pragma once
#include "ECS/SystemPipeline.h"
#include "glm.hpp"
#include "gtc/quaternion.hpp"
#include <unordered_set>
#include <vector>
#include <algorithm>

struct TransformComponent2 {
    glm::vec3 local_position    = glm::vec3(0);
    glm::quat local_rotation    = glm::identity<glm::quat>();
    glm::vec3 local_scale       = glm::vec3(1);

    glm::mat4 global_matrix = glm::identity<glm::mat4>();

    Entity2 parent_id   = Entity2::invalid_entity;
    Entity2 child_id    = Entity2::invalid_entity;
    Entity2 sibling_id  = Entity2::invalid_entity;
};

class TransformHierarchySystem : public ISystem {
private:
    std::unordered_set<Entity2> dirty_entities;
    bool disable_listening = false;

public:
    TransformHierarchySystem(CortexScene& scene) {
        // Subscribe to changes. We just flag them as dirty here.
        auto mark_dirty = [this](CortexScene& s, Entity2 e) {
            if (disable_listening) return;
            dirty_entities.insert(e);
            };

        scene.on_add<TransformComponent2>().subscribe(mark_dirty);
        scene.on_replace<TransformComponent2>().subscribe(mark_dirty);
    }

    void update(SystemContext& ctx) override {
        if (dirty_entities.empty()) return;

        // 1. Identify the unique roots of the dirty subtrees
        // We only want to start propagation from the highest dirty ancestor to prevent redundant math.
        std::vector<Entity2> roots_to_process;
        for (Entity2 e : dirty_entities) {
            Entity2 highest_dirty = e;
            Entity2 current = ctx.scene.get<TransformComponent2>(e).parent_id;

            while (current != Entity2::invalid_entity) {
                if (dirty_entities.find(current) != dirty_entities.end()) {
                    highest_dirty = current;
                }
                current = ctx.scene.get<TransformComponent2>(current).parent_id;
            }

            if (std::find(roots_to_process.begin(), roots_to_process.end(), highest_dirty) == roots_to_process.end()) {
                roots_to_process.push_back(highest_dirty);
            }
        }

        // 2. Propagate changes downward
        for (Entity2 root : roots_to_process) {
            // IMPORTANT: We use access() instead of replace() to silently mutate the data.
            // If we used replace(), the system would trigger its own events endlessly!
            disable_listening = true;
            auto& transform = ctx.scene.replace<TransformComponent2>(root);
            disable_listening = false;

            glm::mat4 parent_matrix = glm::identity<glm::mat4>();
            if (transform.parent_id != Entity2::invalid_entity) {
                parent_matrix = ctx.scene.get<TransformComponent2>(transform.parent_id).global_matrix;
            }

            transform.global_matrix = parent_matrix * calculate_local_matrix(transform);

            if (transform.child_id != Entity2::invalid_entity) {
                update_children(ctx.scene, transform.child_id, transform.global_matrix);
            }
        }

        dirty_entities.clear();
    }

private:
    glm::mat4 calculate_local_matrix(const TransformComponent2& t) {
        glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), t.local_position);
        glm::mat4 r_mat = glm::mat4_cast(t.local_rotation);
        glm::mat4 s_mat = glm::scale(glm::mat4(1.0f), t.local_scale);
        return t_mat * r_mat * s_mat;
    }

    void update_children(CortexScene& scene, Entity2 first_child, const glm::mat4& parent_matrix) {
        Entity2 current_child = first_child;

        while (current_child != Entity2::invalid_entity) {
            disable_listening = true;
            auto& child_transform = scene.replace<TransformComponent2>(current_child);
            disable_listening = false;

            glm::mat4 local_matrix = calculate_local_matrix(child_transform);
            child_transform.global_matrix = parent_matrix * local_matrix;

            if (child_transform.child_id != Entity2::invalid_entity) {
                update_children(scene, child_transform.child_id, child_transform.global_matrix);
            }

            current_child = child_transform.sibling_id;
        }
    }
};