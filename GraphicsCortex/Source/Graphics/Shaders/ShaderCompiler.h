#pragma once
#include "Config.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <unordered_map>

#include <GL/glew.h>

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "Package.h"
#include "Debuger.h"

class Shader {
public:
	std::string vertex_shader = "";
	std::string fragment_shader = "";
	std::string geometry_shader = "";
	std::string compute_shader = "";
	
	std::string filename = "";

	Shader();
	Shader(const std::filesystem::path& target_file);
	Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& fragment_target_file);
	Shader(const std::filesystem::path& vertex_target_file, const std::filesystem::path& geometry_target_file, const std::filesystem::path& fragment_target_file);
	void read_shader(const std::filesystem::path& path);
	void read_shader_source(const std::string& source_code);
};

class Texture1D;
class Texture2D;
class Texture3D;
class Texture2DArray;
class TextureCubeMap;
class Buffer;
class UniformBuffer;

class Program {
public:
	unsigned int id = 0;
	
	Program(const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values = {});
	Program(const Shader& shader, const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values = {});
	Program(const Program& other) = delete;
	~Program();
	void release();

	void clear_preprocessor();
	void set_preprocessor(const std::string& key, const std::string& value);
	void set_preprocessor(const std::vector<std::pair<std::string, std::string>>& preprocessing_key_values);
	std::string get_preprocessor(const std::string& key);
	void compile_shader(const Shader& shader);

	void bind();
	void unbind();

	void update_uniform_as_storage_buffer(const std::string& name, Buffer& buffer, size_t offset, size_t size);
	void update_uniform_as_storage_buffer(const std::string& name, Buffer& buffer, size_t offset = 0);

	void update_uniform_as_uniform_buffer(const std::string& name, UniformBuffer& uniform_buffer, size_t offset, size_t size);
	void update_uniform_as_uniform_buffer(const std::string& name, UniformBuffer& uniform_buffer, size_t offset = 0);

	void update_uniform(const std::string& name, Texture1D& texture1d);
	void update_uniform(const std::string& name, Texture2D& texture2d);
	void update_uniform(const std::string& name, Texture3D& texture3d);
	void update_uniform(const std::string& name, Texture2DArray& texture2darray);
	void update_uniform(const std::string& name, TextureCubeMap& texturecubemap);

	void update_uniform_bindless(const std::string& name, Texture1D& texture1d);
	void update_uniform_bindless(const std::string& name, Texture2D& texture2d);
	void update_uniform_bindless(const std::string& name, Texture3D& texture3d);
	void update_uniform_bindless(const std::string& name, Texture2DArray& texture2darray);
	void update_uniform_bindless(const std::string& name, TextureCubeMap& texturecubemap);

	void update_uniform_as_image(const std::string& name, Texture1D& texture1d, int mipmap_level);
	void update_uniform_as_image(const std::string& name, Texture2D& texture2d, int mipmap_level);
	void update_uniform_as_image(const std::string& name, Texture3D& texture3d, int mipmap_level);
	void update_uniform_as_image(const std::string& name, Texture2DArray& texture2darray, int mipmap_level);
	void update_uniform_as_image(const std::string& name, TextureCubeMap& texturecubemap, int mipmap_level);

	void update_uniform(const std::string& name, const int& a);
	void update_uniform(const std::string& name, const int& a, const int& b);
	void update_uniform(const std::string& name, const int& a, const int& b, const int& c);
	void update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d);

	void update_uniform(const std::string& name, const unsigned int& a);
	void update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b);
	void update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c);
	void update_uniform(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d);

	void update_uniform(const std::string& name, const float& a);
	void update_uniform(const std::string& name, const float& a, const float& b);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d);

	void update_uniform(const std::string& name, const glm::vec2& a);
	void update_uniform(const std::string& name, const glm::vec3& a);
	void update_uniform(const std::string& name, const glm::vec4& a);

	void update_uniform(const std::string& name, const glm::ivec2& a);
	void update_uniform(const std::string& name, const glm::ivec3& a);
	void update_uniform(const std::string& name, const glm::ivec4& a);

	void update_uniform(const std::string& name, const glm::uvec2& a);
	void update_uniform(const std::string& name, const glm::uvec3& a);
	void update_uniform(const std::string& name, const glm::uvec4& a);

	void update_uniform(const std::string& name, const glm::mat2& a);
	void update_uniform(const std::string& name, const glm::mat3& a);
	void update_uniform(const std::string& name, const glm::mat4& a);
	void update_uniform(const std::string& name, const glm::mat3x4& a);

private:

	bool _program_generated = false;
	bool _program_compiled = false;

	void _generate_program();

	bool _does_uniform_exist(const std::string& name);
	int _get_uniform_location(const std::string& name);

	Shader current_shader;

	std::unordered_map<std::string, int> _uniform_location_table;
	std::unordered_map<std::string, std::string> _preprocessing_defines;
};

 class UpdatesDefaultUniforms {
 public:
	 virtual void update_default_uniforms(Program& program) { };

 };