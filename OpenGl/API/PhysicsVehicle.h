#pragma once

#include <glm.hpp>

#include <PxPhysicsAPI.h>
#include <GLFW/glfw3.h>

#include "SnippetVehicleSceneQuery.h"

#include <type_traits>

class PhysicsVehicle {
public:
	float chassisMass;
	physx::PxVec3 chassisDims;
	physx::PxVec3 chassisMOI;
	physx::PxVec3 chassisCMOffset;
	physx::PxMaterial* chassisMaterial;
	physx::PxFilterData chassisSimFilterData;  //word0 = collide type, word1 = collide against types, word2 = PxPairFlags

	float wheelMass;
	float wheelWidth;
	float wheelRadius;
	float wheelMOI;
	physx::PxMaterial* wheelMaterial;
	int numWheels;
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
	physx::PxConvexMesh* wheel_mesh;

	// engine
	physx::PxVehicleDifferential4WData::Enum differential_type;
	float engine_peak_torque;
	float engine_peak_revolution_speed;
	float gear_switch_time;
	float clutch_strength;
	float ackermann_accuracy;

	physx::PxVehicleWheelsSimData* wheelsSimData;
	physx::PxVehicleDriveSimData4W driveSimData;

	enum wheel_type {
		NORMAL = 0,
		WORN,
	};
	
	//wheel
	wheel_type wheel_type;
	physx::PxVec3* wheelOffsets; // array of PxVec3, a vector for each wheel
	float max_steer;
	float max_handbreak_torque;
	
	// suspension
	float max_suspension_compression;
	float max_suspension_droop;
	float suspension_string_strength;
	float suspension_spring_damper_rate;
	physx::PxVec3 suspension_travel_direction;
	float suspension_force_application_offset;	//Suspension force application point 0.3 metres below 
	
	float tire_force_application_offset;		//tire force application point 0.3 metres below 

	// camber angles
	float camber_angle_at_rest;
	float camber_angle_at_max_droop;
	float camber_angle_at_max_compression;

	// anti roll bar
	float antiroll_bar_stiffness_front;
	float antiroll_bar_stiffness_rear;


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

	void compile();
	
	void set_gear(gear gear);
	void set_gear_autouse(bool autouse);

	void simulation_step(long double timestep);
	
	void set_chasis_mesh(physx::PxConvexMesh* convex_mesh);
	void set_chasis_mesh(physx::PxConvexMeshGeometry convex_mesh_geometry);
	
	void set_wheel_mesh(physx::PxConvexMesh* convex_mesh);
	void set_wheel_mesh(physx::PxConvexMeshGeometry convex_mesh_geometry);

	void set_wheel_layout(float x_seperation, float y_displacement, float z_seperation, float z_displacement);

	void vehicle_control(GLFWwindow* window);
	
	void set_position(float x, float y, float z);

	template<typename T>
	std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, void>
		set_position(T rotation_vector) {
		set_position(rotation_vector.x, rotation_vector.y, rotation_vector.z);
	}

	void set_rotation(float x, float y, float z, float w);

	template<typename T>
	std::enable_if_t<std::is_same<T, glm::quat>::value || std::is_same<T, physx::PxQuat>::value, void>
		set_rotation(T rotation_quat) {
		set_rotation(rotation_quat.x, rotation_quat.y, rotation_quat.z, rotation_quat.w);
	}

	physx::PxVec3 get_position();
	physx::PxQuat get_rotation();

	private:
	void _initialize_box_chassis_mesh();
	void _initialize_cylinder_wheel_mesh();
	void _calculate_default_wheel_offsets();
	void _create_actor();
	void _create_wheel_sim();
	void _create_drive_sim();
	void _create_drive();
	void _create_control();
};