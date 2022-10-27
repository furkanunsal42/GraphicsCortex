#pragma once

#include "Graphic.h"
#include "PhysicsVehicle.h"

// objects are structures that sync physics and graphics
class Object {
public:
	Graphic& graphics;
	PhysicsObject& physics;

	Object(Graphic& graphics, PhysicsObject& physics);

	void sync_with_physics();
	
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);

};

/*
class Vehicle : Object{

	PhysicsVehicle physics_representation;

	Graphic chassis;
	Graphic wheels[4];

	Vehicle();

	void sync_with_physics();

};
*/