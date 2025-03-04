#include "Entity.h"

Entity::~Entity() {
	for (auto& component : _components) {
		component.second->entity = nullptr;
	}
}

std::span<Scene*> Entity::get_scenes()
{
	return _scenes;
}

void Entity::update(Component::UpdateType type) {
	for (auto& component_pair : _components) {
		std::shared_ptr<Component> component = component_pair.second;
		switch (type) {

		case Component::UpdateType::AddedToScene	: break;
		case Component::UpdateType::RemovedFromScene: break;

		case Component::UpdateType::Begin		: component->on_begin();		break;
		case Component::UpdateType::Update		: component->on_update();		break;
		case Component::UpdateType::FixedUpdate	: component->on_fixed_update();	break;

		case Component::UpdateType::PreRender	: component->on_pre_render();	break;
		case Component::UpdateType::PostRender	: component->on_post_render();	break;
		case Component::UpdateType::PrePhysics	: component->on_pre_physics();	break;
		case Component::UpdateType::PostPhysics	: component->on_post_physics(); break;

		}
	}
}
