#include "PhysicsObject.h"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"

#include "SnippetVehicleFilterShader.h"
#include "SnippetVehicleSceneQuery.h"

#include <iostream>

PhysicsObject::PhysicsObject(const physx::PxGeometry& geometry, enum type type, bool exclusive_shape) :
	position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape), type(type), actor(nullptr)
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

PhysicsObject::PhysicsObject(const physx::PxPlane& plane, bool exclusive_shape) :
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

void PhysicsObject::create_shape(const physx::PxGeometry& geometry, const physx::PxMaterial& material, bool exclusive_shape) {
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

void PhysicsObject::set_rotation(float x, float y, float z) {
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;

	update_transform();
}

void PhysicsObject::update_transform() {
	glm::quat rotatoion_quat_glm(glm::vec3(rotation.x, rotation.y, rotation.z));
	physx::PxQuat rotation_quat(rotatoion_quat_glm.x, rotatoion_quat_glm.y, rotatoion_quat_glm.z, rotatoion_quat_glm.w);
	transform = physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), rotation_quat);

	if (actor != nullptr)
		actor->setGlobalPose(transform);
}
PhysicsLink* PhysicsObject::add_link(PhysicsObject& other, unsigned int link_type) {
	PhysicsLink* link = new PhysicsLink(*this, other, link_type);
	other.links.push_back(link);
	links.push_back(link);
	return link;
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

physx::PxVec3 PhysicsObject::get_position() {
	return actor->getGlobalPose().p;
}

physx::PxVec3 PhysicsObject::get_rotation() {
	const physx::PxQuat& quat = actor->getGlobalPose().q;
	physx::PxVec3 euler;

	double sinr_cosp = 2 * (quat.w * quat.x + quat.y * quat.z);
	double cosr_cosp = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	double sinp = 2 * (quat.w * quat.y - quat.z * quat.x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysign(physx::PxPi / 2, sinp); // use 90 degrees if out of range
	else
		euler.y = std::asin(sinp);

	double siny_cosp = 2 * (quat.w * quat.z + quat.x * quat.y);
	double cosy_cosp = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	return euler;

}

void PhysicsObject::set_gravity(bool enable_gravity) {
	actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enable_gravity);
}

void PhysicsObject::make_drivable() {
	physx::PxShape* shapes[1];
	actor->getShapes(shapes, 1);
	physx::PxFilterData qryFilterData(0, 0, 0, static_cast<physx::PxU32>(snippetvehicle::DRIVABLE_SURFACE));
	physx::PxFilterData groundPlaneSimFilterData(snippetvehicle::COLLISION_FLAG_GROUND, snippetvehicle::COLLISION_FLAG_GROUND_AGAINST, 0, 0);
	shapes[0]->setQueryFilterData(qryFilterData);
	shapes[0]->setSimulationFilterData(qryFilterData);
}
