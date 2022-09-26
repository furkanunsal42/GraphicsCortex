#include "PhysicsObject.h"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include <iostream>

PhysicsObject::PhysicsObject(const physx::PxGeometry& geometry, enum type type = type::DYNAMIC, bool exclusive_shape = false) :
	position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape), type(type)
{

	auto context = PhysxContext::get();
	material = context.physics->createMaterial(0.5f, 0.5f, 0.5f);
	update_transform();
	if (type == type::DYNAMIC || type == type::KINEMATIC)
		actor = context.physics->createRigidDynamic(transform);
	if (type == type::STATIC)
		actor = context.physics->createRigidStatic(transform);

	create_shape(geometry, *material, exclusive_shape);
}

PhysicsObject::PhysicsObject(const physx::PxPlane& plane, bool exclusive_shape = false) :
	position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape), type(type::STATIC), shape(nullptr)
{
	auto context = PhysxContext::get();
	material = context.physics->createMaterial(0.5f, 0.5f, 0.5f);
	actor = physx::PxCreatePlane(*context.physics, plane, *material);
}

void PhysicsObject::set_type(unsigned int new_type) {
	if (type == type::STATIC) {
		if (type != new_type)
			std::cout << "PhysicsObject.set_type(unsigned int new_type) is called but staic type cannot be changed.\n";
		return;
	}
	if (new_type == type::DYNAMIC)
		((physx::PxRigidBody*)actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
	if (new_type == type::KINEMATIC)
		((physx::PxRigidBody*)actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
}

void PhysicsObject::create_shape(const physx::PxGeometry& geometry, const physx::PxMaterial& material, bool exclusive_shape = false) {
	auto context = PhysxContext::get();
	shape = context.physics->createShape(geometry, material, exclusive_shape);
	actor->attachShape(*shape);
}

void PhysicsObject::set_position(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;

	update_transform();
}

template<typename T>
std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, void>
	PhysicsObject::set_position(T rotation_vector) {
	set_position(rotation_vector.x, rotation_vector.y, rotation_vector.z);
}

void PhysicsObject::set_rotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	update_transform();
}

template<typename T>
std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, void>
	PhysicsObject::set_rotatoin(T rotation_vector) {
	set_rotation(rotation_vector.x, rotation_vector.y, rotation_vector.z);
}

void PhysicsObject::update_transform() {
	glm::quat rotatoion_quat_glm(glm::vec3(rotation.x, rotation.y, rotation.z));
	physx::PxQuat rotation_quat(rotatoion_quat_glm.x, rotatoion_quat_glm.y, rotatoion_quat_glm.z, rotatoion_quat_glm.w);
	transform = physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), rotation_quat);

	if (actor != nullptr)
		actor->setGlobalPose(transform);
}

void PhysicsObject::add_link(PhysicsObject& other, enum PhysicsLink::type link_type = PhysicsLink::type::FIXED) {
	PhysicsLink* link = new PhysicsLink(*this, other, link_type);
	other.links.push_back(link);
	links.push_back(link);
}

void PhysicsObject::remove_link(PhysicsLink* link) {
	std::vector<PhysicsLink*>::iterator index = std::find(links.begin(), links.end(), link);

	if (index == links.end()) {
		std::cout << "[PhysX ERROR] PhysicsObject.remove_link() is called but given link was not a part of this object. \n";
		return;
	}

	link->remove();
	delete link;
}

