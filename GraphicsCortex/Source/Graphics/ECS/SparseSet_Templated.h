#pragma once
#include "SparseSet.h"

inline uint32_t ISparseSet::get_entity_index(Entity2 e)
{
    return e.index;
}

inline uint32_t ISparseSet::get_entity_generation(Entity2 e)
{
    return e.generation;
}

template<typename T>
void SparseSet<T>::add(Entity2 e, T component) {
    uint32_t idx = get_entity_index(e);
    
    if (idx >= sparse.size()) {
        sparse.resize(idx + 1, invalid_index);
    }

    sparse[idx] = (uint32_t)dense_components.size();
    dense_components.push_back(component);
    dense_entities.push_back(e);
}

template<typename T>
T& SparseSet<T>::get(Entity2 e) {
    return dense_components[sparse[get_entity_index(e)]];
}

template<typename T>
bool SparseSet<T>::has(Entity2 e) const {
    uint32_t idx = get_entity_index(e);
    return idx < sparse.size() && sparse[idx] != invalid_index;
}

template<typename T>
void SparseSet<T>::remove(Entity2 e) {
    uint32_t idx = get_entity_index(e);
    if (!has(e)) return;

    uint32_t deleted_dense_index = sparse[idx];
    uint32_t last_dense_index = (uint32_t)dense_components.size() - 1;

    Entity2 last_entity = dense_entities[last_dense_index];

    dense_components[deleted_dense_index] = dense_components[last_dense_index];
    dense_entities[deleted_dense_index] = last_entity;

    sparse[get_entity_index(last_entity)] = deleted_dense_index;

    sparse[idx] = invalid_index;
    dense_components.pop_back();
    dense_entities.pop_back();
}

template<typename T>
void SparseSet<T>::clear() {
    sparse.clear();
    dense_components.clear();
    dense_entities.clear();
}