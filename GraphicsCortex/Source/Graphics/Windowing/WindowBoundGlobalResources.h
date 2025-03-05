#pragma once

#include <unordered_map>
#include <memory>

#include "Tools/EquirectangularProjector/EquirectengularProjector.h"
#include "ShaderCompiler.h";
#include "Tools/GraphicsOperation/GraphicsOperation.h";

class Window;
class GlobalResources {
public:

	std::unique_ptr<EquirectangularProjector> SkylightComponent_projector = nullptr;
	std::unique_ptr<Program> SkylightComponent_render = nullptr;
	std::unique_ptr<Program> SkylightComponent_irradiance_convolver = nullptr;
	std::unique_ptr<Program> SkylightComponent_prefiltered_convolver = nullptr;
	std::unique_ptr<GraphicsOperation> SkylightComponent_operation = nullptr;

};

extern std::unordered_map<void*, GlobalResources> context_to_global_resources;
extern GlobalResources* active_global_resources;
