#include "PhysicsScene.h"

PhysicsScene::PhysicsScene() {}

void PhysicsScene::add_actor(PhysicsObject& object) {
	auto context = PhysxContext::get();
	context.physics_scene->addActor(*object.actor);
	objects.push_back(object);
}
void PhysicsScene::add_actor(PhysicsObject&& object) {
	auto context = PhysxContext::get();
	context.physics_scene->addActor(*object.actor);
	objects.push_back(object);
}


void PhysicsScene::add_actor(PhysicsVehicle& vehicle) {
	auto context = PhysxContext::get();
	context.physics_scene->addActor(*vehicle.vehicle_actor);
	vehicles.push_back(vehicle);
}
void PhysicsScene::add_actor(PhysicsVehicle&& vehicle) {
	auto context = PhysxContext::get();
	context.physics_scene->addActor(*vehicle.vehicle_actor);
	vehicles.push_back(vehicle);
}

void PhysicsScene::add_actor(physx::PxRigidActor*& actor) {
	auto context = PhysxContext::get();
	context.physics_scene->addActor(*actor);
	actors.push_back(actor);
}

void PhysicsScene::simulate_step(long double delta_time) {
	simulation_step_start(delta_time);
	simulation_step_finish();
}

void PhysicsScene::simulation_step_start(long double delta_time) {
	auto context = PhysxContext::get();
	
	_leftover_time += delta_time;
	
	// main iteration
	if (_leftover_time > delta_time_seconds) {
		for (PhysicsVehicle& vehicle : vehicles) {
			vehicle.simulation_step(delta_time_seconds);
		}
		context.physics_scene->simulate(delta_time_seconds);

		_leftover_time -= delta_time_seconds;
		_simulation_active = true;

		if (_leftover_time > delta_time_seconds) {
			simulation_step_finish();
			_leftover_time -= delta_time_seconds;
			simulation_step_start(delta_time_seconds);
		}
	}
}

void PhysicsScene::simulation_step_finish() {
	if (_simulation_active) {
		auto context = PhysxContext::get();
		context.physics_scene->fetchResults(true);
		_simulation_active = false;
	}
}
