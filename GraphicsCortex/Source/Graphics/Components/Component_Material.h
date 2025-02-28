#pragma once
#include "Component/Component.h"

#include <unordered_map>
#include <string>

class TextureBase2;
class Texture1D;
class Texture2D;
class Texture3D;
class Texture2DArray;
class TextureCubeMap;

#include "Buffer.h"
#include "UniformBuffer.h"
#include "ShaderCompiler.h"

#include "Materials/MeshMaterial.h"
#include "RenderParameters.h"

class MaterialComponent : public Component {
public:

	MaterialComponent(std::shared_ptr<Program> program);
	MaterialComponent(std::shared_ptr<Program> program, std::shared_ptr<MeshMaterial> material);

	void set_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d);
	void set_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d);
	void set_uniform(const std::string& name, const int& a, const int& b, const int& c);
	void set_uniform(const std::string& name, const float& a, const float& b, const float& c);
	void set_uniform(const std::string& name, const int& a, const int& b);
	void set_uniform(const std::string& name, const float& a, const float& b);
	void set_uniform(const std::string& name, const int& a);
	void set_uniform(const std::string& name, const float& a);
	void set_uniform(const std::string& name, const glm::mat4& a);
	void set_uniform(const std::string& name, const glm::mat3& a);
	void set_uniform(const std::string& name, const glm::mat2& a);
	void set_uniform(const std::string& name, const glm::vec4& a);
	void set_uniform(const std::string& name, const glm::vec3& a);
	void set_uniform(const std::string& name, const glm::vec2& a);
	void set_uniform(const std::string& name, const glm::ivec4& a);
	void set_uniform(const std::string& name, const glm::ivec3& a);
	void set_uniform(const std::string& name, const glm::ivec2& a);

	void set_uniform(const std::string& name, std::shared_ptr<TextureBase2> texture);
	void set_uniform(const std::string& name, std::shared_ptr<Buffer> buffer);
	void set_uniform(const std::string& name, std::shared_ptr<UniformBuffer> uniform_buffer);

	std::shared_ptr<TextureBase2> get_uniform_texture(const std::string& name);
	std::shared_ptr<Buffer> get_uniform_buffer(const std::string& name);
	std::shared_ptr<UniformBuffer> get_uniform_uniform_buffer(const std::string& name);

	void remove_uniform(const std::string& name);
	void clear_uniforms();

	std::shared_ptr<Program> get_program();
	void set_program(std::shared_ptr<Program> program);

	void set_mesh_material(std::shared_ptr<MeshMaterial> mesh_material);
	std::shared_ptr<MeshMaterial> get_mesh_material();

	void set_render_parameters(RenderParameters params);
	RenderParameters& get_render_parameters();

	void update_uniforms();

private:

	enum UniformType {
		mat4,
		mat3,
		mat2,
		vec4,
		vec3,
		vec2,
		ivec4,
		ivec3,
		ivec2,
		int32,
		float32,
	};

	uint32_t get_UniformType_size_in_bytes(UniformType type);

	std::vector<uint8_t> primitive_uniforms_buffer;
	std::unordered_map<std::string, size_t> name_to_primitive_uniform_location;

	std::unordered_map<std::string, std::shared_ptr<TextureBase2>> textures;
	std::unordered_map<std::string, std::shared_ptr<Buffer>> buffers;
	std::unordered_map<std::string, std::shared_ptr<UniformBuffer>> uniform_buffers;

	RenderParameters render_parameters;
	std::shared_ptr<MeshMaterial> mesh_material = nullptr;
	std::shared_ptr<Program> program = nullptr;
};

#include "Component_MaterialTemplated.h"