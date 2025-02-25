#include "Component_MeshRenderer.h"

#include "Component/Entity.h"
#include "PrimitiveRenderer.h"

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

void MeshRendererComponent::render(Camera& camera)
{

	MeshComponent* mesh_c = get_mesh();
	if (mesh_c == nullptr) return;

	TransformComponent* transform_c = get_transform();
	if (transform_c == nullptr) return;
	
	MaterialComponent* material_c = get_material();
	if (material_c == nullptr) return;

	std::shared_ptr<Program> program = material_c->get_program();
	if (program == nullptr) return;

	std::shared_ptr<Mesh> mesh = mesh_c->mesh;
	if (mesh == nullptr) return;

	camera.update_matrixes();
	camera.update_default_uniforms(*program);

	mesh->traverse([&](Mesh::Node& node, glm::mat4& transform) {

		if (node.get_submeshes().size() == 0) return;

		program->update_uniform("model", transform * transform_c->transform);
		for (mesh_t submesh : node.get_submeshes()) {

			primitive_renderer::render(
				*program,
				*mesh->get_mesh(submesh),
				RenderParameters()	// get from material or program
			);
		}
			

		});
}
