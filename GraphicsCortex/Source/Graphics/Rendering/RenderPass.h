#pragma once
#include "Scene.h"

class RenderPass {

	virtual void on_render(Scene& scene) = 0;
};
