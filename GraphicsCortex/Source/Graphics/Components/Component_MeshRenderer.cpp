#include "Component_MeshRenderer.h"

#include "Component/Entity.h"

MaterialComponent* MeshRendererComponent::get_material() 
{
	Entity* e = get_entity();
	if (e == nullptr) return nullptr;

	if (auto material_component_sp = e->get_component<MaterialComponent>().lock()) {
		return material_component_sp.get();
	}
	else return nullptr;
}

MeshComponent* MeshRendererComponent::get_mesh() 
{
	Entity* e = get_entity();
	if (e == nullptr) return nullptr;

	if (auto mesh_component_sp = e->get_component<MeshComponent>().lock()) {
		return mesh_component_sp.get();
	}
	else return nullptr;
}

TransformComponent* MeshRendererComponent::get_transform()
{
	Entity* e = get_entity();
	if (e == nullptr) return nullptr;

	if (auto transform_component_sp = e->get_component<TransformComponent>().lock()) {
		return transform_component_sp.get();
	}
	else return nullptr;
}
