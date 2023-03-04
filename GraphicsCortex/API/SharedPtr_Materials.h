#pragma once
#include "SharedPtr.h"
#include "Texture.h"

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