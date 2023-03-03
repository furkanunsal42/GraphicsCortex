#pragma once
#include "SharedPtr.h"
#include "SharedPtr_RecurringClasses_Part1.h"

#include "Object.h"
#include "PhysicsObject.h"

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
		obj(std::make_shared<Vehicle>()) {}

};

class PhysicsObject_s {
public:

	_SHARED_POINTER_DEFINITIONS(PhysicsObject)

	PhysicsObject_s(const physx::PxGeometry& geometry, enum PhysicsObject::type type = PhysicsObject::type::DYNAMIC, bool exclusive_shape = false) :
		obj(std::make_shared<PhysicsObject>(geometry, type, exclusive_shape )) {}

	PhysicsObject_s(const physx::PxPlane& plane, bool exclusive_shape = false) :
		obj(std::make_shared<PhysicsObject>(plane, exclusive_shape )) {}

};

class PhysicsVehicle_s {
public:

	_SHARED_POINTER_DEFINITIONS(PhysicsVehicle)

	PhysicsVehicle_s(PhysicsVehicle::InitValues init_type = PhysicsVehicle::InitValues::default_values, int num_wheels = 4) :
		obj(std::make_shared<PhysicsVehicle>(init_type, num_wheels)) {}

};

