#include "Object.h"
#include "PhysicsGeometry.h"
#include "Default_Programs.h"

Object::Object(Graphic& graphic_representation, PhysicsObject& physics_representation) :
	graphics(graphic_representation), physics(physics_representation) { }

void Object::sync_with_physics() {
	physx::PxQuat physx_quat = physics.get_rotation();
	graphics.rotation = glm::quat(physx_quat.w, physx_quat.x, physx_quat.y, physx_quat.z);
	graphics.position.x = physics.get_position().x;
	graphics.position.y = physics.get_position().y;
	graphics.position.z = physics.get_position().z;
}

void Object::set_position(glm::vec3 position){
	graphics.set_position(position);
	physics.set_position(position);
}
void Object::set_rotation(glm::vec3 rotation){
	graphics.set_rotation(rotation);
	physics.set_rotation(rotation);
}

Vehicle::Vehicle() : 
	physics_representation(PhysicsVehicle()) {
}
 
void Vehicle::load_mesh_chassis_graphics(std::shared_ptr<Mesh> chassis) {
	this->chassis->load_model(chassis);
	chassis_graphic_initialized = true;
}

void Vehicle::load_mesh_left_wheel_graphics(std::shared_ptr<Mesh> left_wheel) {
	this->wheels[FRONT_LEFT]->load_model(left_wheel);
	this->wheels[REAR_LEFT]->load_model(left_wheel);
	wheel_graphic_initialized = true;
}

void Vehicle::load_mesh_right_wheel_graphics(std::shared_ptr<Mesh> right_wheel) {
	this->wheels[FRONT_RIGHT]->load_model(right_wheel);
	this->wheels[REAR_RIGHT]->load_model(right_wheel);
	wheel_graphic_initialized = true;
}

void Vehicle::load_mesh_all_graphics(std::shared_ptr<Mesh> chassis, std::shared_ptr<Mesh> left_wheel, std::shared_ptr<Mesh> right_wheel) {
	load_mesh_chassis_graphics(chassis);
	load_mesh_left_wheel_graphics(left_wheel);
	load_mesh_right_wheel_graphics(right_wheel);
}

void Vehicle::load_model_chassis_physics(Model& chassis) {
	std::vector<physx::PxVec3> data = chassis.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_chasis_mesh(create_geometry::convex_hull(data));
}

void Vehicle::load_model_left_wheel_physics(Model& left_wheel) {
	std::vector<physx::PxVec3> data = left_wheel.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_wheel_mesh(create_geometry::convex_hull(data));
}
void Vehicle::load_model_right_wheel_physics(Model& right_wheel) {
	std::vector<physx::PxVec3> data = right_wheel.get_partial_data<physx::PxVec3>("111");
	this->physics_representation.set_wheel_mesh(create_geometry::convex_hull(data));
}
void Vehicle::load_model_all_physics(Model& chassis, Model& left_wheel, Model& right_wheel) {
	load_model_chassis_physics(chassis);
	load_model_left_wheel_physics(left_wheel);
	load_model_right_wheel_physics(right_wheel);
}

void Vehicle::load_model_chassis_physics(Model&& chassis) {
	load_model_chassis_physics(chassis);
}
void Vehicle::load_model_left_wheel_physics(Model&& left_wheel) {
	load_model_left_wheel_physics(left_wheel);
}
void Vehicle::load_model_right_wheel_physics(Model&& right_wheel) {
	load_model_right_wheel_physics(right_wheel);
}
void Vehicle::load_model_all_physics(Model&& chassis, Model&& left_wheel, Model&& right_wheel) {
	load_model_all_physics(chassis, left_wheel, right_wheel);
}

void Vehicle::load_material_chassis(std::shared_ptr<Material> chassis) {
	this->chassis->load_material(chassis);
}
void Vehicle::load_material_left_wheel(std::shared_ptr<Material> left_wheel) {
	this->wheels[FRONT_LEFT]->load_material(left_wheel);
	this->wheels[REAR_LEFT]->load_material(left_wheel);
}
void Vehicle::load_material_right_wheel(std::shared_ptr<Material> right_wheel) {
	this->wheels[FRONT_RIGHT]->load_material(right_wheel);
	this->wheels[REAR_RIGHT]->load_material(right_wheel);
}
void Vehicle::load_material_all(std::shared_ptr<Material> chassis, std::shared_ptr<Material> left_wheel, std::shared_ptr<Material> right_wheel) {
	load_material_chassis(chassis);
	load_material_left_wheel(left_wheel);
	load_material_right_wheel(right_wheel);
}
void Vehicle::load_material_all(std::shared_ptr<Material> all) {
	load_material_all(all, all, all);
}

void Vehicle::load_material_chassis(std::shared_ptr<UnorderedMaterial> chassis) {
	this->chassis->load_material(chassis);
}
void Vehicle::load_material_left_wheel(std::shared_ptr<UnorderedMaterial> left_wheel) {
	this->wheels[FRONT_LEFT]->load_material(left_wheel);
	this->wheels[REAR_LEFT]->load_material(left_wheel);
}
void Vehicle::load_material_right_wheel(std::shared_ptr<UnorderedMaterial> right_wheel) {
	this->wheels[FRONT_RIGHT]->load_material(right_wheel);
	this->wheels[REAR_RIGHT]->load_material(right_wheel);
}
void Vehicle::load_material_all(std::shared_ptr<UnorderedMaterial> chassis, std::shared_ptr<UnorderedMaterial> left_wheel, std::shared_ptr<UnorderedMaterial> right_wheel) {
	load_material_chassis(chassis);
	load_material_left_wheel(left_wheel);
	load_material_right_wheel(right_wheel);
}
void Vehicle::load_material_all(std::shared_ptr<UnorderedMaterial> all) {
	load_material_all(all, all, all);
}

