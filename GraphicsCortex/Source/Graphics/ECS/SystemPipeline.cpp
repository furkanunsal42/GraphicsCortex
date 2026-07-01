#include "SystemPipeline.h"

void SystemPipeline::execute_all(SystemContext& ctx) {
    for (auto& system : systems) {
        system->update(ctx);
    }
}