#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "Component.h"
#include "Scene.h"

class Entity {
public:

	Entity() = default;
	Entity(const Entity& other) = delete;
	
	virtual ~Entity() = default;

	template<typename ComponentType, typename... ArgType>
	void add_component(ArgType... arguments);
	template<typename ComponentType>
	void remove_component();
	template<typename ComponentType>
	std::weak_ptr<ComponentType> get_component();

private:
	friend Scene;

	std::vector<Scene*> _scenes;

	void update(Component::UpdateType type);
	std::unordered_map<component_type_id, std::shared_ptr<Component>> _components;
};

#include "EntityTemplated.h"