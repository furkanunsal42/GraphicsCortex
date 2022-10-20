#include "PhysicsVehicle.h"
#include "PhysicsContext.h"

#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleCreate.h"
#include "SnippetVehicleTireFriction.h"


namespace fourwheel
{

	void computeWheelCenterActorOffsets4W(const physx::PxF32 wheelFrontZ, const physx::PxF32 wheelRearZ, const physx::PxVec3& chassisDims, const physx::PxF32 wheelWidth, const physx::PxF32 wheelRadius, const physx::PxU32 numWheels, physx::PxVec3* wheelCentreOffsets)
	{
		//chassisDims.z is the distance from the rear of the chassis to the front of the chassis.
		//The front has z = 0.5*chassisDims.z and the rear has z = -0.5*chassisDims.z.
		//Compute a position for the front wheel and the rear wheel along the z-axis.
		//Compute the separation between each wheel along the z-axis.
		const physx::PxF32 numLeftWheels = numWheels / 2.0f;
		const physx::PxF32 deltaZ = (wheelFrontZ - wheelRearZ) / (numLeftWheels - 1.0f);
		//Set the outside of the left and right wheels to be flush with the chassis.
		//Set the top of the wheel to be just touching the underside of the chassis.
		//Begin by setting the rear-left/rear-right/front-left,front-right wheels.
		wheelCentreOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + 0 * deltaZ * 0.5f);
		wheelCentreOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + 0 * deltaZ * 0.5f);
		wheelCentreOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + (numLeftWheels - 1) * deltaZ);
		wheelCentreOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + (numLeftWheels - 1) * deltaZ);
		//Set the remaining wheels.
		for (physx::PxU32 i = 2, wheelCount = 4; i < numWheels - 2; i += 2, wheelCount += 2)
		{
			wheelCentreOffsets[wheelCount + 0] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i * deltaZ * 0.5f);
			wheelCentreOffsets[wheelCount + 1] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), wheelRearZ + i * deltaZ * 0.5f);
		}
	}

	void setupWheelsSimulationData
	(const physx::PxF32 wheelMass, const physx::PxF32 wheelMOI, const physx::PxF32 wheelRadius, const physx::PxF32 wheelWidth,
		const physx::PxU32 numWheels, const physx::PxVec3* wheelCenterActorOffsets,
		const physx::PxVec3& chassisCMOffset, const physx::PxF32 chassisMass,
		physx::PxVehicleWheelsSimData* wheelsSimData)
	{
		//Set up the wheels.
		physx::PxVehicleWheelData wheels[PX_MAX_NB_WHEELS];
		{
			//Set up the wheel data structures with mass, moi, radius, width.
			for (physx::PxU32 i = 0; i < numWheels; i++)
			{
				wheels[i].mMass = wheelMass;
				wheels[i].mMOI = wheelMOI;
				wheels[i].mRadius = wheelRadius;
				wheels[i].mWidth = wheelWidth;
			}

			//Enable the handbrake for the rear wheels only.
			wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxHandBrakeTorque = 4000.0f;
			wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxHandBrakeTorque = 4000.0f;
			//Enable steering for the front wheels only.
			wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxSteer = physx::PxPi * 0.3333f;
			wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxSteer = physx::PxPi * 0.3333f;
		}

		//Set up the tires.
		physx::PxVehicleTireData tires[PX_MAX_NB_WHEELS];
		{
			//Set up the tires.
			for (physx::PxU32 i = 0; i < numWheels; i++)
			{
				tires[i].mType = snippetvehicle::TIRE_TYPE_NORMAL;
			}
		}

		//Set up the suspensions
		physx::PxVehicleSuspensionData suspensions[PX_MAX_NB_WHEELS];
		{
			//Compute the mass supported by each suspension spring.
			physx::PxF32 suspSprungMasses[PX_MAX_NB_WHEELS];
			PxVehicleComputeSprungMasses
			(numWheels, wheelCenterActorOffsets,
				chassisCMOffset, chassisMass, 1, suspSprungMasses);

			//Set the suspension data.
			for (physx::PxU32 i = 0; i < numWheels; i++)
			{
				suspensions[i].mMaxCompression = 0.3f;
				suspensions[i].mMaxDroop = 0.1f;
				suspensions[i].mSpringStrength = 35000.0f;
				suspensions[i].mSpringDamperRate = 4500.0f;
				suspensions[i].mSprungMass = suspSprungMasses[i];
			}

			//Set the camber angles.
			const physx::PxF32 camberAngleAtRest = 0.0;
			const physx::PxF32 camberAngleAtMaxDroop = 0.01f;
			const physx::PxF32 camberAngleAtMaxCompression = -0.01f;
			for (physx::PxU32 i = 0; i < numWheels; i += 2)
			{
				suspensions[i + 0].mCamberAtRest = camberAngleAtRest;
				suspensions[i + 1].mCamberAtRest = -camberAngleAtRest;
				suspensions[i + 0].mCamberAtMaxDroop = camberAngleAtMaxDroop;
				suspensions[i + 1].mCamberAtMaxDroop = -camberAngleAtMaxDroop;
				suspensions[i + 0].mCamberAtMaxCompression = camberAngleAtMaxCompression;
				suspensions[i + 1].mCamberAtMaxCompression = -camberAngleAtMaxCompression;
			}
		}

		//Set up the wheel geometry.
		physx::PxVec3 suspTravelDirections[PX_MAX_NB_WHEELS];
		physx::PxVec3 wheelCentreCMOffsets[PX_MAX_NB_WHEELS];
		physx::PxVec3 suspForceAppCMOffsets[PX_MAX_NB_WHEELS];
		physx::PxVec3 tireForceAppCMOffsets[PX_MAX_NB_WHEELS];
		{
			//Set the geometry data.
			for (physx::PxU32 i = 0; i < numWheels; i++)
			{
				//Vertical suspension travel.
				suspTravelDirections[i] = physx::PxVec3(0, -1, 0);

				//Wheel center offset is offset from rigid body center of mass.
				wheelCentreCMOffsets[i] =
					wheelCenterActorOffsets[i] - chassisCMOffset;

				//Suspension force application point 0.3 metres below 
				//rigid body center of mass.
				suspForceAppCMOffsets[i] =
					physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);

				//Tire force application point 0.3 metres below 
				//rigid body center of mass.
				tireForceAppCMOffsets[i] =
					physx::PxVec3(wheelCentreCMOffsets[i].x, -0.3f, wheelCentreCMOffsets[i].z);
			}
		}

		//Set up the filter data of the raycast that will be issued by each suspension.
		physx::PxFilterData qryFilterData;
		snippetvehicle::setupNonDrivableSurface(qryFilterData);

		//Set the wheel, tire and suspension data.
		//Set the geometry data.
		//Set the query filter data
		for (physx::PxU32 i = 0; i < numWheels; i++)
		{
			wheelsSimData->setWheelData(i, wheels[i]);
			wheelsSimData->setTireData(i, tires[i]);
			wheelsSimData->setSuspensionData(i, suspensions[i]);
			wheelsSimData->setSuspTravelDirection(i, suspTravelDirections[i]);
			wheelsSimData->setWheelCentreOffset(i, wheelCentreCMOffsets[i]);
			wheelsSimData->setSuspForceAppPointOffset(i, suspForceAppCMOffsets[i]);
			wheelsSimData->setTireForceAppPointOffset(i, tireForceAppCMOffsets[i]);
			wheelsSimData->setSceneQueryFilterData(i, qryFilterData);
			wheelsSimData->setWheelShapeMapping(i, physx::PxI32(i));
		}

		//Add a front and rear anti-roll bar
		physx::PxVehicleAntiRollBarData barFront;
		barFront.mWheel0 = physx::PxVehicleDrive4WWheelOrder::eFRONT_LEFT;
		barFront.mWheel1 = physx::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT;
		barFront.mStiffness = 10000.0f;
		wheelsSimData->addAntiRollBarData(barFront);
		physx::PxVehicleAntiRollBarData barRear;
		barRear.mWheel0 = physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT;
		barRear.mWheel1 = physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT;
		barRear.mStiffness = 10000.0f;
		wheelsSimData->addAntiRollBarData(barRear);
	}

} //namespace fourwheel

