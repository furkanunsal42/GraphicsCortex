#pragma once

#include "Graphic.h"
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

	std::shared_ptr<Graphic> chassis = std::make_shared<Graphic>();
	std::shared_ptr<Graphic> wheels[4] = { std::make_shared<Graphic>() , std::make_shared<Graphic>() , std::make_shared<Graphic>() , std::make_shared<Graphic>() };

	bool chassis_graphic_initialized = false;
	bool wheel_graphic_initialized = false;

	enum wheel_index {
		FRONT_LEFT = 0,
		FRONT_RIGHT,
		REAR_LEFT,
		REAR_RIGHT,
	};

	Vehicle();

	void load_mesh_chassis_graphics(std::shared_ptr<Mesh> chassis);
	void load_mesh_left_wheel_graphics(std::shared_ptr<Mesh> left_wheel);
	void load_mesh_right_wheel_graphics(std::shared_ptr<Mesh> right_wheel);
	void load_mesh_all_graphics(std::shared_ptr<Mesh> chassis, std::shared_ptr<Mesh> left_wheel, std::shared_ptr<Mesh> right_wheel);

	void load_model_chassis_physics(Model& chassis);
	void load_model_left_wheel_physics(Model& left_wheel);
	void load_model_right_wheel_physics(Model& right_wheel);
	void load_model_all_physics(Model& chassis, Model& left_wheel, Model& right_wheel);

	void load_model_chassis_physics(Model&& chassis);
	void load_model_left_wheel_physics(Model&& left_wheel);
	void load_model_right_wheel_physics(Model&& right_wheel);
	void load_model_all_physics(Model&& chassis, Model&& left_wheel, Model&& right_wheel);

	void load_material_chassis(std::shared_ptr<Material> chassis);
	void load_material_left_wheel(std::shared_ptr<Material> left_wheel);
	void load_material_right_wheel(std::shared_ptr<Material> right_wheel);
	void load_material_all(std::shared_ptr<Material> chassis, std::shared_ptr<Material> left_wheel, std::shared_ptr<Material> right_wheel);
	void load_material_all(std::shared_ptr<Material> all);

	void load_material_chassis(std::shared_ptr<UnorderedMaterial> chassis);
	void load_material_left_wheel(std::shared_ptr<UnorderedMaterial> left_wheel);
	void load_material_right_wheel(std::shared_ptr<UnorderedMaterial> right_wheel);
	void load_material_all(std::shared_ptr<UnorderedMaterial> chassis, std::shared_ptr<UnorderedMaterial> left_wheel, std::shared_ptr<UnorderedMaterial> right_wheel);
	void load_material_all(std::shared_ptr<UnorderedMaterial> all);

	void load_program_chassis(std::shared_ptr<Program> chassis);
	void load_program_left_wheel(std::shared_ptr<Program> left_wheel);
	void load_program_right_wheel(std::shared_ptr<Program> right_wheel);
	void load_program_all(std::shared_ptr<Program> chassis, std::shared_ptr<Program> left_wheel, std::shared_ptr<Program> right_wheel);
	void load_program_all(std::shared_ptr<Program> all);

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
