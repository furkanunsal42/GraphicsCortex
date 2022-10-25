#pragma once
#include "PhysicsContext.h"
#include "PhysicsObject.h"
#include "PhysicsVehicle.h"
#include <vector>

class PhysicsScene {
public:

	PhysicsScene();

	std::vector<std::reference_wrapper<PhysicsObject>> objects;
	std::vector<std::reference_wrapper<PhysicsVehicle>> vehicles;
	std::vector<std::reference_wrapper<physx::PxRigidActor*>> actors;

	long double delta_time_seconds = 1 / 60.0f;
	void add_actor(PhysicsObject& object);
	void add_actor(PhysicsObject&& object);
	void add_actor(PhysicsVehicle& vehicle);
	void add_actor(PhysicsVehicle&& vehicle);
	void add_actor(physx::PxRigidActor*& actor);
	void simulate_step(long double delta_time);
	void simulation_step_start(long double delta_time);
	void simulation_step_finish();

private:
	long double _leftover_time = 0;
	bool _simulation_active = false;

};