PhysicsVehicle::PhysicsVehicle(InitValues init_type, int num_wheels) :
	numWheels(num_wheels), vehicle_actor(nullptr), is_vehicle_in_air(true)
{
	physx::PxVehicleSetBasisVectors(physx::PxVec3(0, 1, 0), physx::PxVec3(0, 0, 1));

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

	SceneQueryData = snippetvehicle::VehicleSceneQueryData::allocate(1, num_wheels, 1, 1, snippetvehicle::WheelSceneQueryPreFilterBlocking, NULL, PhysxContext::get().physics_allocator);
	BatchQuery = snippetvehicle::VehicleSceneQueryData::setUpBatchedSceneQuery(0, *SceneQueryData, PhysxContext::get().physics_scene);
}

void PhysicsVehicle::_create_actor() {

	//Construct a convex mesh for a cylindrical wheel.
	physx::PxConvexMesh* wheelMesh = snippetvehicle::createWheelMesh(wheelWidth, wheelRadius, *PhysxContext::get().physics, *PhysxContext::get().physics_cooking);
	//Assume all wheels are identical for simplicity.
	physx::PxConvexMesh** wheelConvexMeshes = new physx::PxConvexMesh*[numWheels];
	physx::PxMaterial** wheelMaterials = new physx::PxMaterial*[numWheels];

	//Set the meshes and materials for the driven wheels.
	for (physx::PxU32 i = 0; i < numWheels; i++)
	{
		wheelConvexMeshes[i] = wheelMesh;
		wheelMaterials[i] = wheelMaterial;
	}

	//Chassis just has a single convex shape for simplicity.
	physx::PxConvexMesh* chassisConvexMesh = snippetvehicle::createChassisMesh(chassisDims, *PhysxContext::get().physics, *PhysxContext::get().physics_cooking);
	physx::PxConvexMesh* chassisConvexMeshes[1] = { chassisConvexMesh };
	physx::PxMaterial* chassisMaterials[1] = { chassisMaterial };

	//Rigid body data.
	physx::PxVehicleChassisData rigidBodyData;
	rigidBodyData.mMOI = chassisMOI;
	rigidBodyData.mMass = chassisMass;
	rigidBodyData.mCMOffset = chassisCMOffset;

	vehicle_actor = snippetvehicle::createVehicleActor
	(rigidBodyData,
		wheelMaterials, wheelConvexMeshes, numWheels, wheelSimFilterData,
		chassisMaterials, chassisConvexMeshes, 1, chassisSimFilterData,
		*PhysxContext::get().physics);

	physx::PxTransform startTransform(physx::PxVec3(0, (chassisDims.y * 0.5f + wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	vehicle_actor->setGlobalPose(startTransform);

	// delete[] wheelConvexMeshes ?
	// delete[] wheelMaterials ?
}

void PhysicsVehicle::_create_drive() {
	//Set up the sim data for the wheels.
	physx::PxVehicleWheelsSimData* wheelsSimData = physx::PxVehicleWheelsSimData::allocate(numWheels);
	{
		//Compute the wheel center offsets from the origin.
		physx::PxVec3 wheelCenterActorOffsets[PX_MAX_NB_WHEELS];
		const physx::PxF32 frontZ = chassisDims.z * 0.3f;
		const physx::PxF32 rearZ = -chassisDims.z * 0.3f;
		fourwheel::computeWheelCenterActorOffsets4W(frontZ, rearZ, chassisDims, wheelWidth, wheelRadius, numWheels, wheelCenterActorOffsets);

		//Set up the simulation data for all wheels.
		fourwheel::setupWheelsSimulationData
		(wheelMass, wheelMOI, wheelRadius, wheelWidth,
			numWheels, wheelCenterActorOffsets,
			chassisCMOffset, chassisMass,
			wheelsSimData);
	}

	//Set up the sim data for the vehicle drive model.
	physx::PxVehicleDriveSimData4W driveSimData;
	{
		//Diff
		physx::PxVehicleDifferential4WData diff;
		diff.mType = physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD;
		driveSimData.setDiffData(diff);

		//Engine
		physx::PxVehicleEngineData engine;
		engine.mPeakTorque = 500.0f;
		engine.mMaxOmega = 600.0f;//approx 6000 rpm
		driveSimData.setEngineData(engine);

		//Gears
		physx::PxVehicleGearsData gears;
		gears.mSwitchTime = 0.5f;
		driveSimData.setGearsData(gears);

		//Clutch
		physx::PxVehicleClutchData clutch;
		clutch.mStrength = 10.0f;
		driveSimData.setClutchData(clutch);

		//Ackermann steer accuracy
		physx::PxVehicleAckermannGeometryData ackermann;
		ackermann.mAccuracy = 1.0f;
		ackermann.mAxleSeparation =
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).z -
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT).z;
		ackermann.mFrontWidth =
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT).x -
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT).x;
		ackermann.mRearWidth =
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT).x -
			wheelsSimData->getWheelCentreOffset(snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT).x;
		driveSimData.setAckermannGeometryData(ackermann);
	}

	//Create a vehicle from the wheels and drive sim data.
	vehicle_drive = snippetvehicle::PxVehicleDrive4W::allocate(numWheels);
	vehicle_drive->setup(PhysxContext::get().physics, vehicle_actor, *wheelsSimData, driveSimData, numWheels - 4);

	vehicle_drive->setToRestState();
	set_gear(PhysicsVehicle::FIRST);
	set_gear_autouse(true);

	FrictionPairs = snippetvehicle::createFrictionPairs(wheelMaterial);

	//Free the sim data because we don't need that any more.
	wheelsSimData->free();
}

