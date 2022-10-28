#pragma once

#include "Graphic.h"
#include "PhysicsVehicle.h"

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
	PhysicsVehicle& physics_representation;

	Graphic chassis;
	Graphic wheels[4];

	enum wheel_index {
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		REAR_LEFT,
		REAR_RIGHT,
	};

	Vehicle(PhysicsVehicle& physics_representation);

	void load_model_chassis(Model& chassis, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_left_wheel(Model& left_wheel, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_right_wheel(Model& right_wheel, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_all(Model& chassis, Model& left_wheel, Model& right_wheel, bool use_for_graphics = true, bool use_for_physics = true);

	void load_model_chassis(Model&& chassis, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_left_wheel(Model&& left_wheel, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_right_wheel(Model&& right_wheel, bool use_for_graphics = true, bool use_for_physics = true);
	void load_model_all(Model&& chassis, Model&& left_wheel, Model&& right_wheel, bool use_for_graphics = true, bool use_for_physics = true);

	void load_material_chassis(Material& chassis);
	void load_material_left_wheel(Material& left_wheel);
	void load_material_right_wheel(Material& right_wheel);
	void load_material_all(Material& chassis, Material& left_wheel, Material& right_wheel);
	void load_material_all(Material& all);

	void load_program_chassis(Program& chassis);
	void load_program_left_wheel(Program& left_wheel);
	void load_program_right_wheel(Program& right_wheel);
	void load_program_all(Program& chassis, Program& left_wheel, Program& right_wheel);
	void load_program_all(Program& all);

	void set_uniform_queue_chassis(uniform_update_queue& chassis);
	void set_uniform_queue_left_wheel(uniform_update_queue& left_wheel);
	void set_uniform_queue_right_wheel(uniform_update_queue& right_wheel);
	void set_uniform_queue_all(uniform_update_queue& chassis, uniform_update_queue& left_wheel, uniform_update_queue& right_wheel);
	void set_uniform_queue_all(uniform_update_queue& all);

	void set_uniform_queue_chassis(uniform_update_queue&& chassis);
	void set_uniform_queue_left_wheel(uniform_update_queue&& left_wheel);
	void set_uniform_queue_right_wheel(uniform_update_queue&& right_wheel);
	void set_uniform_queue_all(uniform_update_queue&& chassis, uniform_update_queue&& left_wheel, uniform_update_queue&& right_wheel);
	void set_uniform_queue_all(uniform_update_queue&& all);

	void set_default_uniform_queue_chassis(Scene& scene);
	void set_default_uniform_queue_left_wheel(Scene& scene);
	void set_default_uniform_queue_right_wheel(Scene& scene);
	void set_default_uniform_queue_all(Scene& scene);

	void sync_with_physics();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);
	void set_rotation(physx::PxQuat rotation);
};
