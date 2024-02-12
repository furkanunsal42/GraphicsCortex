#include "Object.h"
#include "PhysicsGeometry.h"
#include "Default_Programs.h"

Object::Object(Graphic& graphic_representation, PhysicsObject& physics_representation) :
	graphics(graphic_representation), physics(physics_representation) { }

void Object::sync_with_physics() {
	physx::PxQuat physx_quat = physics.get_rotation();
	*graphics.rotation = glm::quat(physx_quat.w, physx_quat.x, physx_quat.y, physx_quat.z);
	(*graphics.position).x = physics.get_position().x;
	(*graphics.position).y = physics.get_position().y;
	(*graphics.position).z = physics.get_position().z;
}

void Object::set_position(glm::vec3 position){
	*graphics.position = position;
	physics.set_position(position);
}
void Object::set_rotation(glm::vec3 rotation){
	*graphics.rotation = glm::quat(rotation);
	physics.set_rotation(rotation);
}

Vehicle::Vehicle() : 
	physics_representation(PhysicsVehicle()) {
}
 
void Vehicle::set_mesh_chassis_graphics(Mesh chassis) {
	this->chassis.set_mesh(chassis);
	chassis_graphic_initialized = true;
}

void Vehicle::set_mesh_left_wheel_graphics(Mesh left_wheel) {
	this->wheels[FRONT_LEFT].set_mesh(left_wheel);
	this->wheels[REAR_LEFT].set_mesh(left_wheel);
	wheel_graphic_initialized = true;
}

void Vehicle::set_mesh_right_wheel_graphics(Mesh right_wheel) {
	this->wheels[FRONT_RIGHT].set_mesh(right_wheel);
	this->wheels[REAR_RIGHT].set_mesh(right_wheel);
	wheel_graphic_initialized = true;
}

void Vehicle::set_mesh_all_graphics(Mesh chassis, Mesh left_wheel, Mesh right_wheel) {
	set_mesh_chassis_graphics(chassis);
	set_mesh_left_wheel_graphics(left_wheel);
	set_mesh_right_wheel_graphics(right_wheel);
}

void Vehicle::set_model_chassis_physics(Model& chassis) {
	std::vector<physx::PxVec3> data = chassis.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_chasis_mesh(create_geometry::convex_hull(data));
}

void Vehicle::set_model_left_wheel_physics(Model& left_wheel) {
	std::vector<physx::PxVec3> data = left_wheel.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_wheel_mesh(create_geometry::convex_hull(data));
}
void Vehicle::set_model_right_wheel_physics(Model& right_wheel) {
	std::vector<physx::PxVec3> data = right_wheel.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_wheel_mesh(create_geometry::convex_hull(data));
}
void Vehicle::set_model_all_physics(Model& chassis, Model& left_wheel, Model& right_wheel) {
	set_model_chassis_physics(chassis);
	set_model_left_wheel_physics(left_wheel);
	set_model_right_wheel_physics(right_wheel);
}

void Vehicle::set_model_chassis_physics(Model&& chassis) {
	set_model_chassis_physics(chassis);
}
void Vehicle::set_model_left_wheel_physics(Model&& left_wheel) {
	set_model_left_wheel_physics(left_wheel);
}
void Vehicle::set_model_right_wheel_physics(Model&& right_wheel) {
	set_model_right_wheel_physics(right_wheel);
}
void Vehicle::set_model_all_physics(Model&& chassis, Model&& left_wheel, Model&& right_wheel) {
	set_model_all_physics(chassis, left_wheel, right_wheel);
}

void Vehicle::set_material_chassis(BindlessMaterial chassis) {
	this->chassis.set_material(chassis);
}
void Vehicle::set_material_left_wheel(BindlessMaterial left_wheel) {
	this->wheels[FRONT_LEFT].set_material(left_wheel);
	this->wheels[REAR_LEFT].set_material(left_wheel);
}
void Vehicle::set_material_right_wheel(BindlessMaterial right_wheel) {
	this->wheels[FRONT_RIGHT].set_material(right_wheel);
	this->wheels[REAR_RIGHT].set_material(right_wheel);
}
void Vehicle::set_material_all(BindlessMaterial chassis, BindlessMaterial left_wheel, BindlessMaterial right_wheel) {
	set_material_chassis(chassis);
	set_material_left_wheel(left_wheel);
	set_material_right_wheel(right_wheel);
}
void Vehicle::set_material_all(BindlessMaterial all) {
	set_material_all(all, all, all);
}

void Vehicle::sync_with_physics() {
	
	int shape_count = physics_representation.vehicle_actor->getNbShapes();
	if (shape_count != 5) {
		std::cout << "Vehicle::sync_with_physics() was called but number of shapes of physics representatoin is not 5" << std::endl;
		ASSERT(false);
	}

	physx::PxShape** shapes = new physx::PxShape*[shape_count];
	physics_representation.vehicle_actor->getShapes(shapes, shape_count, 0);

	physx::PxQuat global_rotation = physics_representation.get_rotation();
	physx::PxVec3 global_position = physics_representation.get_position();
	
	(*chassis.rotation) = glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z);
	(*chassis.position).x = global_position.x;
	(*chassis.position).y = global_position.y;
	(*chassis.position).z = global_position.z;
	
	for (Graphic& wheel : wheels) {
		*wheel.rotation = (glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z));
		*wheel.rotation = (glm::vec3(global_position.x, global_position.y, global_position.z));
	}

	{
		//chassis
		physx::PxTransform transform = shapes[4]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		*chassis.position = (*chassis.position + local_position);
		*chassis.rotation = (*chassis.rotation * local_rotation);
	}

	{
		physx::PxTransform transform = shapes[0]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		*wheels[FRONT_RIGHT].rotation = (*wheels[FRONT_RIGHT].rotation * local_rotation);
		*wheels[FRONT_RIGHT].position = (*wheels[FRONT_RIGHT].position + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[1]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		*wheels[FRONT_LEFT].rotation = (*wheels[FRONT_LEFT].rotation * local_rotation);
		*wheels[FRONT_LEFT].position = (*wheels[FRONT_LEFT].position + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[2]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		*wheels[REAR_RIGHT].rotation = (*wheels[REAR_RIGHT].rotation * local_rotation);
		*wheels[REAR_RIGHT].position = (*wheels[REAR_RIGHT].position + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[3]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		*wheels[REAR_LEFT].rotation = (*wheels[REAR_LEFT].rotation * local_rotation);
		*wheels[REAR_LEFT].position = (*wheels[REAR_LEFT].position + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}

	delete[] shapes;
}

void Vehicle::set_position(glm::vec3 position) {
	physics_representation.set_position(position);
	sync_with_physics();
}
void Vehicle::set_rotation(glm::quat rotation) {
	physics_representation.set_rotation(rotation);
	sync_with_physics();
}
void Vehicle::set_rotation(physx::PxQuat rotation) {
	physics_representation.set_rotation(rotation);
	sync_with_physics();
}

