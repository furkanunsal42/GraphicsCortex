#include "API/GraphicsCortex.h"

#include "vehicle/PxVehicleUtil.h"

#include "SnippetVehicleConcurrency.h"
#include "SnippetVehicleCreate.h"
#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleSceneQuery.h"
#include "SnippetVehicleTireFriction.h"
#include "SnippetVehicleWheelQueryResult.h"

snippetvehicle::VehicleDesc initVehicleDesc()
{
	//Set up the chassis mass, dimensions, moment of inertia, and center of mass offset.
	//The moment of inertia is just the moment of inertia of a cuboid but modified for easier steering.
	//Center of mass offset is 0.65m above the base of the chassis and 0.25m towards the front.
	const physx::PxF32 chassisMass = 1500.0f;
	const physx::PxVec3 chassisDims(2.5f, 2.0f, 5.0f);
	const physx::PxVec3 chassisMOI
	((chassisDims.y * chassisDims.y + chassisDims.z * chassisDims.z) * chassisMass / 12.0f,
		(chassisDims.x * chassisDims.x + chassisDims.z * chassisDims.z) * 0.8f * chassisMass / 12.0f,
		(chassisDims.x * chassisDims.x + chassisDims.y * chassisDims.y) * chassisMass / 12.0f);
	const physx::PxVec3 chassisCMOffset(0.0f, -chassisDims.y * 0.5f + 0.65f, 0.25f);

	//Set up the wheel mass, radius, width, moment of inertia, and number of wheels.
	//Moment of inertia is just the moment of inertia of a cylinder.
	const physx::PxF32 wheelMass = 20.0f;
	const physx::PxF32 wheelRadius = 0.5f;
	const physx::PxF32 wheelWidth = 0.1f;
	const physx::PxF32 wheelMOI = 0.5f * wheelMass * wheelRadius * wheelRadius;
	const physx::PxU32 nbWheels = 4;

	snippetvehicle::VehicleDesc vehicleDesc;

	vehicleDesc.chassisMass = chassisMass;
	vehicleDesc.chassisDims = chassisDims;
	vehicleDesc.chassisMOI = chassisMOI;
	vehicleDesc.chassisCMOffset = chassisCMOffset;
	physx::PxMaterial* material = PhysxContext::get().physics->createMaterial(0.5f, 0.5f, 0.5f);
	vehicleDesc.chassisMaterial = material;
	vehicleDesc.chassisSimFilterData = physx::PxFilterData(snippetvehicle::COLLISION_FLAG_CHASSIS, snippetvehicle::COLLISION_FLAG_CHASSIS_AGAINST, 0, 0);

	vehicleDesc.wheelMass = wheelMass;
	vehicleDesc.wheelRadius = wheelRadius;
	vehicleDesc.wheelWidth = wheelWidth;
	vehicleDesc.wheelMOI = wheelMOI;
	vehicleDesc.numWheels = nbWheels;
	vehicleDesc.wheelMaterial = material;
	vehicleDesc.chassisSimFilterData = physx::PxFilterData(snippetvehicle::COLLISION_FLAG_WHEEL, snippetvehicle::COLLISION_FLAG_WHEEL_AGAINST, 0, 0);

	return vehicleDesc;
}

void vehicle_control(GLFWwindow* window, physx::PxVehicleDrive4WRawInputData& vehicle_controller, physx::PxVehicleDrive4W* vehicle) {
	vehicle_controller.setDigitalAccel(false);
	vehicle_controller.setDigitalBrake(false);
	vehicle_controller.setDigitalHandbrake(false);
	vehicle_controller.setDigitalSteerLeft(false);
	vehicle_controller.setDigitalSteerRight(false);

	vehicle->mDriveDynData.setUseAutoGears(true);

	if (glfwGetKey(window, GLFW_KEY_W) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		vehicle_controller.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eREVERSE);
		vehicle_controller.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1) {
		vehicle_controller.setDigitalSteerLeft(true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == 1) {
		vehicle_controller.setDigitalSteerRight(true);
	}


	if (glfwGetKey(window, GLFW_KEY_W) == 1 && glfwGetKey(window, GLFW_KEY_S) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		vehicle_controller.setDigitalAccel(false);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1 && glfwGetKey(window, GLFW_KEY_D) == 1) {
		vehicle_controller.setDigitalSteerLeft(false);
		vehicle_controller.setDigitalSteerRight(false);
	}

}

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

	//Create a vehicle that will drive on the plane.
	snippetvehicle::VehicleDesc vehicleDesc = initVehicleDesc();
	physx::PxVehicleDrive4W* gVehicle4W = createVehicle4W(vehicleDesc, PhysxContext::get().physics, PhysxContext::get().physics_cooking);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y * 0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	gVehicle4W->getRigidDynamicActor()->setGlobalPose(startTransform);
	PhysxContext::get().physics_scene->addActor(*gVehicle4W->getRigidDynamicActor());

	//Set the vehicle to rest in first gear.
	//Set the vehicle to use auto-gears.
	gVehicle4W->setToRestState();
	gVehicle4W->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
	gVehicle4W->mDriveDynData.setUseAutoGears(true);

	physx::PxVehicleDrive4WRawInputData gVehicleInputData;

	//gVehicleInputData.setDigitalBrake(true);
	gVehicleInputData.setDigitalBrake(false);
	gVehicleInputData.setDigitalHandbrake(false);
	gVehicleInputData.setDigitalAccel(true);

	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(box);
	box.set_position(4.0f, 10.0f, 4.0f);

	bool is_vehicle_in_air = true;


	physx::PxVehicleKeySmoothingData gKeySmoothingData =
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

	physx::PxF32 gSteerVsForwardSpeedData[2 * 8] =
	{
		0.0f,		0.75f,
		5.0f,		0.75f,
		30.0f,		0.125f,
		120.0f,		0.1f,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32,
		PX_MAX_F32, PX_MAX_F32
	};

	physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable(gSteerVsForwardSpeedData, 4);
	float timestep = 1 / 60.0f;

	GLFWwindow* window = frame::create_window(100, 100, "vehicle_control", 0, 1);

	while (true) {
		glfwPollEvents();
		frame::clear_window();

		vehicle_control(window, gVehicleInputData, gVehicle4W);

		//Raycasts.
		physx::PxVehicleWheels* vehicles[1] = { gVehicle4W };
		physx::PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
		const physx::PxU32 raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
		PxVehicleSuspensionRaycasts(gBatchQuery, 1, vehicles, raycastResultsSize, raycastResults);

		PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(gKeySmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData, timestep, is_vehicle_in_air, *gVehicle4W);

		//Vehicle update.
		const physx::PxVec3 grav = PhysxContext::get().physics_scene->getGravity();
		physx::PxWheelQueryResult wheelQueryResults[PX_MAX_NB_WHEELS];
		physx::PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
		PxVehicleUpdates(timestep, grav, *gFrictionPairs, 1, vehicles, vehicleQueryResults);

		//Work out if the vehicle is in the air.
		is_vehicle_in_air = gVehicle4W->getRigidDynamicActor()->isSleeping() ? false : PxVehicleIsInAir(vehicleQueryResults[0]);

		scene.simulation_step_start(timestep);
		scene.simulation_step_finish();

		glfwSwapBuffers(window);
	}

}