#pragma once
#include <GL\glew.h>

#include <glm.hpp>
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <string>
#include <map>
#include "Debuger.h"

struct Shader {
	std::string vertex_shader;
	std::string fragment_shader;
	Shader();
	Shader(const std::string& target_file);
	Shader(const std::string& vertex_target_file, const std::string& fragment_target_file);
	void read_shader(const std::string& target_file);
};


struct Program {
private:
	unsigned int compile_shader(unsigned int type, const std::string& shader_source);
public:
	unsigned int id = 0;
	std::map<std::string, unsigned int> uniforms;
	Program();
	Program(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	void compile(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	void bind();
	void unbind();
	void define_uniform(std::string name);
	// template definitions
	template<typename T>
	void update_uniform(std::string name, T a, T b, T c, T d) {
		if (uniforms.find(name) == uniforms.end())
			define_uniform(name);
		bind();
		if (typeid(T) == typeid(float)){
			GLCall(glUniform4f(uniforms[name], (float)a, (float)b, (float)c, (float)d));
		}
		else if (typeid(T) == typeid(int)){
			GLCall(glUniform4i(uniforms[name], (int)a, (int)b, (int)c, (int)d));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a, T b, T c) {
		if (uniforms.find(name) == uniforms.end())
			define_uniform(name);
		bind();
		if (typeid(T) == typeid(float)){
			GLCall(glUniform3f(uniforms[name], (float)a, (float)b, (float)c));
		}
		else if (typeid(T) == typeid(int)){
			GLCall(glUniform3i(uniforms[name], (int)a, (int)b, (int)c));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a, T b) {
		if (uniforms.find(name) == uniforms.end())
			define_uniform(name);
		bind();
		if (typeid(T) == typeid(float)){
			GLCall(glUniform2f(uniforms[name], (float)a, (float)b));
		}
		else if (typeid(T) == typeid(int)){
			GLCall(glUniform2i(uniforms[name], (int)a, (int)b));
		}
	}
	template<typename T>
	void update_uniform(std::string name, T a) {
		if (uniforms.find(name) == uniforms.end())
			define_uniform(name);
		bind();
		if (typeid(T) == typeid(float)){
			GLCall(glUniform1f(uniforms[name], (float)a));
		}
		else if (typeid(T) == typeid(int)){
			GLCall(glUniform1i(uniforms[name], (int)a));
		}
	}
	void update_uniform(std::string name, glm::mat4 a);
	void update_uniform(std::string name, glm::mat3 a);
	void update_uniform(std::string name, glm::mat2 a);
	void update_uniform(std::string name, glm::vec4 a);
	void update_uniform(std::string name, glm::vec3 a);
	void update_uniform(std::string name, glm::vec2 a);


	

};


