#pragma once
#include "SystemPipeline.h"

template<typename T, typename... Args>
void SystemPipeline::add_system(Args&&... args) {
    systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
}
