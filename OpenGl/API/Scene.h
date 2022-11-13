#pragma once
#include "Config.h"

#include "Camera.h"
#include "Graphic.h"
#include "ShaderCompiler.h"
#include "Lights.h";
#include "FrameBuffer.h"
#include "Object.h"
#include "Frame.h"

#include "PhysicsScene.h"

#include <vector>
#include <type_traits>

class Scene {
public:
	Camera* camera = nullptr;
	std::string model_uniform_name = "model";
	std::string view_uniform_name = "view";
	std::string projection_uniform_name = "projection";
	glm::vec4 background_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	FrameBuffer* frame_buffer = nullptr;

	template<typename T>
	std::enable_if_t<std::is_same<T, AmbiantLight>::value || std::is_same<T, DirectionalLight>::value || std::is_same<T, PointLight>::value || std::is_same<T, SpotLight>::value, void> 
		add_light(T& light)
	{
		lights.push_back(&light);
		T::count += 1;
	}

	
	void add_graphic(Graphic& graphic);
	void add_object(Object& object);
	void add_object(Vehicle& vehicle);

	template<typename T>
	std::enable_if_t<std::is_same<T, PhysicsObject&>::value || std::is_same<T, PhysicsObject&&>::value || std::is_same<T, PhysicsVehicle&>::value || std::is_same<T, PhysicsVehicle&&>::value || std::is_same<T, physx::PxRigidActor*&>::value, void>
		add_physics(T object) {
		PhysicsScene::get().add_actor(object);
	}
	template<typename T>
	std::enable_if_t<std::is_same<T, PhysicsObject>::value || std::is_same<T, PhysicsVehicle>::value || std::is_same<T, physx::PxRigidActor*>::value, void>
		add_physics(T& object) {
		PhysicsScene::get().add_actor(object);
	}

	void render(GLFWwindow* window);
	void render_to_framebuffer(Frame& frame);

private:
	std::vector<Graphic*> meshes;
	std::vector<Light*> lights;

};