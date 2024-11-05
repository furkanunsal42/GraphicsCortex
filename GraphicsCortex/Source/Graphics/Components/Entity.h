#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "Component.h"

typedef uint32_t ComponentId;

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

	void update(Component::UpdateType type);

private:

	ComponentId _next_id_to_generate = 0;
	ComponentId _generate_id();

	std::unordered_map<size_t, std::shared_ptr<Component>> _components;
};

#include "EntityTemplated.h"


