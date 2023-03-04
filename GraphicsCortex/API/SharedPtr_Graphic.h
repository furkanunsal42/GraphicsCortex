#pragma once
#include "SharedPtr.h"
#include "SharedPtr_Materials.h"
#include "SharedPtr_Mesh.h"
#include "SharedPtr_Program.h"

#include "Graphic.h"

class Graphic_s {
public:

	_SHARED_POINTER_DEFINITIONS(Graphic)

	Graphic_s() :
		obj(std::make_shared<Graphic>()) {}

	Graphic_s(Mesh_s mesh, Material_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(mesh, material, renderer)) {}

	Graphic_s(Mesh_s mesh, UnorderedMaterial_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(mesh, material, renderer)) {}

	Graphic_s(const std::vector<float>& verticies, int data_dim) :
		obj(std::make_shared<Graphic>(verticies, data_dim)) {}

	Graphic_s(Material_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(material, renderer)) {}

	Graphic_s(UnorderedMaterial_s material, Program_s renderer) :
		obj(std::make_shared<Graphic>(material, renderer)) {}

};
