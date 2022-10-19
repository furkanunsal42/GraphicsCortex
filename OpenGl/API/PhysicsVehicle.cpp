#include "PhysicsVehicle.h"
#include "PhysicsContext.h"

#include "SnippetVehicleFilterShader.h"

PhysicsVehicle::PhysicsVehicle(InitValues init_type, int num_wheels) {
	
	physx::PxVehicleSetBasisVectors(physx::PxVec3(0, 1, 0), physx::PxVec3(0, 0, 1));


	numWheels = numWheels;

	chassisSimFilterData = physx::PxFilterData(snippetvehicle::COLLISION_FLAG_CHASSIS, snippetvehicle::COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);
	wheelSimFilterData = physx::PxFilterData(snippetvehicle::COLLISION_FLAG_WHEEL, snippetvehicle::COLLISION_FLAG_WHEEL_AGAINST, 0, 0);

	if (init_type == PhysicsVehicle::default_values) {
		chassisMass = 1500.0f;
		chassisDims = physx::PxVec3(2.5f, 2.0f, 5.0f);
		chassisMOI = physx::PxVec3(		// rectangle moment of inertia calculation
			(chassisDims.y * chassisDims.y + chassisDims.z * chassisDims.z) * chassisMass / 12.0f,
			(chassisDims.x * chassisDims.x + chassisDims.z * chassisDims.z) * 0.8f * chassisMass / 12.0f,
			(chassisDims.x * chassisDims.x + chassisDims.y * chassisDims.y) * chassisMass / 12.0f);

		chassisCMOffset = physx::PxVec3(0.0f, -chassisDims.y * 0.5f + 0.65f, 0.25f);
		physx::PxMaterial* chasis_material = PhysxContext::get().physics->createMaterial(0.5f, 0.5f, 0.5f);
		chassisMaterial = chasis_material;
		wheelMass = 20.0f;
		wheelWidth = 0.2f;
		wheelRadius = 0.5f;
		wheelMOI = 0.5f * wheelMass * wheelRadius * wheelRadius;
		physx::PxMaterial* wheel_material = PhysxContext::get().physics->createMaterial(0.3f, 0.2f, 0.7f);
		wheelMaterial = wheel_material;
	}
	else if (init_type == PhysicsVehicle::null_values) {
		chassisMass = 0.0f;
		chassisDims = physx::PxVec3(0.0f, 0.0f, 0.0f);
		chassisMOI = physx::PxVec3(0.0f, 0.0f, 0.0f);
		chassisCMOffset = physx::PxVec3(0.0f, 0.0f, 0.0f);
		chassisMaterial = NULL;
		wheelMass = 0.0f;
		wheelWidth = 0.0f;
		wheelRadius = 0.0f;
		wheelMOI = 0.0f;
		wheelMaterial = NULL;
	}
}