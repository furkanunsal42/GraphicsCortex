#include "WindowBoundGlobalResources.h"

std::unordered_map<void*, GlobalResources> context_to_global_resources;
std::unordered_map<void*, void*> shared_context_to_root_context;
GlobalResources* active_global_resources = nullptr;
Window* active_window = nullptr;