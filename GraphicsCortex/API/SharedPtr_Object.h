#pragma once
#include "SharedPtr.h"

#include "Object.h"

class Object_s {
public:

	_SHARED_POINTER_DEFINITIONS(Object)

	Object_s(Graphic& graphics, PhysicsObject& physics) :
		obj(std::make_shared<Object>(graphics, physics)) {}

};

class Vehicle_s {
public:

	_SHARED_POINTER_DEFINITIONS(Vehicle)

	Vehicle_s() :
		obj(std::make_shared<Vehicle>()) {} // different

};