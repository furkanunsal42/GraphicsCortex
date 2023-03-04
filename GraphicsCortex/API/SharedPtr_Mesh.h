#pragma once
#include "SharedPtr.h"
#include "SharedPtr_Buffers.h"

#include "Mesh.h"

class Mesh_s {
public:

	_SHARED_POINTER_DEFINITIONS(Mesh)

	Mesh_s() :
		obj(std::make_shared<Mesh>()) {}

	Mesh_s(ArrayBuffer_s array_buffer, IndexBuffer_s index_buffer) :
		obj(std::make_shared<Mesh>(array_buffer, index_buffer)) {}

	explicit Mesh_s(const Model& model) :
		obj(std::make_shared<Mesh>(model)) {}

};