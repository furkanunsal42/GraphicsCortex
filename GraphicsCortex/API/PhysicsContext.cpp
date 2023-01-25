#include "PhysicsContext.h"
#include "Config.h"

#include <thread>

PhysxContext& PhysxContext::get() {
	static PhysxContext instance;
	return instance;
}

PhysxContext::PhysxContext() {
	physics_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physics_allocator, physics_error_callback);

	physics_pvd = PxCreatePvd(*physics_foundation);
	physics_transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	physics_pvd->connect(*physics_transport, physx::PxPvdInstrumentationFlag::eALL);
	PxInitExtensions(*physics, physics_pvd);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *physics_foundation, physx::PxTolerancesScale(), true, physics_pvd);

	scene_desc = new physx::PxSceneDesc(physics->getTolerancesScale());
	scene_desc->gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	if (PHYSX_THREAD_SIZE <= 0)
		scene_desc->cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
	else
		scene_desc->cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(PHYSX_THREAD_SIZE);

	scene_desc->filterShader = physx::PxDefaultSimulationFilterShader;

	physics_scene = physics->createScene(*scene_desc);
	physics_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	physics_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

	physics_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *physics_foundation, physx::PxCookingParams(physics->getTolerancesScale()));

	physx::PxSerializationRegistry* serialization_registary = physx::PxSerialization::createSerializationRegistry(*physics);
	physx::PxInitVehicleSDK(*physics, serialization_registary);

	physx::PxVehicleSetUpdateMode(physx::PxVehicleUpdateMode::eACCELERATION);

}
