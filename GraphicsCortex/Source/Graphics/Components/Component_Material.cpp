#include "Component_Material.h"

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program) :
	program(program) {}

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program, const MeshMaterial& material) :
	program(program)
{
	load_mesh_material(material);
}

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program, const MeshMaterial::SingleMaterial& material) :
	program(program)
{
	load_mesh_material(material);
}

void MaterialComponent::set_texture(const std::string& name, std::shared_ptr<Texture2D> texture)
{
	textures[name] = texture;
}

std::shared_ptr<Texture2D> MaterialComponent::get_texture2d(const std::string& name)
{
	auto iterator = textures.find(name);
	if (iterator == textures.end())
		return nullptr;

	return iterator->second;
}

void MaterialComponent::remove_texture(const std::string& name)
{
	textures.erase(name);
}

void MaterialComponent::clear_uniforms()
{
	textures.clear();
}

std::shared_ptr<Program> MaterialComponent::get_program()
{
	return program;
}

void MaterialComponent::set_program(std::shared_ptr<Program> program)
{
	this->program = program;
}

void MaterialComponent::on_pre_render()
{
	if (program == nullptr) return;

	for (auto& uniform : textures) {
		
		// always binds textures as sampler
		program->update_uniform(uniform.first, uniform.second);

	}
}

