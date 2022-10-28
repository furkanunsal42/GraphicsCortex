#include "PhysicsVehicle.h"
#include "PhysicsContext.h"

#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleCreate.h"
#include "SnippetVehicleTireFriction.h"

#include "iostream"

PhysicsVehicle::PhysicsVehicle(InitValues init_type, int num_wheels) :
	numWheels(num_wheels), vehicle_actor(nullptr), is_vehicle_in_air(true), chassis_mesh(nullptr), wheel_mesh(nullptr), differential_type(physx::PxVehicleDifferential4WData::eDIFF_TYPE_LS_4WD)
{
	PhysxContext::get();

	wheelsSimData = physx::PxVehicleWheelsSimData::allocate(num_wheels);
	vehicle_drive = snippetvehicle::PxVehicleDrive4W::allocate(num_wheels);

	wheelOffsets = new physx::PxVec3[num_wheels];
	for (int i = 0; i < num_wheels; i++) {
		wheelOffsets[i] = physx::PxVec3(0, 0, 0);
	}

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

		max_steer = physx::PxPi * 0.3333f;
		max_handbreak_torque = 4000.0f;
		ackermann_accuracy = 1.0f;
		clutch_strength = 10.0f;
		gear_switch_time = 0.5f;
		engine_peak_revolution_speed = 600.0f;
		engine_peak_torque = 500.0f;

		max_suspension_compression = 0.3f;
		max_suspension_droop = 0.1f;
		suspension_string_strength = 35000.0f;
		suspension_spring_damper_rate = 4500.0f;
		suspension_travel_direction = physx::PxVec3(0.0f, -1.0f, 0.0f);
		suspension_force_application_offset = -0.3f;	//Suspension force application point 0.3 metres below 
		tire_force_application_offset = -0.3f;			//tire force application point 0.3 metres below 
		
		camber_angle_at_rest = 0.0f;
		camber_angle_at_max_droop = 0.01f;
		camber_angle_at_max_compression = -0.1f;
		antiroll_bar_stiffness_front = 10000.0f;
		antiroll_bar_stiffness_rear = 10000.0f;
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
		max_steer = 0.0f;
		max_handbreak_torque = 0.0f;
		ackermann_accuracy = 0.0f;
		clutch_strength = 0.0f;
		gear_switch_time = 0.0f;
		engine_peak_revolution_speed = 0.0f;
		engine_peak_torque = 0.0f;
		max_suspension_compression = 0.0f;
		max_suspension_droop = 0.0f;
		suspension_string_strength = 0.0f;
		suspension_spring_damper_rate = 0.0f;
		suspension_travel_direction = physx::PxVec3(0);
		suspension_force_application_offset = 0.0f;	
		tire_force_application_offset = 0.0f;		
		camber_angle_at_rest = 0.0f;
		camber_angle_at_max_droop = 0.0f;
		camber_angle_at_max_compression = 0.0f;
		antiroll_bar_stiffness_front = 0.0f;
		antiroll_bar_stiffness_rear = 0.0f;
	}

	SceneQueryData = snippetvehicle::VehicleSceneQueryData::allocate(1, num_wheels, 1, 1, snippetvehicle::WheelSceneQueryPreFilterBlocking, NULL, PhysxContext::get().physics_allocator);
	BatchQuery = snippetvehicle::VehicleSceneQueryData::setUpBatchedSceneQuery(0, *SceneQueryData, PhysxContext::get().physics_scene);
}

void PhysicsVehicle::_initialize_box_chassis_mesh() {
	if (chassis_mesh != nullptr)
		chassis_mesh->release();
		
	chassis_mesh = snippetvehicle::createChassisMesh(chassisDims, *PhysxContext::get().physics, *PhysxContext::get().physics_cooking);
}

void PhysicsVehicle::_initialize_cylinder_wheel_mesh() {
	if (wheel_mesh != nullptr)
		wheel_mesh->release();

	wheel_mesh = snippetvehicle::createWheelMesh(wheelWidth, wheelRadius, *PhysxContext::get().physics, *PhysxContext::get().physics_cooking);
}

