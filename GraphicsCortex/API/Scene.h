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

#include "SharedPtr_Lights.h"
#include "SharedPtr_Object.h"
#include "SharedPtr_Camera.h"

class Text;
class CubeMapTexture;

class Scene {
public:

	Scene(const Frame& frame);

	Camera_s camera;
	std::string model_uniform_name = "model";
	std::string view_uniform_name = "view";
	std::string projection_uniform_name = "projection";
	glm::vec4 background_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	template<typename T>
	std::enable_if_t<std::is_same<T, AmbiantLight_s>::value || std::is_same<T, DirectionalLight_s>::value || std::is_same<T, PointLight_s>::value || std::is_same<T, SpotLight_s>::value, void> 
		add_light(T light)
	{
		_lights.push_back(light.obj);
	}

	
	void add_graphic(Graphic_s graphic);
	void add_object(Object_s object);
	void add_object(Vehicle_s vehicle);
	void add_text(std::shared_ptr<Text> text);
	void set_skybox(std::shared_ptr<CubeMapTexture> cubemap);

	template<typename T>
	std::enable_if_t<std::is_same<T, PhysicsObject>::value || std::is_same<T, PhysicsVehicle>::value || std::is_same<T, physx::PxRigidActor*>::value, void>
		add_physics(T& object) {
		PhysicsScene::get().add_actor(object);
	}

	void render(bool show_warnings = true);
	void render_to_framebuffer(FrameBuffer& framebuffer, Frame& frame, bool show_warnings = true);

	void sync_with_physics();

	std::vector<std::shared_ptr<Graphic>> _graphics;
	std::vector<std::shared_ptr<Light>> _lights;
	
	std::vector<std::shared_ptr<Object>> _objects;
	std::vector<std::shared_ptr<Vehicle>> _vehicles;

	std::vector<std::shared_ptr<Text>> _texts;

	std::shared_ptr<CubeMapTexture> skybox;

private:
};