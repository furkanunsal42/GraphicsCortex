#include "BindlessMaterial.h"
#include "Debuger.h"


BindlessMaterial::BindlessMaterial() {
	_texture_uniform_buffer->push_variable_array(_buffer_texture_count);
}

BindlessMaterial::BindlessMaterial(const std::shared_ptr<Program>& program)
{
	_texture_uniform_buffer->push_variable_array(_buffer_texture_count);
	set_program(program);
}

bool BindlessMaterial::operator==(const BindlessMaterial& other)
{
	return program == other.program && _texture_uniform_buffer == other._texture_uniform_buffer;
}

void BindlessMaterial::add_texture(int index, const std::shared_ptr<Texture2D>& texture)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to add_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}

	_textures[index] = texture;
}

void BindlessMaterial::remove_texture(int index)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to remove_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}
	_textures.erase(index);
}

void BindlessMaterial::set_program(const std::shared_ptr<Program>& new_program)
{
	set_program_without_texture_reset(new_program);
	_textures.clear();
}

void BindlessMaterial::set_program_without_texture_reset(const std::shared_ptr<Program>& new_program)
{ 
	if (program != nullptr)
		program->deattach_uniform_buffer(texture_buffer_name);
	program = new_program;
	program->attach_uniform_buffer(texture_buffer_name, _texture_uniform_buffer);
}

void BindlessMaterial::update_uniforms()
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] BindlessMaterial tried to update_uniforms() but program was nullptr" << std::endl;
		ASSERT(false);
	}
	
	program->attach_uniform_buffer(texture_buffer_name, _texture_uniform_buffer);

	for (auto iterator = _textures.begin(); iterator != _textures.end(); iterator++) {
		int location = iterator->first;
		std::shared_ptr<Texture2D>& texture = iterator->second;

		if (texture->async_load_happening) texture->wait_async_load();
		if (!texture->_texture_handle_created) texture->_create_handle();
		
		char* handle = new char[4*4];
		for (int i = 0; i < 4 * 4; i++)
			handle[i] = 0;

		memcpy(handle, &texture->texture_handle, 8);

		_texture_uniform_buffer->set_data(4*4*location, 0, 4*4, handle);
		delete[] handle;


		//GLCall(glUniformHandleui64ARB(location, texture->texture_handle));
	}
	_texture_uniform_buffer->upload_data();
}
