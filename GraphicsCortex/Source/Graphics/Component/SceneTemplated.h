#pragma once
#include "Scene.h"

template<typename ComponentType>
inline std::span<ComponentType*> Scene::get_components()
{
	component_type_id type = typeid(ComponentType).hash_code();

	auto iterator = type_to_components.find(type);
	if (iterator == type_to_components.end()) return std::span<ComponentType*>();

	return std::span<ComponentType*>(*((std::vector<ComponentType*>*)&iterator->second));

}
