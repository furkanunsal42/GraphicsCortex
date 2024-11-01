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

	enum UpdateType {
		Activated,
		Deactivated,
		Initialized,
		Destructed,

		Begin,
		
		Update,
		FixedUpdate,
		
		PreRender,
		PostRender,
		PrePhysics,
		PostPhysics,
	};

	template<typename ComponentType>
	void add_component();
	template<typename ComponentType>
	void remove_component();
	template<typename ComponentType>
	std::weak_ptr<ComponentType> get_component();

	void update(UpdateType type);

private:

	ComponentId _next_id_to_generate = 0;
	ComponentId _generate_id();

	std::unordered_map<size_t, std::shared_ptr<Component>> _components;
};

#include "EntityTemplated.hpp"


