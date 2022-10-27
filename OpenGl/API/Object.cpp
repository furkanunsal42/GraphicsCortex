#include "Object.h"

Object::Object(Graphic& graphic_representation, PhysicsObject& physics_representation) :
	graphics(graphic_representation), physics(physics_representation) { }

void Object::sync_with_physics() {
	physx::PxQuat physx_quat = physics.get_rotation();
	graphics.rotation_quat = glm::quat(physx_quat.w, physx_quat.x, physx_quat.y, physx_quat.z);
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

