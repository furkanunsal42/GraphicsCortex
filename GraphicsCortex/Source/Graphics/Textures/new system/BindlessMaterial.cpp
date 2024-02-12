#include "BindlessMaterial.h"
#include "Debuger.h"


BindlessMaterial::BindlessMaterial() {}

BindlessMaterial::BindlessMaterial(const std::shared_ptr<Program>& program)
{
	set_program(program);
}

bool BindlessMaterial::operator==(const BindlessMaterial& other)
{
	return program == other.program && _textures == other._textures;
}

void BindlessMaterial::add_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to add_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}
	GLCall(unsigned int location = glGetUniformLocation(program->id, name.c_str()));
	(*_textures)[location] = texture;
	
}

void BindlessMaterial::remove_texture(const std::string& name)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to remove_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}
	GLCall(unsigned int location = glGetUniformLocation(program->id, name.c_str()));
	_textures->erase(location);
}

void BindlessMaterial::set_program(const std::shared_ptr<Program>& new_program)
{
	set_program_without_texture_reset(new_program);
	_textures->clear();
}

void BindlessMaterial::set_program_without_texture_reset(const std::shared_ptr<Program>& new_program)
{
	program = new_program;
}

void BindlessMaterial::update_uniforms()
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] BindlessMaterial tried to update_uniforms() but program was nullptr" << std::endl;
		ASSERT(false);
	}

	program->bind();
	for (auto iterator = _textures->begin(); iterator != _textures->end(); iterator++) {
		unsigned int location = iterator->first;
		std::shared_ptr<Texture2D>& texture = iterator->second;

		if (texture->async_load_happening) texture->wait_async_load();
		if (!texture->_texture_handle_created) texture->_allocate_texture();
		GLCall(glUniformHandleui64ARB(location, texture->texture_handle));
	}
}
