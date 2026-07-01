#pragma once
#include "Prefab.h"
#include "CortexScene.h"

inline int32_t Prefab::create_entity(int32_t parent_index) {
    int32_t new_index = static_cast<int32_t>(blueprints.size());

    EntityBlueprint bp;
    bp.parent_index = parent_index;
    blueprints.push_back(bp);

    return new_index;
}

inline Prefab::EntityBlueprint& Prefab::get_entity(int32_t index) {
    return blueprints[index];
}

inline size_t Prefab::get_entity_count() const {
    return blueprints.size();
}

template<typename T>
void Prefab::EntityBlueprint::add(const T& component) {
    copiers.push_back([component](CortexScene& scene, Entity2 target) {
        scene.add<T>(target, component);
        });
}