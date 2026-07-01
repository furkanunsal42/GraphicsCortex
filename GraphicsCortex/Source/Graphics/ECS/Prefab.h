#pragma once
#include <vector>
#include <functional>
#include <cstdint>
#include "Entity.h"

// Forward declaration
class CortexScene;

class Prefab {
public:
    
    struct EntityBlueprint {
        int32_t parent_index = invalid_index;
        template<typename T>
        void add(const T& component);
    private:
        std::vector<std::function<void(CortexScene&, Entity2)>> copiers;
        friend class Prefab;
        friend class CortexScene;
    };

    int32_t create_entity(int32_t parent_index = invalid_index);
    EntityBlueprint& get_entity(int32_t index);
    
    size_t get_entity_count() const;

    constexpr static uint32_t invalid_index = -1;

private:
    std::vector<EntityBlueprint> blueprints;
    friend class CortexScene; 
};

#include "Prefab_Templated.h"