#pragma once
#include "SharedPtr.h"

#include "PhysicsObject.h"
#include "PhysicsVehicle.h"

class PhysicsObject_s {
public:

	_SHARED_POINTER_DEFINITIONS(PhysicsObject)

		PhysicsObject_s(const physx::PxGeometry& geometry, enum PhysicsObject::type type = PhysicsObject::type::DYNAMIC, bool exclusive_shape = false) :
		obj(std::make_shared<PhysicsObject>(geometry, type, exclusive_shape)) {}

	PhysicsObject_s(const physx::PxPlane& plane, bool exclusive_shape = false) :
		obj(std::make_shared<PhysicsObject>(plane, exclusive_shape)) {}

};

class PhysicsVehicle_s {
public:

	_SHARED_POINTER_DEFINITIONS(PhysicsVehicle)

		PhysicsVehicle_s(PhysicsVehicle::InitValues init_type = PhysicsVehicle::InitValues::default_values, int num_wheels = 4) :
		obj(std::make_shared<PhysicsVehicle>(init_type, num_wheels)) {}

};

