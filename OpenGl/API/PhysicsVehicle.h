#pragma once
#include <PxPhysicsAPI.h>

#include "SnippetVehicleSceneQuery.h"

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

	snippetvehicle::VehicleSceneQueryData* SceneQueryData;
	physx::PxBatchQuery* BatchQuery;

	snippetvehicle::PxVehicleDrivableSurfaceToTireFrictionPairs* FrictionPairs;

	physx::PxVehicleDrive4WRawInputData InputData;
	physx::PxVehicleKeySmoothingData KeySmoothingData;
	physx::PxF32 SteerVsForwardSpeedData[2 * 8];
	physx::PxFixedSizeLookupTable<8> SteerVsForwardSpeedTable;

	bool is_vehicle_in_air;

	physx::PxConvexMesh* chassis_mesh;

	enum InitValues {
		null_values,
		default_values,
	};

	enum gear {
		REVERSE = 0,
		NATURAL,
		FIRST,
		SECOND,
		THIRD,
		FORTH,
		FIFTH,
	};

	PhysicsVehicle(InitValues init_type = InitValues::default_values, int num_wheels = 4);

	void initialize();
	
	void set_gear(gear gear);
	void set_gear_autouse(bool autouse);

	void simulation_step(long double timestep);
	
	void set_chasis_mesh(physx::PxConvexMesh* convex_mesh);
	void set_chasis_mesh(physx::PxConvexMeshGeometry convex_mesh_geometry);

private:
	void _initialize_box_chassis_mesh();
	void _create_actor();
	void _create_drive();
	void _create_control();

};