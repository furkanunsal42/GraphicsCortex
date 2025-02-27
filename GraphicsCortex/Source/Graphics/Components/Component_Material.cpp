#include "Component_Material.h"

#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program) :
	program(program) {}

MaterialComponent::MaterialComponent(std::shared_ptr<Program> program, std::shared_ptr<MeshMaterial> material) :
	program(program)
{
	set_mesh_material(material);
}

void MaterialComponent::set_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d)
{
	set_uniform(name, glm::ivec4(a, b, c, d));
}

void MaterialComponent::set_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d)
{
	set_uniform(name, glm::vec4(a, b, c, d));
}

void MaterialComponent::set_uniform(const std::string& name, const int& a, const int& b, const int& c)
{
	set_uniform(name, glm::ivec3(a, b, c));
}

void MaterialComponent::set_uniform(const std::string& name, const float& a, const float& b, const float& c)
{
	set_uniform(name, glm::vec3(a, b, c));
}

void MaterialComponent::set_uniform(const std::string& name, const int& a, const int& b)
{
	set_uniform(name, glm::ivec2(a, b));
}

void MaterialComponent::set_uniform(const std::string& name, const float& a, const float& b)
{
	set_uniform(name, glm::vec2(a, b));
}

void MaterialComponent::set_uniform(const std::string& name, const int& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::int32;
	auto address_value = (int*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const float& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::float32;
	auto address_value = (float*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::mat4& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::mat4;
	auto address_value = (glm::mat4*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::mat3& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::mat3;
	auto address_value = (glm::mat3*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;

}

void MaterialComponent::set_uniform(const std::string& name, const glm::mat2& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::mat2;
	auto address_value = (glm::mat2*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;

}

void MaterialComponent::set_uniform(const std::string& name, const glm::vec4& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::vec4;
	auto address_value = (glm::vec4*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::vec3& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::vec3;
	auto address_value = (glm::vec3*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::vec2& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::vec2;
	auto address_value = (glm::vec2*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::ivec4& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::ivec4;
	auto address_value = (glm::ivec4*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::ivec3& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::ivec3;
	auto address_value = (glm::ivec3*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;
}

void MaterialComponent::set_uniform(const std::string& name, const glm::ivec2& a)
{
	auto iterator = name_to_primitive_uniform_location.find(name);
	size_t location;
	if (iterator != name_to_primitive_uniform_location.end()) {
		location = iterator->second;
	}
	else {
		location = primitive_uniforms_buffer.size();
		primitive_uniforms_buffer.resize(primitive_uniforms_buffer.size() + sizeof(UniformType) + sizeof(a));
	}

	auto address_type = (UniformType*)&primitive_uniforms_buffer[location];
	*address_type = UniformType::ivec2;
	auto address_value = (glm::ivec2*)(address_type + 1);
	*address_value = a;

	name_to_primitive_uniform_location[name] = location;

}

void MaterialComponent::set_uniform(const std::string& name, std::shared_ptr<TextureBase2> texture)
{
	textures[name] = texture;
}

void MaterialComponent::set_uniform(const std::string& name, std::shared_ptr<Buffer> buffer)
{
	buffers[name] = buffer;
}

void MaterialComponent::set_uniform(const std::string& name, std::shared_ptr<UniformBuffer> uniform_buffer)
{
	uniform_buffers[name] = uniform_buffer;
}

std::shared_ptr<TextureBase2> MaterialComponent::get_uniform_texture(const std::string& name)
{
	auto iterator = textures.find(name);
	if (iterator == textures.end())
		return nullptr;

	std::shared_ptr<TextureBase2> texture = std::dynamic_pointer_cast<TextureBase2, TextureBase2>(iterator->second);
	return texture;
}

std::shared_ptr<Buffer> MaterialComponent::get_uniform_buffer(const std::string& name)
{
	auto iterator = buffers.find(name);
	if (iterator == buffers.end())
		return nullptr;

	return iterator->second;
}

std::shared_ptr<UniformBuffer> MaterialComponent::get_uniform_uniform_buffer(const std::string& name)
{
	auto iterator = uniform_buffers.find(name);
	if (iterator == uniform_buffers.end())
		return nullptr;

	return iterator->second;
}

void MaterialComponent::remove_uniform(const std::string& name)
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

void MaterialComponent::set_mesh_material(std::shared_ptr<MeshMaterial> mesh_material)
{
	this->mesh_material = mesh_material;
}

std::shared_ptr<MeshMaterial> MaterialComponent::get_mesh_material()
{
	return mesh_material;
}

void MaterialComponent::set_render_parameters(RenderParameters params)
{
	render_parameters = params;
}

RenderParameters& MaterialComponent::get_render_parameters()
{
	return render_parameters;
}

void MaterialComponent::update_uniforms()
{
	if (program == nullptr) return;

	for (auto& pair : name_to_primitive_uniform_location) {
		const std::string& name = pair.first;
		size_t location = pair.second;
		UniformType* type = (UniformType*)&primitive_uniforms_buffer[location];
		void* value_address = type  + 1;
		switch (*type) {
		case mat4:		program->update_uniform(name, *(glm::mat4*)(value_address));		break;
		case mat3:		program->update_uniform(name, *(glm::mat3*)(value_address));		break;
		case mat2:		program->update_uniform(name, *(glm::mat2*)(value_address));		break;
		case vec4:		program->update_uniform(name, *(glm::vec4*)(value_address));		break;
		case vec3:		program->update_uniform(name, *(glm::vec3*)(value_address));		break;
		case vec2:		program->update_uniform(name, *(glm::vec2*)(value_address));		break;
		case ivec4:		program->update_uniform(name, *(glm::ivec4*)(value_address));		break;
		case ivec3:		program->update_uniform(name, *(glm::ivec3*)(value_address));		break;
		case ivec2:		program->update_uniform(name, *(glm::ivec2*)(value_address));		break;
		case int32:		program->update_uniform(name, *(int32_t*)(value_address));			break;
		case float32:	program->update_uniform(name, *(float*)(value_address));			break;
		}
	}


	for (auto& uniform : textures) {

		{
			std::shared_ptr<Texture1D> tex_1d = std::dynamic_pointer_cast<Texture1D, TextureBase2>(uniform.second);
			if (tex_1d != nullptr)
				program->update_uniform(uniform.first, *tex_1d);
		}
		
		{
			std::shared_ptr<Texture2D> tex_2d = std::dynamic_pointer_cast<Texture2D, TextureBase2>(uniform.second);
			if (tex_2d != nullptr)
				program->update_uniform(uniform.first, *tex_2d);
		}

		{
			std::shared_ptr<Texture3D> tex_3d = std::dynamic_pointer_cast<Texture3D, TextureBase2>(uniform.second);
			if (tex_3d != nullptr)
				program->update_uniform(uniform.first, *tex_3d);
		}

		{
			std::shared_ptr<Texture2DArray> tex_2d_array = std::dynamic_pointer_cast<Texture2DArray, TextureBase2>(uniform.second);
			if (tex_2d_array != nullptr)
				program->update_uniform(uniform.first, *tex_2d_array);
		}

		{
			std::shared_ptr<TextureCubeMap> tex_cube = std::dynamic_pointer_cast<TextureCubeMap, TextureBase2>(uniform.second);
			if (tex_cube != nullptr)
				program->update_uniform(uniform.first, *tex_cube);
		}
	}
}

void MaterialComponent::on_pre_render()
{
	update_uniforms();
}

uint32_t MaterialComponent::get_UniformType_size_in_bytes(UniformType type)
{

	switch (type) {
	case mat4:		return sizeof(glm::mat4);
	case mat3:		return sizeof(glm::mat3);
	case mat2:		return sizeof(glm::mat2);
	case vec4:		return sizeof(glm::vec4);
	case vec3:		return sizeof(glm::vec3);
	case vec2:		return sizeof(glm::vec2);
	case ivec4:		return sizeof(glm::ivec4);
	case ivec3:		return sizeof(glm::ivec3);
	case ivec2:		return sizeof(glm::ivec2);
	case int32:		return sizeof(int32_t);
	case float32:	return sizeof(float);
	}
	
	std::cout << "[OpenGL Error] MaterialComponent::get_UniformType_size_in_bytes() is called but given MaterialComponent::UniformType isn't supported" << std::endl;
	ASSERT(false);
	return 0;
}

