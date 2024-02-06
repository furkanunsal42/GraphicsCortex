#include "PhysicsLink.h"
#include "PhysicsObject.h"

PhysicsLink::PhysicsLink(PhysicsObject& actor0, PhysicsObject& actor1, unsigned int link_type) : // PhysicsLink::type instead of unsigned int
	type(link_type), actor0(&actor0), actor1(&actor1)
{
	physx::PxVec3 offset(1.5f, 0, 0);
	auto context = PhysxContext::get();
	if (link_type == FIXED){
		joint = physx::PxFixedJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	else if (link_type == SPHERICAL) {
		joint = physx::PxSphericalJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	else if (link_type == REVOLUTE) {
		joint = physx::PxRevoluteJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	else if (link_type == PRISMATIC) {
		joint = physx::PxPrismaticJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	else if (link_type == DISTANCE) {
		joint = physx::PxDistanceJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	else if (link_type == D6) {
		joint = physx::PxD6JointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	}
	joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
}

void PhysicsLink::remove() {
	std::vector<PhysicsLink*>::iterator index0 = std::find(actor0->links.begin(), actor0->links.end(), this);
	std::vector<PhysicsLink*>::iterator index1 = std::find(actor1->links.begin(), actor0->links.end(), this);
	actor0->links.erase(index0);
	actor1->links.erase(index1);

	joint->release();
}
