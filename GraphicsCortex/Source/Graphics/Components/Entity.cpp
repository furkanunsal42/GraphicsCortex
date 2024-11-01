#include "Entity.h"

void Entity::update(UpdateType type) {
	for (auto& component_pair : _components) {
		std::shared_ptr<Component> component = component_pair.second;

		switch (type) {

		case Activated	: component->on_activated();	break;
		case Deactivated: component->on_deactivated();	break;
		case Initialized: component->on_initialized();	break;
		case Destructed	: component->on_destructed();	break;

		case Begin		: component->on_begin();		break;

		case Update		: component->on_update();		break;
		case FixedUpdate: component->on_fixed_update(); break;

		case PreRender	: component->on_pre_render();	break;
		case PostRender	: component->on_post_render();	break;
		case PrePhysics	: component->on_pre_physics();	break;
		case PostPhysics: component->on_post_physics(); break;

		}
	}
}

ComponentId Entity::_generate_id()
{
    ComponentId id = _next_id_to_generate++;
    return id;
}
