#pragma once

#include "Entity.h"

template<typename ComponentType, typename... ArgType>
void Entity::add_component(ArgType... arguments)
{
    size_t component_id_hash = typeid(ComponentType).hash_code();

    if (_components.find(component_id_hash) != _components.end()) {
        std::cout << "Component already exist" << std::endl;
        return;
    }

    _components[typeid(ComponentType).hash_code()] = std::make_shared<ComponentType>(arguments...);
}

template<typename ComponentType>
void Entity::remove_component()
{
    size_t component_id_hash = typeid(ComponentType).hash_code();

    _components.erase(component_id_hash);
}

template<typename ComponentType>
std::weak_ptr<ComponentType> Entity::get_component()
{
    size_t component_id_hash = typeid(ComponentType).hash_code();
    auto iterator = _components.find(component_id_hash);
    
    if (iterator == _components.end())
        return std::weak_ptr<ComponentType>();

    std::weak_ptr<ComponentType> weak = std::dynamic_pointer_cast<ComponentType, Component>((*iterator).second);
    return weak;
}