#include "PhysicsLink.h"
#include "PhysicsObject.h"

PhysicsLink::PhysicsLink(PhysicsObject& actor0, PhysicsObject& actor1, enum PhysicsLink::type link_type) :
	type(link_type), actor0(&actor0), actor1(&actor1)
{
	physx::PxVec3 offset(0.5f, 0, 0);
	auto context = PhysxContext::get();
	joint = physx::PxFixedJointCreate(*context.physics, actor0.actor, physx::PxTransform(-offset), actor1.actor, physx::PxTransform(offset));
	joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);

}

void PhysicsLink::remove() {
	std::vector<PhysicsLink*>::iterator index0 = std::find(actor0->links.begin(), actor0->links.end(), this);
	std::vector<PhysicsLink*>::iterator index1 = std::find(actor1->links.begin(), actor0->links.end(), this);
	actor0->links.erase(index0);
	actor1->links.erase(index1);

	joint->release();
}
