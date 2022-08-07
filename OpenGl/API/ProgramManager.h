#pragma once

#include "ShaderCompiler.h"
#include "Graphic.h"
#include "Lights.h"
#include "Camera.h"

class ProgramManager {
public:
	Program* program = nullptr;
	Graphic* mesh = nullptr;
	std::vector<Light*>* lights = nullptr;
	Camera* cam;
	virtual void update_uniforms(bool init = true);

	static ProgramManager& get();
protected:
	ProgramManager() = default;
};
