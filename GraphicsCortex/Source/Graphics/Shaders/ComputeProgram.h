#pragma once
#include "ShaderCompiler.h"

class ComputeProgram {
public:
	unsigned int id;

	ComputeProgram();
	ComputeProgram(const Shader& shader);
	ComputeProgram(const ComputeProgram& other) = delete;
	~ComputeProgram();
	void release();

	void dispatch(int workgroup_size_x, int workgroup_size_y, int workgroup_size_z);
	void bind();
	void unbind();

	void load_shader(const Shader& shader);

	void update_uniform(const std::string& name, Texture2D& texture2d);
	void update_uniform(const std::string& name, TextureCubeMap& texturecubemap);
	
	void update_uniform(const std::string& name, const int& a);
	void update_uniform(const std::string& name, const int& a, const int& b);
	void update_uniform(const std::string& name, const int& a, const int& b, const int& c);
	void update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d);

	void update_uniform(const std::string& name, const float& a);
	void update_uniform(const std::string& name, const float& a, const float& b);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d);

	void update_uniform(const std::string& name, const glm::vec2& a);
	void update_uniform(const std::string& name, const glm::vec3& a);
	void update_uniform(const std::string& name, const glm::vec4& a);

	void update_uniform(const std::string& name, const glm::mat2& a);
	void update_uniform(const std::string& name, const glm::mat3& a);
	void update_uniform(const std::string& name, const glm::mat4& a);

private:

	bool _program_generated = false;
	bool _program_compiled = false;

	void _generate_program();

	int _get_uniform_location(const std::string& name);
	std::unordered_map<std::string, int> _uniform_location_table;
};