void Vehicle::load_program_chassis(std::shared_ptr<Program> chassis) {
	this->chassis->load_program(chassis);
}
void Vehicle::load_program_left_wheel(std::shared_ptr<Program> left_wheel) {
	this->wheels[FRONT_LEFT]->load_program(left_wheel);
	this->wheels[REAR_LEFT]->load_program(left_wheel);
}
void Vehicle::load_program_right_wheel(std::shared_ptr<Program> right_wheel) {
	this->wheels[FRONT_RIGHT]->load_program(right_wheel);
	this->wheels[REAR_RIGHT]->load_program(right_wheel);
}
void Vehicle::load_program_all(std::shared_ptr<Program> chassis, std::shared_ptr<Program> left_wheel, std::shared_ptr<Program> right_wheel) {
	load_program_chassis(chassis);
	load_program_left_wheel(left_wheel);
	load_program_right_wheel(right_wheel);
}
void Vehicle::load_program_all(std::shared_ptr<Program> all) {
	load_program_chassis(all);
	load_program_left_wheel(all);
	load_program_right_wheel(all);
}

void Vehicle::set_uniform_queue_chassis(uniform_update_queue& chassis) {
	this->chassis->set_uniform_all(chassis);
}
void Vehicle::set_uniform_queue_left_wheel(uniform_update_queue& left_wheel) {
	this->wheels[FRONT_LEFT]->set_uniform_all(left_wheel);
	this->wheels[REAR_LEFT]->set_uniform_all(left_wheel);
}
void Vehicle::set_uniform_queue_right_wheel(uniform_update_queue& right_wheel) {
	this->wheels[FRONT_RIGHT]->set_uniform_all(right_wheel);
	this->wheels[REAR_RIGHT]->set_uniform_all(right_wheel);
}
void Vehicle::set_uniform_queue_all(uniform_update_queue& chassis, uniform_update_queue& left_wheel, uniform_update_queue& right_wheel) {
	set_uniform_queue_chassis(chassis);
	set_uniform_queue_left_wheel(left_wheel);
	set_uniform_queue_right_wheel(right_wheel);
}
void Vehicle::set_uniform_queue_all(uniform_update_queue& all) {
	set_uniform_queue_all(all, all, all);
}

void Vehicle::set_uniform_queue_chassis(uniform_update_queue&& chassis) {
	set_uniform_queue_chassis(chassis);
}
void Vehicle::set_uniform_queue_left_wheel(uniform_update_queue&& left_wheel) {
	set_uniform_queue_left_wheel(left_wheel);
}
void Vehicle::set_uniform_queue_right_wheel(uniform_update_queue&& right_wheel) {
	set_uniform_queue_right_wheel(right_wheel);
}
void Vehicle::set_uniform_queue_all(uniform_update_queue&& chassis, uniform_update_queue&& left_wheel, uniform_update_queue&& right_wheel) {
	set_uniform_queue_all(chassis, left_wheel, right_wheel);
}
void Vehicle::set_uniform_queue_all(uniform_update_queue&& all) {
	set_uniform_queue_all(all, all, all);
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
	
	chassis->rotation = glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z);
	chassis->position.x = global_position.x;
	chassis->position.y = global_position.y;
	chassis->position.z = global_position.z;
	
	for (std::shared_ptr<Graphic>& wheel : wheels) {
		wheel->set_rotation(glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z));
		wheel->set_position(glm::vec3(global_position.x, global_position.y, global_position.z));
	}

	{
		//chassis
		physx::PxTransform transform = shapes[4]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		chassis->set_position(chassis->get_position() + local_position);
		chassis->set_rotation(chassis->get_rotation() * local_rotation);
	}

	{
		physx::PxTransform transform = shapes[0]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		wheels[FRONT_RIGHT]->set_rotation(wheels[FRONT_RIGHT]->get_rotation() * local_rotation);
		wheels[FRONT_RIGHT]->set_position(wheels[FRONT_RIGHT]->get_position() + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[1]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		wheels[FRONT_LEFT]->set_rotation(wheels[FRONT_LEFT]->get_rotation() * local_rotation);
		wheels[FRONT_LEFT]->set_position(wheels[FRONT_LEFT]->get_position() + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[2]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		wheels[REAR_RIGHT]->set_rotation(wheels[REAR_RIGHT]->get_rotation() * local_rotation);
		wheels[REAR_RIGHT]->set_position(wheels[REAR_RIGHT]->get_position() + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
	}
	{
		physx::PxTransform transform = shapes[3]->getLocalPose();
		glm::vec3 local_position = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
		glm::quat local_rotation = glm::quat(transform.q.w, transform.q.x, transform.q.y, transform.q.z);
		wheels[REAR_LEFT]->set_rotation(wheels[REAR_LEFT]->get_rotation() * local_rotation);
		wheels[REAR_LEFT]->set_position(wheels[REAR_LEFT]->get_position() + glm::quat(global_rotation.w, global_rotation.x, global_rotation.y, global_rotation.z) * local_position);
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

