#include "Scene.h"
#include <algorithm>

#include "Entity.h"

Scene::~Scene()
{
	for (std::shared_ptr<Entity> entity : entities) {
		std::vector<Scene*>& e_scenes = entity->_scenes;
		e_scenes.erase(std::find(e_scenes.begin(), e_scenes.end(), this));
	}
}

void Scene::add_entity(std::shared_ptr<Entity> entity)
{
	auto iterator = std::find(entities.begin(), entities.end(), entity);
	if (iterator != entities.end()) return;
	else {
		entities.push_back(entity);
		entity->_scenes.push_back(this);

		for (auto& pair : entity->_components) {
			component_type_id type = pair.first;
			std::shared_ptr<Component> component = pair.second;
			type_to_components[type].push_back(component.get());
			component->on_added_to_scene(*this);
		}
	}

	//entity->update(Component::Begin);
}

void Scene::remove_entity(std::shared_ptr<Entity> entity)
{
	auto iterator = std::find(entities.begin(), entities.end(), entity);
	if (iterator == entities.end()) return;

	for (auto& pair : entity->_components) {
		component_type_id type = pair.first;
		std::shared_ptr<Component> component = pair.second;
		auto& vector = type_to_components[type];
		vector.erase(std::find(vector.begin(), vector.end(), component.get()));
		component->on_removed_from_scene(*this);
	}

	std::vector<Scene*>& e_scenes = entity->_scenes;
	e_scenes.erase(std::find(e_scenes.begin(), e_scenes.end(), this));

	entities.erase(std::remove(entities.begin(), entities.end(), entity));
}

void Scene::tick()
{
	for (auto& entity : entities) {
		entity->update(Component::Update);
	}
}

std::span<std::shared_ptr<Entity>> Scene::get_entities() {
	return entities;
}

