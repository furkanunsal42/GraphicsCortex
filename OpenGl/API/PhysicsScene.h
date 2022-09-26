#pragma once
#include "PhysicsContext.h"
#include <vector>


class PhysicsScene {
public:

	PhysicsScene() {}

	std::vector<std::reference_wrapper<PhysicsObject>> actors;

	void add_actor(PhysicsObject& actor) {
		auto context = PhysxContext::get();
		context.physics_scene->addActor(*actor.actor);
		actors.push_back(actor);
	}

	void simulate_step(long double delta_time) {
		simulation_step_start(delta_time);
		simulation_step_finish();
	}

	void simulation_step_start(long double delta_time) {
		auto context = PhysxContext::get();
		context.physics_scene->simulate(delta_time);
	}

	void simulation_step_finish() {
		auto context = PhysxContext::get();
		context.physics_scene->fetchResults(true);
	}
};