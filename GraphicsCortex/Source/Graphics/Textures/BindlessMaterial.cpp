#include "BindlessMaterial.h"
#include "Debuger.h"


BindlessMaterial::BindlessMaterial() {
	_cluster_texture_uniform_buffer->push_variable_array(_cluster_buffer_texture_count);

	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_albedo_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_normal_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_metalic_roughness_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_brdf_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_emmisive
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_ambient_occlusion
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_albedo
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_metalic_roughness
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_emmisive
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 albedo
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 normal
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 metalic
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 roughness
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 brdf
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 emmisive
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 ambient_occlusion
}																	 

BindlessMaterial::BindlessMaterial(const std::shared_ptr<Program>& program)
{
	_cluster_texture_uniform_buffer->push_variable_array(_cluster_buffer_texture_count);

	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_albedo_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_normal_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_metalic_roughness_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_brdf_map
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_emmisive
	_pbr_texture_uniform_buffer->push_variable<bool>();			// use_ambient_occlusion
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_albedo
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_metalic_roughness
	_pbr_texture_uniform_buffer->push_variable<glm::vec4>();	// const_emmisive
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 albedo
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 normal
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 metalic
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 roughness
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 brdf
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 emmisive
	_pbr_texture_uniform_buffer->push_variable<Texture2D>();	//	 ambient_occlusion

	set_program(program);
}

bool BindlessMaterial::operator==(const BindlessMaterial& other)
{
	return program == other.program && _cluster_texture_uniform_buffer == other._cluster_texture_uniform_buffer;
}

void BindlessMaterial::add_texture(int index, const std::shared_ptr<Texture2D>& texture)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to add_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}

	_cluster_textures[index] = texture;
}

void BindlessMaterial::remove_texture(int index)
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] Bindless Material tried to remove_texture() but program wasn't set yet" << std::endl;
		ASSERT(false);
	}
	_cluster_textures.erase(index);
}

void BindlessMaterial::set_albedo(glm::vec4 color)
{
	_pbr_const_albedo = color;
}

void BindlessMaterial::set_metalic(float value)
{
	_pbr_const_metalic_roughness.x = value;
}

void BindlessMaterial::set_roughness(float value)
{
	_pbr_const_metalic_roughness.y = value;
}

void BindlessMaterial::set_emmisive(glm::vec3 color)
{
	_pbr_const_emmisive.x = color.x;
	_pbr_const_emmisive.y = color.y;
	_pbr_const_emmisive.z = color.z;
}

void BindlessMaterial::add_albedo_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_albedo = texture;
}

void BindlessMaterial::add_normal_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_normal = texture;
}

void BindlessMaterial::add_metalic_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_metalic = texture;
}

void BindlessMaterial::add_roughness_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_roughness = texture;
}

void BindlessMaterial::add_brdf_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_brdf = texture;
}

void BindlessMaterial::add_emmisive_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_emmisive = texture;
}

void BindlessMaterial::add_ambient_occlusion_texture(std::shared_ptr<Texture2D> texture)
{
	_pbr_ambient_occlusion = texture;
}

void BindlessMaterial::set_program(const std::shared_ptr<Program>& new_program)
{
	set_program_without_texture_reset(new_program);
	_cluster_textures.clear();
}

void BindlessMaterial::set_program_without_texture_reset(const std::shared_ptr<Program>& new_program)
{ 
	if (program != nullptr) {
		program->deattach_uniform_buffer(_cluster_texture_buffer_name);
		program->deattach_uniform_buffer(_pbr_material_buffer_name);
	}
	program = new_program;
	program->attach_uniform_buffer(_cluster_texture_buffer_name, _cluster_texture_uniform_buffer);
	program->attach_uniform_buffer(_pbr_material_buffer_name, _pbr_texture_uniform_buffer);
}

void BindlessMaterial::update_uniforms()
{
	if (program == nullptr) {
		std::cout << "[OpenGL Error] BindlessMaterial tried to update_uniforms() but program was nullptr" << std::endl;
		ASSERT(false);
	}
	
	program->attach_uniform_buffer(_cluster_texture_buffer_name, _cluster_texture_uniform_buffer);

	for (auto iterator = _cluster_textures.begin(); iterator != _cluster_textures.end(); iterator++) {
		int location = iterator->first;
		std::shared_ptr<Texture2D>& texture = iterator->second;

		if (texture->async_load_happening) texture->wait_async_load();
		if (!texture->_texture_handle_created) texture->_create_handle();
		
		char* handle = new char[4*4];
		for (int i = 0; i < 4 * 4; i++)
			handle[i] = 0;

		memcpy(handle, &texture->texture_handle, 8);

		_cluster_texture_uniform_buffer->set_data(4*4*location, 0, 4*4, handle);
		delete[] handle;


		//GLCall(glUniformHandleui64ARB(location, texture->texture_handle));
	}
	_cluster_texture_uniform_buffer->upload_data();


}
