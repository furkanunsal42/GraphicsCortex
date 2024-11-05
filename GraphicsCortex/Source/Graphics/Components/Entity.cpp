#include "Entity.h"

void Entity::update(Component::UpdateType type) {
	for (auto& component_pair : _components) {
		std::shared_ptr<Component> component = component_pair.second;

		switch (type) {

		case Component::UpdateType::Activated	: component->on_activated();	break;
		case Component::UpdateType::Deactivated	: component->on_deactivated();	break;
		case Component::UpdateType::Initialized	: component->on_initialized();	break;
		case Component::UpdateType::Destructed	: component->on_destructed();	break;

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

ComponentId Entity::_generate_id()
{
    ComponentId id = _next_id_to_generate++;
    return id;
}
