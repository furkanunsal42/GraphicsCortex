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
	std::string vertex_shader = "";
	std::string fragment_shader = "";
	std::string geometry_shader = "";

	std::string filename = "";

	Shader();
	Shader(const std::string& target_file);
	Shader(const std::string& vertex_target_file, const std::string& fragment_target_file);
	Shader(const std::string& vertex_target_file, const std::string& geometry_terget_file, const std::string& fragment_target_file);
	void read_shader(const std::string& target_file);
	
};


struct UniformData {
	enum Type {
		FLOAT,
		FLOAT_VEC2,
		FLOAT_VEC3,
		FLOAT_VEC4,
		DOUBLE,
		DOUBLE_VEC2,
		DOUBLE_VEC3,
		DOUBLE_VEC4,
		INT,
		INT_VEC2,
		INT_VEC3,
		INT_VEC4,
		UNSIGNED_INT,
		UNSIGNED_INT_VEC2,
		UNSIGNED_INT_VEC3,
		UNSIGNED_INT_VEC4,
		BOOL,
		BOOL_VEC2,
		BOOL_VEC3,
		BOOL_VEC4,
		FLOAT_MAT2,
		FLOAT_MAT3,
		FLOAT_MAT4,
		FLOAT_MAT2x3,
		FLOAT_MAT2x4,
		FLOAT_MAT3x2,
		FLOAT_MAT3x4,
		FLOAT_MAT4x2,
		FLOAT_MAT4x3,
		DOUBLE_MAT2,
		DOUBLE_MAT3,
		DOUBLE_MAT4,
		DOUBLE_MAT2x3,
		DOUBLE_MAT2x4,
		DOUBLE_MAT3x2,
		DOUBLE_MAT3x4,
		DOUBLE_MAT4x2,
		DOUBLE_MAT4x3,
		SAMPLER_1D,
		SAMPLER_2D,
		SAMPLER_3D,
		SAMPLER_CUBE,
		SAMPLER_1D_SHADOW,
		SAMPLER_2D_SHADOW,
		SAMPLER_1D_ARRAY,
		SAMPLER_2D_ARRAY,
		SAMPLER_1D_ARRAY_SHADOW,
		SAMPLER_2D_ARRAY_SHADOW,
		SAMPLER_2D_MULTISAMPLE,
		SAMPLER_2D_MULTISAMPLE_ARRAY,
		SAMPLER_CUBE_SHADOW,
		SAMPLER_BUFFER,
		SAMPLER_2D_RECT,
		SAMPLER_2D_RECT_SHADOW,
		INT_SAMPLER_1D,
		INT_SAMPLER_2D,
		INT_SAMPLER_3D,
		INT_SAMPLER_CUBE,
		INT_SAMPLER_1D_ARRAY,
		INT_SAMPLER_2D_ARRAY,
		INT_SAMPLER_2D_MULTISAMPLE,
		INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		INT_SAMPLER_BUFFER,
		INT_SAMPLER_2D_RECT,
		UNSIGNED_INT_SAMPLER_1D,
		UNSIGNED_INT_SAMPLER_2D,
		UNSIGNED_INT_SAMPLER_3D,
		UNSIGNED_INT_SAMPLER_CUBE,
		UNSIGNED_INT_SAMPLER_1D_ARRAY,
		UNSIGNED_INT_SAMPLER_2D_ARRAY,
		UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
		UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
		UNSIGNED_INT_SAMPLER_BUFFER,
		UNSIGNED_INT_SAMPLER_2D_RECT,
		IMAGE_1D,
		IMAGE_2D,
		IMAGE_3D,
		IMAGE_2D_RECT,
		IMAGE_CUBE,
		IMAGE_BUFFER,
		IMAGE_1D_ARRAY,
		IMAGE_2D_ARRAY,
		IMAGE_2D_MULTISAMPLE,
		IMAGE_2D_MULTISAMPLE_ARRAY,
		INT_IMAGE_1D,
		INT_IMAGE_2D,
		INT_IMAGE_3D,
		INT_IMAGE_2D_RECT,
		INT_IMAGE_CUBE,
		INT_IMAGE_BUFFER,
		INT_IMAGE_1D_ARRAY,
		INT_IMAGE_2D_ARRAY,
		INT_IMAGE_2D_MULTISAMPLE,
		INT_IMAGE_2D_MULTISAMPLE_ARRAY,
		UNSIGNED_INT_IMAGE_1D,
		UNSIGNED_INT_IMAGE_2D,
		UNSIGNED_INT_IMAGE_3D,
		UNSIGNED_INT_IMAGE_2D_RECT,
		UNSIGNED_INT_IMAGE_CUBE,
		UNSIGNED_INT_IMAGE_BUFFER,
		UNSIGNED_INT_IMAGE_1D_ARRAY,
		UNSIGNED_INT_IMAGE_2D_ARRAY,
		UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
		UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
		UNSIGNED_INT_ATOMIC_COUNTER,
		UNKNOWN,
	};

