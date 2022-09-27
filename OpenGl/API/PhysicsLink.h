#pragma once
#include "PhysicsObject.h"
#include "PxPhysicsAPI.h"

class PhysicsObject;

class PhysicsLink {
public:
	PhysicsObject* actor0;
	PhysicsObject* actor1;

	physx::PxJoint* joint;

	unsigned int type;

	enum type {
		FIXED = 0,
		DISTANCE,
		SPHERICAL,
		REVOLUTE,
		PRISMATIC,
		D6,
	};

	PhysicsLink(PhysicsObject& actor0, PhysicsObject& actor1, unsigned int link_type); // PhysicsLink::type insted of unsigned int
	void remove();
};
