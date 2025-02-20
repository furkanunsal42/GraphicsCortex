#pragma once

#include "Entity.h"

template<typename ComponentType, typename... ArgType>
void Entity::add_component(ArgType... arguments)
{
    component_type_id type = typeid(ComponentType).hash_code();

    if (_components.find(type) != _components.end()) {
        std::cout << "Component already exist" << std::endl;
        return;
    }

    std::shared_ptr<Component> component = std::make_shared<ComponentType>(arguments...);
    component->entity = this;

    for (Scene* scene : _scenes) {
        scene->type_to_components[type].push_back(component.get());
    }

    _components[type] = component;
}

template<typename ComponentType>
void Entity::remove_component()
{
    component_type_id type = typeid(ComponentType).hash_code();

    if (_components.find(type) == _components.end()) return;

    for (Scene* scene : _scenes) {
        std::vector<std::weak_ptr<Component>>& scene_components = scene->type_to_components[type];
        scene_components.erase(std::find(scene_components.begin(), scene_components.end(), _components.at(type)));
    }

    _components.erase(type);
}

template<typename ComponentType>
std::weak_ptr<ComponentType> Entity::get_component()
{
    component_type_id type = typeid(ComponentType).hash_code();
    auto iterator = _components.find(type);
    
    if (iterator == _components.end())
        return std::weak_ptr<ComponentType>();

    std::weak_ptr<ComponentType> weak = std::dynamic_pointer_cast<ComponentType, Component>((*iterator).second);
    return weak;
}