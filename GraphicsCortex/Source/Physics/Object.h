#pragma once

#include "Graphic.h"
#include "PhysicsObject.h"
#include "PhysicsVehicle.h"
#include <memory>

class Camera;
class Scene;

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


class Vehicle/* : Object */ {
public:

	PhysicsVehicle physics_representation;

	Graphic chassis;
	Graphic wheels[4] = { Graphic() , Graphic(), Graphic(), Graphic() };

	bool chassis_graphic_initialized = false;
	bool wheel_graphic_initialized = false;

	enum wheel_index {
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		REAR_LEFT,
		REAR_RIGHT,
	};

	Vehicle();

	void set_mesh_chassis_graphics(Mesh chassis);
	void set_mesh_left_wheel_graphics(Mesh left_wheel);
	void set_mesh_right_wheel_graphics(Mesh right_wheel);
	void set_mesh_all_graphics(Mesh chassis, Mesh left_wheel, Mesh right_wheel);

	void set_model_chassis_physics(Model& chassis);
	void set_model_left_wheel_physics(Model& left_wheel);
	void set_model_right_wheel_physics(Model& right_wheel);
	void set_model_all_physics(Model& chassis, Model& left_wheel, Model& right_wheel);

	void set_model_chassis_physics(Model&& chassis);
	void set_model_left_wheel_physics(Model&& left_wheel);
	void set_model_right_wheel_physics(Model&& right_wheel);
	void set_model_all_physics(Model&& chassis, Model&& left_wheel, Model&& right_wheel);

	void set_material_chassis(BindlessMaterial chassis);
	void set_material_left_wheel(BindlessMaterial left_wheel);
	void set_material_right_wheel(BindlessMaterial right_wheel);
	void set_material_all(BindlessMaterial chassis, BindlessMaterial left_wheel, BindlessMaterial right_wheel);
	void set_material_all(BindlessMaterial all);

	void sync_with_physics();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);
	void set_rotation(physx::PxQuat rotation);
};
