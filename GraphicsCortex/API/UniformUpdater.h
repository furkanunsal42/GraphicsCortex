#pragma once

#include <functional>
#include "SharedPtr_Program.h"
#include "SharedPtr_Camera.h"
#include "SharedPtr_Graphic.h"
#include "SharedPtr_Lights.h"
#include "SharedPtr_FrameBuffer.h"

#define UniformFunction_Graphic_s Graphic_s graphic, Program_s program, Camera_s camera, int object_index
#define UniformFunction_DirectionalLight_s DirectionalLight_s directional_light, Program_s program, Camera_s camera, int object_index
#define UniformFunction_AmbiantLight_s AmbiantLight_s ambiant_light, Program_s program, Camera_s camera, int object_index
#define UniformFunction_PointLight_s PointLight_s point_light, Program_s program, Camera_s camera, int object_index
#define UniformFunction_SpotLight_s SpotLight_s spot_light, Program_s program, Camera_s camera, int object_index

template<typename T>
class UniformUpdater {
public:

	UniformUpdater(std::function<void(T, Program_s, Camera_s, int)> update_call = [](T, Program_s, Camera_s, int) {}) {
		_uniform_update_call = update_call;
	}

	void operator()(T object, Program_s program, Camera_s camera, int object_index) {
		_uniform_update_call(object, program, camera, object_index);
	}

	void operator()(const std::vector<T>& objects, Program_s program, Camera_s camera, int object_index) {
		for (auto& object : objects)
			_uniform_update_call(object, program, camera, object_index);
	}

	void set_uniform_call(std::function<void(T, Program_s, Camera_s, int)> update_call) {
		_uniform_update_call = update_call;
	}

	void operator=(std::function<void(T, Program_s, Camera_s, int)> update_call) {
		set_uniform_call(update_call);
	}

private:
	std::function<void(T, Program_s, Camera_s, int)> _uniform_update_call;

};
