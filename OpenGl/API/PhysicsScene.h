#pragma once
#include "PhysicsContext.h"
#include "PhysicsObject.h"
#include <vector>

class PhysicsScene {
public:

	PhysicsScene();

	std::vector<std::reference_wrapper<PhysicsObject>> actors;

	void add_actor(PhysicsObject& actor);
	void simulate_step(long double delta_time);
	void simulation_step_start(long double delta_time);
	void simulation_step_finish();

};