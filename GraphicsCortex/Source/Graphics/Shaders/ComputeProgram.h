#pragma once
#include "ShaderCompiler.h"

class ComputeProgram {
public:
	unsigned int id;

	enum MemoryBarrierType {
		VERTEX_ATTRIB_ARRAY_BARRIER_BIT		= 0x00000001,
		ELEMENT_ARRAY_BARRIER_BIT			= 0x00000002,
		UNIFORM_BARRIER_BIT					= 0x00000004,
		TEXTURE_FETCH_BARRIER_BIT			= 0x00000008,
		SHADER_IMAGE_ACCESS_BARRIER_BIT		= 0x00000020,
		COMMAND_BARRIER_BIT					= 0x00000040,
		PIXEL_BUFFER_BARRIER_BIT			= 0x00000080,
		TEXTURE_UPDATE_BARRIER_BIT			= 0x00000100,
		BUFFER_UPDATE_BARRIER_BIT			= 0x00000200,
		FRAMEBUFFER_BARRIER_BIT				= 0x00000400,
		TRANSFORM_FEEDBACK_BARRIER_BIT		= 0x00000800,
		ATOMIC_COUNTER_BARRIER_BIT			= 0x00001000,
		SHADER_STORAGE_BARRIER_BIT			= 0x00002000,
		QUERY_BUFFER_BARRIER_BIT			= 0x00008000,
		ALL_BARRIER_BITS					= 0xFFFFFFFF,
	};

	ComputeProgram();
	ComputeProgram(const Shader& shader, std::vector<std::pair<std::string, std::string>> preprocessing_key_values = {});
	ComputeProgram(const ComputeProgram& other) = delete;
	~ComputeProgram();
	void release();

	void dispatch(int workgroup_size_x, int workgroup_size_y, int workgroup_size_z);
	void dispatch_thread(int thread_count_x, int thread_count_y, int thread_count_z);
	void dispatch_without_barrier(int workgroup_size_x, int workgroup_size_y, int workgroup_size_z);
	void dispatch_thread_without_barrier(int thread_count_x, int thread_count_y, int thread_count_z);
	void memory_barrier(MemoryBarrierType barrier);

	void bind();
	void unbind();

	void clear_preprocessor(const std::string& key, const std::string& value);
	void set_preprocessor(const std::string& key, const std::string& value);
	std::string get_preprocessor(const std::string& key, const std::string& value);
	void compile_shader(const Shader& shader);
	
	glm::ivec3 get_work_group_size();

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

	void update_uniform_as_slice(const std::string& name, Texture3D& texture3d, int mipmap_level, int layer);
	void update_uniform_as_slice(const std::string& name, Texture2DArray& texture3d, int mipmap_level, int layer);
	void update_uniform_as_slice(const std::string& name, TextureCubeMap& texture3d, int mipmap_level, int layer);

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

private:

	bool _program_generated = false;
	bool _program_compiled = false;

	glm::ivec3 _work_group_size;

	void _generate_program();

	bool _does_uniform_exist(const std::string& name);
	int _get_uniform_location(const std::string& name);

	Shader current_shader;

	std::unordered_map<std::string, int> _uniform_location_table;
	std::unordered_map<std::string, std::string> _preprocessing_defines;
};