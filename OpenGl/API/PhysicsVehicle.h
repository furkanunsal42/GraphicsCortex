#pragma once
#include <PxPhysicsAPI.h>


class PhysicsVehicle {
public:
	physx::PxF32 chassisMass;
	physx::PxVec3 chassisDims;
	physx::PxVec3 chassisMOI;
	physx::PxVec3 chassisCMOffset;
	physx::PxMaterial* chassisMaterial;
	physx::PxFilterData chassisSimFilterData;  //word0 = collide type, word1 = collide against types, word2 = PxPairFlags

	physx::PxF32 wheelMass;
	physx::PxF32 wheelWidth;
	physx::PxF32 wheelRadius;
	physx::PxF32 wheelMOI;
	physx::PxMaterial* wheelMaterial;
	physx::PxU32 numWheels;
	physx::PxFilterData wheelSimFilterData;	//word0 = collide type, word1 = collide against types, word2 = PxPairFlags
	
	physx::PxRigidDynamic* vehicle_actor;
	physx::PxVehicleDrive4W* vehicle_drive;

	enum InitValues {
		null_values,
		default_values,
	};

	PhysicsVehicle(InitValues init_type = default_values, int num_wheels = 4);

	void create_actor();
	void create_drive();

};