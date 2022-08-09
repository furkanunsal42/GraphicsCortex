#pragma once
#include "Config.h"

#include <GL\glew.h>

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <string>
#include <map>
#include <unordered_map>
#include "Debuger.h"

#include <iostream>

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
	void define_uniform(const std::string& name);
	// template definitions
	
	// for non-supported types
	template<typename T>
	void update_uniform(const std::string& name, const T& a) {
		std::cout << "[Opengl Error] Program.update_uniform(const std::string& name, const T& a) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(const std::string& name, const T& a, const T& b) {
		std::cout << "[Opengl Error] Program.update_uniform(const std::string& name, const T& a, const T& b) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(const std::string& name, const T& a, const T& b, const T& c) {
		std::cout << "[Opengl Error] Program.update_uniform(const std::string& name, const T& a, const T& b, const T& c) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(const std::string& name, const T& a, const T& b, const T& c, const T& d) {
		std::cout << "[Opengl Error] Program.update_uniform(const std::string& name, const T& a, const T& b, const T& c, const T& d) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}

	void update_uniform(const std::string& name, const int& a, const int& b, const int& c, const int& d);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c, const float& d);
	void update_uniform(const std::string& name, const int& a, const int& b, const int& c);
	void update_uniform(const std::string& name, const float& a, const float& b, const float& c);
	void update_uniform(const std::string& name, const int& a, const int& b);
	void update_uniform(const std::string& name, const float& a, const float& b);
	void update_uniform(const std::string& name, const int& a);
	void update_uniform(const std::string& name, const float& a);
	void update_uniform(const std::string& name, const glm::mat4& a);
	void update_uniform(const std::string& name, const glm::mat3& a);
	void update_uniform(const std::string& name, const glm::mat2& a);
	void update_uniform(const std::string& name, const glm::vec4& a);
	void update_uniform(const std::string& name, const glm::vec3& a);
	void update_uniform(const std::string& name, const glm::vec2& a);

	template<typename T>
	void update_uniform(const int& uniform_id, const T& a, const T& b, const T& c, const T& d) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform4f(uniform_id, (float)a, (float)b, (float)c, (float)d));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform4i(uniform_id, (int)a, (int)b, (int)c, (int)d));
		}
	}
	template<typename T>
	void update_uniform(const int& uniform_id, const T& a, const T& b, const T& c) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform3f(uniform_id, (float)a, (float)b, (float)c));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform3i(uniform_id, (int)a, (int)b, (int)c));
		}
	}
	template<typename T>
	void update_uniform(const int& uniform_id, const T& a, const T& b) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform2f(uniform_id, (float)a, (float)b));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform2i(uniform_id, (int)a, (int)b));
		}
	}
	template<typename T>
	void update_uniform(const int& uniform_id, const T& a) {
		bind();
		if (std::is_same<T, float>::value) {
			GLCall(glUniform1f(uniform_id, (float)a));
		}
		else if (std::is_same<T, int>::value) {
			GLCall(glUniform1i(uniform_id, (int)a));
		}
	}

	void update_uniform(const int& uniform_id, const glm::mat4& a);
	void update_uniform(const int& uniform_id, const glm::mat3& a);
	void update_uniform(const int& uniform_id, const glm::mat2& a);
	void update_uniform(const int& uniform_id, const glm::vec4& a);
	void update_uniform(const int& uniform_id, const glm::vec3& a);
	void update_uniform(const int& uniform_id, const glm::vec2& a);
};