void PhysicsVehicle::_create_control() {
	KeySmoothingData =
	{
		{
			6.0f,	//rise rate eANALOG_INPUT_ACCEL
			6.0f,	//rise rate eANALOG_INPUT_BRAKE		
			6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
			2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
			2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
		},
		{
			10.0f,	//fall rate eANALOG_INPUT_ACCEL
			10.0f,	//fall rate eANALOG_INPUT_BRAKE		
			10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
			5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
			5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
		}
	};

	physx::PxF32 SteerVsForwardSpeedData[2 * 8] =
	{
		0.0f,		0.75f,
		5.0f,		0.75f,
		30.0f,		0.325f,
		120.0f,		0.125f,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32
	};

	SteerVsForwardSpeedTable = physx::PxFixedSizeLookupTable<8>(SteerVsForwardSpeedData, 4);
}

void PhysicsVehicle::simulation_step(long double timestep) {
	//Raycasts.
	physx::PxVehicleWheels* vehicles[1] = { vehicle_drive };
	physx::PxRaycastQueryResult* raycastResults = SceneQueryData->getRaycastQueryResultBuffer(0);
	const physx::PxU32 raycastResultsSize = SceneQueryData->getQueryResultBufferSize();
	PxVehicleSuspensionRaycasts(BatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

	PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(KeySmoothingData, SteerVsForwardSpeedTable, InputData, timestep, is_vehicle_in_air, *vehicle_drive);

	//Vehicle update.
	const physx::PxVec3 grav = PhysxContext::get().physics_scene->getGravity();
	physx::PxWheelQueryResult* wheelQueryResults = new physx::PxWheelQueryResult[numWheels];
	physx::PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, vehicle_drive->mWheelsSimData.getNbWheels()} };
	PxVehicleUpdates(timestep, grav, *FrictionPairs, 1, vehicles, vehicleQueryResults);

	//Work out if the vehicle is in the air.
	is_vehicle_in_air = vehicle_drive->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);

	delete[] wheelQueryResults;
}

void PhysicsVehicle::initialize() {
	_create_actor();
	_create_drive();
	_create_control();
}

void PhysicsVehicle::set_gear(gear input_gear) {
	vehicle_drive->mDriveDynData.forceGearChange((snippetvehicle::PxVehicleGearsData::Enum)input_gear);
}

void PhysicsVehicle::set_gear_autouse(bool autouse) {
	vehicle_drive->mDriveDynData.setUseAutoGears(autouse);
}

