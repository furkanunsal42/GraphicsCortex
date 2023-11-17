#pragma once

#include <functional>
#include <memory>

#include "ShaderCompiler.h"
#include "Camera.h"
#include "Graphic.h"
#include "Lights.h"
#include "FrameBuffer.h"

#define UniformFunction_Graphic std::shared_ptr<Graphic> graphic, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index
#define UniformFunction_DirectionalLight std::shared_ptr<DirectionalLight> directional_light, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index
#define UniformFunction_AmbiantLight std::shared_ptr<AmbiantLight> ambiant_light, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index
#define UniformFunction_PointLight std::shared_ptr<PointLight> point_light, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index
#define UniformFunction_SpotLight std::shared_ptr<SpotLight> spot_light, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index

template<typename T>
class UniformUpdater {
public:

	UniformUpdater(std::function<void(T, std::shared_ptr<Program>, std::shared_ptr<Camera>, int)> update_call = [](T, std::shared_ptr<Program>, std::shared_ptr<Camera>, int) {}) {
		_uniform_update_call = update_call;
	}

	void operator()(T object, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index) {
		_uniform_update_call(object, program, camera, object_index);
	}

	void operator()(const std::vector<T>& objects, std::shared_ptr<Program> program, std::shared_ptr<Camera> camera, int object_index) {
		for (auto& object : objects)
			_uniform_update_call(object, program, camera, object_index);
	}

	void set_uniform_call(std::function<void(T, std::shared_ptr<Program>, std::shared_ptr<Camera>, int)> update_call) {
		_uniform_update_call = update_call;
	}

	void operator=(std::function<void(T, std::shared_ptr<Program>, std::shared_ptr<Camera>, int)> update_call) {
		set_uniform_call(update_call);
	}

private:
	std::function<void(T, std::shared_ptr<Program>, std::shared_ptr<Camera>, int)> _uniform_update_call;

};