	UniformData() {
		name = "";
		type = UNKNOWN;
		index = -1;
	}

	UniformData(std::string name, Type type, unsigned int index) {
		this->name = name;
		this->type = type;
		this->index = index;
	}

	Type type;
	std::string name;
	unsigned int index;
};

struct AttributeData {
	enum Type {
		Unknown,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4,
	};

	enum Purpose {
		VertexPosition,
		VertexNormal,
		TextureCoordinate,
		Custom,
	};

	Type type;
	std::string name;
	unsigned int id;
	int layout_index;
};

class Program {
private:
	unsigned int compile_shader(unsigned int type, const std::string& shader_source);
	void _define_all_uniforms();
public:
	unsigned int id = 0;
	std::unordered_map<std::string, UniformData> uniforms;
	std::unordered_map<std::string, AttributeData> attributes;

	Program();
	Program(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	Program(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code);
	explicit Program(const Shader& shader);
	
	void compile(const std::string& vertex_shader_code, const std::string& fragment_shader_code);
	void compile(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code);
	void bind();
	void unbind();

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
	template<typename T>
	void update_uniform(unsigned int uniform_id, const T& a) {
		std::cout << "[Opengl Error] Program.update_uniform(const unsigned int& uniform_id, const T& a) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(unsigned int uniform_id, const T& a, const T& b) {
		std::cout << "[Opengl Error] Program.update_uniform(const unsigned int& uniform_id, const T& a, const T& b) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(unsigned int uniform_id, const T& a, const T& b, const T& c) {
		std::cout << "[Opengl Error] Program.update_uniform(const unsigned int& uniform_id, const T& a, const T& b, const T& c) has called but " << typeid(T).name() << " isn't supported. \n";
		return;
	}
	template<typename T>
	void update_uniform(unsigned int uniform_id, const T& a, const T& b, const T& c, const T& d) {
		std::cout << "[Opengl Error] Program.update_uniform(const unsigned int& uniform_id, const T& a, const T& b, const T& c, const T& d) has called but " << typeid(T).name() << " isn't supported. \n";
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

	void update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c, const int& d);
	void update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c, const float& d);
	void update_uniform(unsigned int uniform_id, const int& a, const int& b, const int& c);
	void update_uniform(unsigned int uniform_id, const float& a, const float& b, const float& c);
	void update_uniform(unsigned int uniform_id, const int& a, const int& b);
	void update_uniform(unsigned int uniform_id, const float& a, const float& b);
	void update_uniform(unsigned int uniform_id, const int& a);
	void update_uniform(unsigned int uniform_id, const float& a);
	void update_uniform(unsigned int uniform_id, glm::mat4& a);
	void update_uniform(unsigned int uniform_id, glm::mat3& a);
	void update_uniform(unsigned int uniform_id, glm::mat2& a);
	void update_uniform(unsigned int uniform_id, glm::vec4& a);
	void update_uniform(unsigned int uniform_id, glm::vec3& a);
	void update_uniform(unsigned int uniform_id, glm::vec2& a);
};

 class UpdatesDefaultUniforms {
 public:
	 virtual void update_default_uniforms(Program& program) { };

 };