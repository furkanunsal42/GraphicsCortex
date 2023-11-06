#pragma once

#include "Graphic.h"
#include "PhysicsVehicle.h"

#include "SharedPtr_Graphic.h"
class Camera;
class Scene;

// objects are structures that sync physics and graphics
class Object {
public:
	Graphic_s graphics;
	PhysicsObject& physics;

	Object(Graphic& graphics, PhysicsObject& physics);

	void sync_with_physics();
	
	void set_position(glm::vec3 position);
	void set_rotation(glm::vec3 rotation);

};


class Vehicle/* : Object */ {
public:

	PhysicsVehicle physics_representation;

	Graphic_s chassis;
	Graphic_s wheels[4];

	bool chassis_graphic_initialized = false;
	bool wheel_graphic_initialized = false;

	enum wheel_index {
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		REAR_LEFT,
		REAR_RIGHT,
	};

	Vehicle();

	void load_mesh_chassis_graphics(Mesh_s chassis);
	void load_mesh_left_wheel_graphics(Mesh_s left_wheel);
	void load_mesh_right_wheel_graphics(Mesh_s right_wheel);
	void load_mesh_all_graphics(Mesh_s chassis, Mesh_s left_wheel, Mesh_s right_wheel);

	void load_model_chassis_physics(Model& chassis);
	void load_model_left_wheel_physics(Model& left_wheel);
	void load_model_right_wheel_physics(Model& right_wheel);
	void load_model_all_physics(Model& chassis, Model& left_wheel, Model& right_wheel);

	void load_model_chassis_physics(Model&& chassis);
	void load_model_left_wheel_physics(Model&& left_wheel);
	void load_model_right_wheel_physics(Model&& right_wheel);
	void load_model_all_physics(Model&& chassis, Model&& left_wheel, Model&& right_wheel);

	void load_material_chassis(Material_s chassis);
	void load_material_left_wheel(Material_s left_wheel);
	void load_material_right_wheel(Material_s right_wheel);
	void load_material_all(Material_s chassis, Material_s left_wheel, Material_s right_wheel);
	void load_material_all(Material_s all);

	void load_material_chassis(UnorderedMaterial_s chassis);
	void load_material_left_wheel(UnorderedMaterial_s left_wheel);
	void load_material_right_wheel(UnorderedMaterial_s right_wheel);
	void load_material_all(UnorderedMaterial_s chassis, UnorderedMaterial_s left_wheel, UnorderedMaterial_s right_wheel);
	void load_material_all(UnorderedMaterial_s all);

	void load_program_chassis(Program_s chassis);
	void load_program_left_wheel(Program_s left_wheel);
	void load_program_right_wheel(Program_s right_wheel);
	void load_program_all(Program_s chassis, Program_s left_wheel, Program_s right_wheel);
	void load_program_all(Program_s all);

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

	void sync_with_physics();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);
	void set_rotation(physx::PxQuat rotation);
};
