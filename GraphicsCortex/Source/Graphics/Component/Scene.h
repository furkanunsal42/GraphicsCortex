#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <span>
#include <optional>
#include "Component.h"

class Entity;

class Scene {
public:

	Scene() = default;
	~Scene();

	void add_entity(std::shared_ptr<Entity> entity);
	void remove_entity(std::shared_ptr<Entity> entity);

	std::span<std::shared_ptr<Entity>> get_entities();
	template<typename ComponentType>
	std::span<ComponentType*> get_components();
	
	void tick();

private:
	friend Entity;
	std::vector<std::shared_ptr<Entity>> entities;
	std::unordered_map<component_type_id, std::vector<Component*>> type_to_components;
};

#include "SceneTemplated.h"