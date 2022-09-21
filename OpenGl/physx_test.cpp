#include "PxPhysicsAPI.h"

int main()
{
    auto Allocator = physx::PxDefaultAllocator();
    auto ErrorCallback = physx::PxDefaultErrorCallback();

    auto Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, Allocator, ErrorCallback);

    auto PhysXVisualDebugger = PxCreatePvd(*Foundation);

    const auto transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    PhysXVisualDebugger->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

    auto Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *Foundation, physx::PxTolerancesScale(), true, PhysXVisualDebugger);

    physx::PxSceneDesc sceneDesc(Physics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    auto Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = Dispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    auto Scene = Physics->createScene(sceneDesc);

    auto pvdClient = Scene->getScenePvdClient();;
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
}