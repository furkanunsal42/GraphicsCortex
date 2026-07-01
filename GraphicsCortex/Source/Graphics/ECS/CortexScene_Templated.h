#pragma once
#include "CortexScene.h"

template<typename T>
CortexScene::Event& CortexScene::on_add() {
    uint32_t id = ComponentType::get_id<T>();
    if (id >= on_add_events.size()) on_add_events.resize(id + 1);
    return on_add_events[id];
}

template<typename T>
CortexScene::Event& CortexScene::on_remove() {
    uint32_t id = ComponentType::get_id<T>();
    if (id >= on_remove_events.size()) on_remove_events.resize(id + 1);
    return on_remove_events[id];
}

template<typename T>
CortexScene::Event& CortexScene::on_replace() {
    uint32_t id = ComponentType::get_id<T>();
    if (id >= on_replace_events.size()) on_replace_events.resize(id + 1);
    return on_replace_events[id];
}

template<typename T>
void CortexScene::add(Entity2 entity, T component) {
    uint32_t type_id = ComponentType::get_id<T>();

    if (type_id >= component_pools.size()) {
        component_pools.resize(type_id + 1);
    }
    if (!component_pools[type_id]) {
        component_pools[type_id] = std::make_unique<SparseSet<T>>();
    }

    static_cast<SparseSet<T>*>(component_pools[type_id].get())->add(entity, component);

    if (type_id < on_add_events.size()) {
        on_add_events[type_id].publish(*this, entity);
    }
}

template<typename T>
void CortexScene::remove(Entity2 entity) {
    uint32_t type_id = ComponentType::get_id<T>();

    if (type_id < on_remove_events.size()) {
        on_remove_events[type_id].publish(*this, entity);
    }

    if (type_id < component_pools.size() && component_pools[type_id]) {
        static_cast<SparseSet<T>*>(component_pools[type_id].get())->remove(entity);
    }
}

template<typename T>
T& CortexScene::replace(Entity2 entity) {
    uint32_t type_id = ComponentType::get_id<T>();

    if (type_id < on_replace_events.size()) {
        on_replace_events[type_id].publish(*this, entity);
    }

    return static_cast<SparseSet<T>*>(component_pools[type_id].get())->get(entity);
}

template<typename T>
const T& CortexScene::get(Entity2 entity) const {
    uint32_t type_id = ComponentType::get_id<T>();
    return static_cast<SparseSet<T>*>(component_pools[type_id].get())->get(entity);
}

template<typename T>
T& CortexScene::access(Entity2 entity) {
    uint32_t type_id = ComponentType::get_id<T>();
    return static_cast<SparseSet<T>*>(component_pools[type_id].get())->get(entity);
}

template<typename T>
bool CortexScene::has(Entity2 entity) {
    uint32_t type_id = ComponentType::get_id<T>();
    if (type_id >= component_pools.size() || !component_pools[type_id]) return false;
    return static_cast<SparseSet<T>*>(component_pools[type_id].get())->has(entity);
}

template<typename T>
std::vector<T>& CortexScene::get_dense_array() {
    uint32_t type_id = ComponentType::get_id<T>();
    return static_cast<SparseSet<T>*>(component_pools[type_id].get())->dense_components;
}

template<typename T, typename... Other>
CortexScene::View<T, Other...>::View(CortexScene* s, std::vector<Entity2>* entities) : scene(s), dense_entities(entities) {}

template<typename T, typename... Other>
void CortexScene::View<T, Other...>::Iterator::advance_to_valid() {
    while (it != end) {
        if constexpr (sizeof...(Other) > 0) {
            // C++17 Fold Expression: checks all 'Other' components instantly
            if ((scene->has<Other>(*it) && ...)) {
                break;
            }
        }
        else {
            break; // No other components to check, entity is valid!
        }
        ++it;
    }
}
template<typename T, typename... Other>
CortexScene::View<T, Other...>::Iterator::Iterator(CortexScene* s, entity_iterator start, entity_iterator finish) : 
    scene(s), it(start), end(finish) 
{
    advance_to_valid();
}

template<typename T, typename... Other>
CortexScene::View<T, Other...>::Iterator& CortexScene::View<T, Other...>::Iterator::operator++() {
    ++it;
    advance_to_valid();
    return *this;
}
template<typename T, typename... Other>
bool CortexScene::View<T, Other...>::Iterator::operator!=(const Iterator& other) const 
{ 
    return it != other.it; 
}

template<typename T, typename... Other>
Entity2 CortexScene::View<T, Other...>::Iterator::operator*() const 
{ 
    return *it; 
}
    
template<typename T, typename... Other>
CortexScene::View<T, Other...>::Iterator CortexScene::View<T, Other...>::begin()
{
    if (dense_entities) return Iterator(scene, dense_entities->begin(), dense_entities->end());
    static std::vector<Entity2> empty;
    return Iterator(scene, empty.begin(), empty.end());
}

template<typename T, typename... Other>
CortexScene::View<T, Other...>::Iterator CortexScene::View<T, Other...>::end()
{
    if (dense_entities) return Iterator(scene, dense_entities->end(), dense_entities->end());
    static std::vector<Entity2> empty;
    return Iterator(scene, empty.end(), empty.end());
}


template<typename T, typename... Other>
CortexScene::View<T, Other...> CortexScene::view()
{
    uint32_t type_id = ComponentType::get_id<T>();
    std::vector<Entity2>* dense_entities = nullptr;

    if (type_id < component_pools.size() && component_pools[type_id]) {
        // We use the dense entities array of the FIRST component as our base iterator
        dense_entities = &static_cast<SparseSet<T>*>(component_pools[type_id].get())->dense_entities;
    }

    return View<T, Other...>(this, dense_entities);
}

template<typename T>
uint32_t CortexScene::ComponentType::get_id() {
    static uint32_t id = next_type_id++;
    return id;
}
