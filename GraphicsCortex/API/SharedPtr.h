#pragma once
#include "Config.h"

#include <memory>
#include <type_traits>

#include "Texture.h"
#include "Mesh.h"
#include "ShaderCompiler.h"

template<typename T>
class SharedPtr{
public:

	SharedPtr(std::shared_ptr<T> other_obj) :
		obj(other_obj) {}

	explicit SharedPtr(T& other) :
		obj(std::make_shared<T>(other)) {}

	explicit SharedPtr(T&& other) :
		obj(std::make_shared<T>(other)) {}

	T* operator->() const { return obj.get();  }

	std::shared_ptr<T> obj;
};

#define _SHARED_POINTER_DEFINITIONS(type)				\
	type##_s(std::shared_ptr<type> other_obj) :			\
		obj(other_obj) {}								\
														\
	explicit type##_s(type& other) :					\
		obj(std::make_shared<type>(other)) {}			\
														\
	explicit type##_s(type&& other) :					\
		obj(std::make_shared<type>(other)) {}			\
														\
	type* operator->() const { return obj.get(); }		\
														\
	std::shared_ptr<type> obj;			


class PxRigidActor;		// physx		// implemented

class ArrayBuffer;	// not implemented as shared_pointers yet
class IndexBuffer;	// not implemented as shared_pointers yet

typedef SharedPtr<PxRigidActor> PxRigidActor_s;		// physx


class UnorderedMaterial_s {
public:

	_SHARED_POINTER_DEFINITIONS(UnorderedMaterial)

	UnorderedMaterial_s() :
		obj(std::make_shared<UnorderedMaterial>()) {}

	UnorderedMaterial_s(int size) :
		obj(std::make_shared<UnorderedMaterial>(size)) {}

	UnorderedMaterial_s(const std::string& filename) :
		obj(std::make_shared<UnorderedMaterial>(filename)) {}

};

class Material_s {
public:

	_SHARED_POINTER_DEFINITIONS(Material)

	Material_s() :
		obj(std::make_shared<Material>()) {}

};

class Mesh_s {
public:

	_SHARED_POINTER_DEFINITIONS(Mesh)

	Mesh_s() :
		obj(std::make_shared<Mesh>()) {}

	Mesh_s(ArrayBuffer& array_buffer, IndexBuffer& index_buffer) :
		obj(std::make_shared<Mesh>(array_buffer, index_buffer)) {}

	Mesh_s(ArrayBuffer&& array_buffer, IndexBuffer&& index_buffer) :
		obj(std::make_shared<Mesh>(array_buffer, index_buffer)) {}
	
	explicit Mesh_s(const Model& model) :
		obj(std::make_shared<Mesh>(model)) {}

};

class Program_s {
public:

	_SHARED_POINTER_DEFINITIONS(Program)

	Program_s() :
		obj(std::make_shared<Program>()) {}

	Program_s(const std::string& vertex_shader_code, const std::string& fragment_shader_code) :
		obj(std::make_shared<Program>(vertex_shader_code, fragment_shader_code)) {}

	Program_s(const std::string& vertex_shader_code, const std::string& geometry_shader_code, const std::string& fragment_shader_code) :
		obj(std::make_shared<Program>(vertex_shader_code, geometry_shader_code, fragment_shader_code)) {}

	explicit Program_s(const Shader& shader) :
		obj(std::make_shared<Program>(shader)) {}
};
