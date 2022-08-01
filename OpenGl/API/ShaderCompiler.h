#pragma once
#include <GL\glew.h>

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <string>
#include <map>
#include <unordered_map>
#include "Debuger.h"

class Shader {
public:
	std::string vertex_shader;
	std::string fragment_shader;
	std::string geometry_shader;
	Shader();
	Shader(const std::string& target_file);
	Shader(const std::string& vertex_target_file, const std::string& fragment_target_file);
	Shader(const std::string& vertex_target_file, const std::string& geometry_terget_file, const std::string& fragment_target_file);
	void read_shader(const std::string& target_file);
	
};


class Program {
private:
	static unsigned int current_binded_program;
	unsigned int compile_shader(unsigned int type, const std::string& shader_source);
	void _detect_and_define_all_uniforms(const std::string& shader_code);
public:
	unsigned int id = 0;
	std::unordered_map<std::string, unsigned int> uniforms;
	std::vector<unsigned int> uniform_id_shortcuts;
	Program();
	Program(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	Program(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code);
	void compile(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	void compile(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code);
	void bind();
	void unbind();
	void define_uniform(std::string name);
	// template definitions
	template<typename T>
	void update_uniform(std::string name, T a, T b, T c, T d) {
		if (name.find('.') != std::string::npos)
			define_uniform(name);
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform4f(uniforms[name], (float)a, (float)b, (float)c, (float)d));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform4i(uniforms[name], (int)a, (int)b, (int)c, (int)d));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a, T b, T c) {
		if (name.find('.') != std::string::npos)
			define_uniform(name);
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform3f(uniforms[name], (float)a, (float)b, (float)c));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform3i(uniforms[name], (int)a, (int)b, (int)c));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a, T b) {
		if (name.find('.') != std::string::npos)
			define_uniform(name);
		bind();
		if (std::is_same<T, float>::value){
			GLCall(glUniform2f(uniforms[name], (float)a, (float)b));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform2i(uniforms[name], (int)a, (int)b));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a) {
		if (name.find('.') != std::string::npos)
			define_uniform(name);
		bind();
		if (std::is_same<T, float>::value){
			GLCall(glUniform1f(uniforms[name], (float)a));
		}
		else if (std::is_same<T, int>::value){
			GLCall(glUniform1i(uniforms[name], (int)a));
		}
	}

	void update_uniform(std::string name, glm::mat4 a);
	void update_uniform(std::string name, glm::mat3 a);
	void update_uniform(std::string name, glm::mat2 a);
	void update_uniform(std::string name, glm::vec4 a);
	void update_uniform(std::string name, glm::vec3 a);
	void update_uniform(std::string name, glm::vec2 a);

	template<typename T>
	void update_uniform(int shortcut_index, T a, T b, T c, T d) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform4f(uniform_id_shortcuts[shortcut_index], (float)a, (float)b, (float)c, (float)d));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform4i(uniform_id_shortcuts[shortcut_index], (int)a, (int)b, (int)c, (int)d));
		}
	}
	template<typename T>
	void update_uniform(int shortcut_index, T a, T b, T c) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform3f(uniform_id_shortcuts[shortcut_index], (float)a, (float)b, (float)c));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform3i(uniform_id_shortcuts[shortcut_index], (int)a, (int)b, (int)c));
		}
	}
	template<typename T>
	void update_uniform(int shortcut_index, T a, T b) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform2f(uniform_id_shortcuts[shortcut_index], (float)a, (float)b));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform2i(uniform_id_shortcuts[shortcut_index], (int)a, (int)b));
		}
	}
	template<typename T>
	void update_uniform(int shortcut_index, T a) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform1f(uniform_id_shortcuts[shortcut_index], (float)a));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform1i(uniform_id_shortcuts[shortcut_index], (int)a));
		}
	}

	void update_uniform(int shortcut_index, glm::mat4 a);
	void update_uniform(int shortcut_index, glm::mat3 a);
	void update_uniform(int shortcut_index, glm::mat2 a);
	void update_uniform(int shortcut_index, glm::vec4 a);
	void update_uniform(int shortcut_index, glm::vec3 a);
	void update_uniform(int shortcut_index, glm::vec2 a);
};
