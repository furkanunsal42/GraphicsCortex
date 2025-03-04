#include "WindowBoundGlobalResources.h"

std::unordered_map<void*, GlobalResources> context_to_global_resources;
GlobalResources* active_global_resources = nullptr;