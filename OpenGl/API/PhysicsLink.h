#pragma once
#include "PhysicsObject.h"
#include "PxPhysicsAPI.h"

#include "glm.hpp"

#include <type_traits>
#include <iostream>

class PhysicsObject;

class PhysicsLink {
public:
	PhysicsObject* actor0;
	PhysicsObject* actor1;

	physx::PxJoint* joint;

	unsigned int type;

	enum type {
		FIXED = 0,
		SPHERICAL,
		REVOLUTE,
		PRISMATIC,
		DISTANCE,
		D6,
	};

	PhysicsLink(PhysicsObject& actor0, PhysicsObject& actor1, unsigned int link_type); // PhysicsLink::type insted of unsigned int

	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, void>
		set_limit(T limit) {
		
		if (type == SPHERICAL) {
			((physx::PxSphericalJoint*)joint)->setLimitCone(physx::PxJointLimitCone(limit.x, limit.y, limit.z));
			((physx::PxSphericalJoint*)joint)->setSphericalJointFlag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
		}
		else if (type == REVOLUTE) {
			((physx::PxRevoluteJoint*)joint)->setLimit(physx::PxJointAngularLimitPair(limit.x, limit.y, limit.z));
			((physx::PxRevoluteJoint*)joint)->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}
		else if (type == PRISMATIC) {
			auto context = PhysxContext::get();
			((physx::PxPrismaticJoint*)joint)->setLimit(physx::PxJointLinearLimitPair(context.physics->getTolerancesScale(), limit.x, limit.y, limit.z));
			((physx::PxPrismaticJoint*)joint)->setPrismaticJointFlag(physx::PxPrismaticJointFlag::eLIMIT_ENABLED, true);
		}
		else {
			std::cout << "[Physx Error] PhysicsLink.set_limit() is called but link type doesn't have set_limit property.\n";
		}

	}
	


	void remove();
};
