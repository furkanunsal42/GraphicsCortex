#include "CortexScene.h"
#include "Prefab.h"

#include "ECS_Systems/TransformHierarchySystem.h"

Entity2 CortexScene::create_entity() {
    Entity2 entity;

    if (!free_indices.empty()) {
        entity.index = free_indices.back();
        free_indices.pop_back();
        entity.generation = entity_generations[entity.index];
    }
    else {
        entity.index = (uint32_t)entity_generations.size();
        entity.generation = 0;
        entity_generations.push_back(entity.generation);
    }

    return entity;
}

Entity2 CortexScene::create_entity(Prefab& prefab)
{
    if (prefab.get_entity_count() == 0) return Entity2::invalid_entity;

    std::vector<Entity2> spawned_entities(prefab.get_entity_count());

    for (size_t i = 0; i < prefab.get_entity_count(); i++) {
        Entity2 new_entity = create_entity();
        spawned_entities[i] = new_entity;

        Prefab::EntityBlueprint& blueprint = prefab.get_entity(i);

        for (const auto& copier : blueprint.copiers) {
            copier(*this, new_entity);
        }
    }
    
    for (size_t i = 0; i < prefab.get_entity_count(); i++) {
        Prefab::EntityBlueprint& blueprint = prefab.get_entity(i);

        if (blueprint.parent_index != -1) {
            Entity2 child_ent = spawned_entities[i];
            Entity2 parent_ent = spawned_entities[blueprint.parent_index];

            if (has<TransformComponent2>(child_ent) && has<TransformComponent2>(parent_ent)) {

                auto& child_transform = access<TransformComponent2>(child_ent);
                auto& parent_transform = access<TransformComponent2>(parent_ent);

                child_transform.parent_id = parent_ent;

                if (parent_transform.child_id == Entity2::invalid_entity) {
                    parent_transform.child_id = child_ent;
                }
                else {
                    Entity2 current_sibling = parent_transform.child_id;
                    while (true) {
                        auto& sibling_transform = access<TransformComponent2>(current_sibling);
                        if (sibling_transform.sibling_id == Entity2::invalid_entity) {
                            sibling_transform.sibling_id = child_ent;
                            break;
                        }
                        current_sibling = sibling_transform.sibling_id;
                    }
                }
            }
        }
    }

    // Return the "Root" entity of the prefab (usually the first one added)
    return spawned_entities[0];
}

void CortexScene::destroy_entity(Entity2 entity) {
    if (!is_valid(entity)) return;

    for (auto& pool : component_pools) {
        if (pool) pool->remove(entity);
    }

    entity_generations[entity.index]++;

    free_indices.push_back(entity.index);
}

bool CortexScene::is_valid(Entity2 entity) {
    return entity.index < entity_generations.size() && entity_generations[entity.index] == entity.generation;
}
