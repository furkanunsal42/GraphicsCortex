#include "BindlessMaterial.h"
#include "Debuger.h"


BindlessMaterial::BindlessMaterial() { }

BindlessMaterial::BindlessMaterial(const std::shared_ptr<Program>& program)
{
	set_program(program);
}

void BindlessMaterial::add_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture)
{
	GLCall(unsigned int location = glGetUniformLocation(program->id, name.c_str()));
	_textures[location] = texture;
	
}

void BindlessMaterial::remove_texture(const std::string& name)
{
	GLCall(unsigned int location = glGetUniformLocation(program->id, name.c_str()));
	_textures.erase(location);
}

void BindlessMaterial::set_program(const std::shared_ptr<Program>& new_program)
{
	program = new_program;
	_textures.clear();
}

void BindlessMaterial::update_uniforms()
{
	program->bind();
	for (auto iterator = _textures.begin(); iterator != _textures.end(); iterator++) {
		unsigned int location = iterator->first;
		std::shared_ptr<Texture2D>& texture = iterator->second;

		if (!texture->_texture_handle_created) texture->_allocate_texture();
		GLCall(glUniformHandleui64ARB(location, texture->texture_handle));
	}
}