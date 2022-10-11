#include "API/GraphicsCortex.h"

#include "vehicle/PxVehicleUtil.h"

#include "SnippetVehicleConcurrency.h"
#include "SnippetVehicleCreate.h"
#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleSceneQuery.h"
#include "SnippetVehicleTireFriction.h"
#include "SnippetVehicleWheelQueryResult.h"

int main() {
	physx::PxVehicleSetBasisVectors(physx::PxVec3(0, 1, 0), physx::PxVec3(0, 0, 1));
	physx::PxVehicleSetUpdateMode(physx::PxVehicleUpdateMode::eVELOCITY_CHANGE);
	
	PhysicsScene scene;

	//Create the batched scene queries for the suspension raycasts.
	snippetvehicle::VehicleSceneQueryData* gVehicleSceneQueryData = snippetvehicle::VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, snippetvehicle::WheelSceneQueryPreFilterBlocking, NULL, PhysxContext::get().physics_allocator);
	physx::PxBatchQuery* gBatchQuery = snippetvehicle::VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, PhysxContext::get().physics_scene);

	//Create the friction table for each combination of tire and surface type.
	physx::PxMaterial* material = PhysxContext::get().physics->createMaterial(0.5f, 0.5f, 0.5f);
	snippetvehicle::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = snippetvehicle::createFrictionPairs(material);

	//Create a plane to drive on.
	physx::PxFilterData groundPlaneSimFilterData(snippetvehicle::COLLISION_FLAG_GROUND, snippetvehicle::COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	physx::PxRigidStatic* gGroundPlane = snippetvehicle::createDrivablePlane(groundPlaneSimFilterData, material, PhysxContext::get().physics);
	PhysxContext::get().physics_scene->addActor(*gGroundPlane);


	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(box);
	box.set_position(4.0f, 10.0f, 4.0f);

	while (true) {
		scene.simulation_step_start(1 / 60.0f);
		scene.simulation_step_finish();
	}

}