void PhysicsVehicle::_create_actor() {

	//Construct a convex mesh for a cylindrical wheel.
	if (wheel_mesh == nullptr) {
		_initialize_cylinder_wheel_mesh();
	}
	//Assume all wheels are identical for simplicity.
	physx::PxConvexMesh** wheelConvexMeshes = new physx::PxConvexMesh*[numWheels];
	physx::PxMaterial** wheelMaterials = new physx::PxMaterial*[numWheels];

	//Set the meshes and materials for the driven wheels.
	for (physx::PxU32 i = 0; i < numWheels; i++)
	{
		wheelConvexMeshes[i] = wheel_mesh;
		wheelMaterials[i] = wheelMaterial;
	}

	if (chassis_mesh == nullptr) {
		_initialize_box_chassis_mesh();
	}
	physx::PxConvexMesh* chassisConvexMeshes[1] = { chassis_mesh };
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

	delete[] wheelConvexMeshes;
	delete[] wheelMaterials;
}

void PhysicsVehicle::_calculate_default_wheel_offsets() {
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), -chassisDims.z / 2 + wheelRadius);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), -chassisDims.z / 2 + wheelRadius);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT] = physx::PxVec3((-chassisDims.x + wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), chassisDims.z / 2 - wheelRadius);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT] = physx::PxVec3((+chassisDims.x - wheelWidth) * 0.5f, -(chassisDims.y / 2 + wheelRadius), chassisDims.z / 2 - wheelRadius);
}

