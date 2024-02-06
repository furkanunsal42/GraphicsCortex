#pragma once
#include <PxPhysicsAPI.h>
#include "Config.h"

class PhysxContext {
public:
	physx::PxDefaultAllocator physics_allocator;
	physx::PxDefaultErrorCallback physics_error_callback;
	physx::PxPhysics* physics;
	physx::PxFoundation* physics_foundation;
	physx::PxSceneDesc* scene_desc;
	physx::PxScene* physics_scene;
	physx::PxPvd* physics_pvd;
	physx::PxPvdTransport* physics_transport;
	physx::PxCooking* physics_cooking;

	static PhysxContext& get();

private:
	PhysxContext();
};

