#include "Component_Material.h"

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program) :
	_program(program) {}

void MaterialComponent::set_texture(const std::string& name, std::shared_ptr<Texture2D> texture, TexturePurpose texture_purpose)
{
	_textures[name] = _texture_with_purpose(texture, texture_purpose);
}

std::shared_ptr<Texture2D> MaterialComponent::get_texture2d(const std::string& name)
{
	auto iterator = _textures.find(name);
	if (iterator == _textures.end())
		return nullptr;

	return iterator->second._texture;
}

MaterialComponent::TexturePurpose MaterialComponent::get_texture2d_purpose(const std::string& name)
{
	auto iterator = _textures.find(name);
	if (iterator == _textures.end())
		return MaterialComponent::GeneralTexture;

	return iterator->second._purpose;
}

void MaterialComponent::remove_texture(const std::string& name)
{
	_textures.erase(name);
}

std::shared_ptr<Program> MaterialComponent::get_program()
{
	return _program;
}