void PhysicsVehicle::_create_wheel_sim() {

	//Set up the wheels.
	physx::PxVehicleWheelData* wheels = new physx::PxVehicleWheelData[numWheels];
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
		wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT].mMaxHandBrakeTorque = max_handbreak_torque;
		wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT].mMaxHandBrakeTorque = max_handbreak_torque;
		//Enable steering for the front wheels only.
		wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT].mMaxSteer = max_steer;
		wheels[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT].mMaxSteer = max_steer;
	}

	//Set up the tires.
	physx::PxVehicleTireData* tires = new physx::PxVehicleTireData[numWheels];
	{
		//Set up the tires.
		for (physx::PxU32 i = 0; i < numWheels; i++)
		{
			tires[i].mType = wheel_type;
		}
	}

	//Set up the suspensions
	physx::PxVehicleSuspensionData* suspensions = new physx::PxVehicleSuspensionData[numWheels];
	{
		//Compute the mass supported by each suspension spring.
		physx::PxF32* suspSprungMasses = new physx::PxF32[numWheels];
		PxVehicleComputeSprungMasses
		(numWheels, wheelOffsets,
			chassisCMOffset, chassisMass, 1, suspSprungMasses);

		//Set the suspension data.
		for (physx::PxU32 i = 0; i < numWheels; i++)
		{
			suspensions[i].mMaxCompression = max_suspension_compression;
			suspensions[i].mMaxDroop = max_suspension_droop;
			suspensions[i].mSpringStrength = suspension_string_strength;
			suspensions[i].mSpringDamperRate = suspension_spring_damper_rate;
			suspensions[i].mSprungMass = suspSprungMasses[i];
		}

		//Set the camber angles.
		for (physx::PxU32 i = 0; i < numWheels; i += 2)
		{
			suspensions[i + 0].mCamberAtRest = camber_angle_at_rest;
			suspensions[i + 1].mCamberAtRest = -camber_angle_at_rest;
			suspensions[i + 0].mCamberAtMaxDroop = camber_angle_at_max_droop;
			suspensions[i + 1].mCamberAtMaxDroop = -camber_angle_at_max_droop;
			suspensions[i + 0].mCamberAtMaxCompression = camber_angle_at_max_compression;
			suspensions[i + 1].mCamberAtMaxCompression = -camber_angle_at_max_compression;
		}
		delete[] suspSprungMasses;
	}

	//Set up the wheel geometry.
	physx::PxVec3* suspTravelDirections = new physx::PxVec3[numWheels];
	physx::PxVec3* wheelCentreCMOffsets = new physx::PxVec3[numWheels];
	physx::PxVec3* suspForceAppCMOffsets = new physx::PxVec3[numWheels];
	physx::PxVec3* tireForceAppCMOffsets = new physx::PxVec3[numWheels];
	{
		//Set the geometry data.
		for (physx::PxU32 i = 0; i < numWheels; i++)
		{
			//Vertical suspension travel.
			suspTravelDirections[i] = suspension_travel_direction;

			//Wheel center offset is offset from rigid body center of mass.
			wheelCentreCMOffsets[i] =
				wheelOffsets[i] - chassisCMOffset;

			//Suspension force application point 0.3 metres below 
			//rigid body center of mass.
			suspForceAppCMOffsets[i] =
				physx::PxVec3(wheelCentreCMOffsets[i].x, -suspension_force_application_offset, wheelCentreCMOffsets[i].z);

			//Tire force application point 0.3 metres below 
			//rigid body center of mass.
			tireForceAppCMOffsets[i] =
				physx::PxVec3(wheelCentreCMOffsets[i].x, -tire_force_application_offset, wheelCentreCMOffsets[i].z);
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
	barFront.mStiffness = antiroll_bar_stiffness_front;
	wheelsSimData->addAntiRollBarData(barFront);
	physx::PxVehicleAntiRollBarData barRear;
	barRear.mWheel0 = physx::PxVehicleDrive4WWheelOrder::eREAR_LEFT;
	barRear.mWheel1 = physx::PxVehicleDrive4WWheelOrder::eREAR_RIGHT;
	barRear.mStiffness = antiroll_bar_stiffness_rear;
	wheelsSimData->addAntiRollBarData(barRear);

	delete[] wheels;
	delete[] tires;
	delete[] suspensions;
	delete[] suspTravelDirections;
	delete[] wheelCentreCMOffsets;
	delete[] suspForceAppCMOffsets;
	delete[] tireForceAppCMOffsets;
}

void PhysicsVehicle::_create_drive_sim() {
	//Set up the sim data for the vehicle drive model.

	physx::PxVehicleDifferential4WData diff;
	diff.mType = differential_type;
	driveSimData.setDiffData(diff);

	physx::PxVehicleEngineData engine;
	engine.mPeakTorque = engine_peak_torque;
	engine.mMaxOmega = engine_peak_revolution_speed;
	driveSimData.setEngineData(engine);

	physx::PxVehicleGearsData gears;
	gears.mSwitchTime = gear_switch_time;
	driveSimData.setGearsData(gears);

	physx::PxVehicleClutchData clutch;
	clutch.mStrength = clutch_strength;
	driveSimData.setClutchData(clutch);

	//Ackermann steer accuracy
	physx::PxVehicleAckermannGeometryData ackermann;
	ackermann.mAccuracy = ackermann_accuracy;
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

void PhysicsVehicle::_create_drive() {
	
	vehicle_drive->setup(PhysxContext::get().physics, vehicle_actor, *wheelsSimData, driveSimData, numWheels - 4);

	vehicle_drive->setToRestState();
	set_gear(PhysicsVehicle::FIRST);
	set_gear_autouse(true);

	FrictionPairs = snippetvehicle::createFrictionPairs(wheelMaterial);

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

void PhysicsVehicle::compile() {
	_create_actor();

	physx::PxTransform startTransform(physx::PxVec3(0, (chassisDims.y * 0.5f + wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	vehicle_actor->setGlobalPose(startTransform);

	bool initialize_offsets = true;
	for (int i = 0; i < numWheels; i++) {
		if (wheelOffsets[i] != physx::PxVec3(0, 0, 0)) {
			initialize_offsets = false;
			break;
		}
	}
	if (initialize_offsets) {
		_calculate_default_wheel_offsets();
	}

	_create_wheel_sim();
	_create_drive_sim();

	_create_drive();
	_create_control();
}

void PhysicsVehicle::set_gear(gear input_gear) {
	vehicle_drive->mDriveDynData.forceGearChange((snippetvehicle::PxVehicleGearsData::Enum)input_gear);
}

void PhysicsVehicle::set_gear_autouse(bool autouse) {
	vehicle_drive->mDriveDynData.setUseAutoGears(autouse);
}

void PhysicsVehicle::set_chasis_mesh(physx::PxConvexMesh* convex_mesh) {
	if (chassis_mesh != nullptr)
		chassis_mesh->release();
	chassis_mesh = convex_mesh;
}

void PhysicsVehicle::set_chasis_mesh(physx::PxConvexMeshGeometry convex_mesh_geometry) {
	if (chassis_mesh != nullptr)
		chassis_mesh->release();
	chassis_mesh = convex_mesh_geometry.convexMesh;
}

void PhysicsVehicle::set_wheel_mesh(physx::PxConvexMesh* convex_mesh) {
	if (wheel_mesh != nullptr)
		wheel_mesh->release();
	wheel_mesh = convex_mesh;
}
void PhysicsVehicle::set_wheel_mesh(physx::PxConvexMeshGeometry convex_mesh_geometry) {
	if (wheel_mesh != nullptr)
		wheel_mesh->release();
	wheel_mesh = convex_mesh_geometry.convexMesh;
}

void PhysicsVehicle::set_wheel_layout(float x_seperation, float y_displacement, float z_seperation, float z_displacement) {
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_LEFT] = physx::PxVec3((-x_seperation + wheelWidth) * 0.5f, -((chassisDims.y + y_displacement)/ 2 + wheelRadius), -z_seperation / 2 + wheelRadius + z_displacement);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eREAR_RIGHT] = physx::PxVec3((+x_seperation - wheelWidth) * 0.5f, -((chassisDims.y + y_displacement)/ 2 + wheelRadius), -z_seperation / 2 + wheelRadius + z_displacement);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_LEFT] = physx::PxVec3((-x_seperation + wheelWidth) * 0.5f, -((chassisDims.y + y_displacement)/ 2 + wheelRadius), z_seperation / 2 - wheelRadius + z_displacement);
	wheelOffsets[snippetvehicle::PxVehicleDrive4WWheelOrder::eFRONT_RIGHT] = physx::PxVec3((+x_seperation - wheelWidth) * 0.5f, -((chassisDims.y + y_displacement)/ 2 + wheelRadius), z_seperation / 2 - wheelRadius + z_displacement);
}

void PhysicsVehicle::vehicle_control(GLFWwindow* window) {
	InputData.setDigitalAccel(false);
	InputData.setDigitalBrake(false);
	InputData.setDigitalHandbrake(false);
	InputData.setDigitalSteerLeft(false);
	InputData.setDigitalSteerRight(false);

	vehicle_drive->mDriveDynData.setUseAutoGears(true);

	if (glfwGetKey(window, GLFW_KEY_W) == 1) {
		vehicle_drive->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		if (vehicle_drive->computeForwardSpeed() < -0.2)
			InputData.setDigitalBrake(true);
		InputData.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == 1) {
		if (vehicle_drive->computeForwardSpeed() > 0.2)
			InputData.setDigitalBrake(true);
		vehicle_drive->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eREVERSE);
		InputData.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1) {
		InputData.setDigitalSteerRight(true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == 1) {
		InputData.setDigitalSteerLeft(true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == 1) {
		InputData.setDigitalHandbrake(true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == 1 && glfwGetKey(window, GLFW_KEY_S) == 1) {
		vehicle_drive->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		InputData.setDigitalAccel(false);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1 && glfwGetKey(window, GLFW_KEY_D) == 1) {
		InputData.setDigitalSteerLeft(false);
		InputData.setDigitalSteerRight(false);
	}

	std::cout << physics_representation.vehicle_drive->mDriveDynData.getEngineRotationSpeed() << std::endl;

}

void PhysicsVehicle::set_position(float x, float y, float z) {
	physx::PxTransform transform = vehicle_actor->getGlobalPose();
	transform.p = physx::PxVec3(x, y, z);
	vehicle_actor->setGlobalPose(transform);
}

void PhysicsVehicle::set_rotation(float x, float y, float z, float w) {
	physx::PxTransform transform = vehicle_actor->getGlobalPose();
	transform.q = physx::PxQuat(x, y, z, w);
	vehicle_actor->setGlobalPose(transform);
}

physx::PxVec3 PhysicsVehicle::get_position() {
	return vehicle_actor->getGlobalPose().p;
}

physx::PxQuat PhysicsVehicle::get_rotation() {
	const physx::PxQuat quat = vehicle_actor->getGlobalPose().q;
	return quat;
}