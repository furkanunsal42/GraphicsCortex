#pragma once
#include <cinttypes>
#include <vector>
#include "Entity.h"

class CortexScene;

class ISparseSet {  // enables storing multiple SparseSets in same container
public:
    virtual ~ISparseSet()           = default;
    virtual void remove(Entity2 e)  = 0;
    virtual void clear()            = 0;

protected:
    static uint32_t get_entity_index(Entity2 e);
    static uint32_t get_entity_generation(Entity2 e);
};

template<typename T>
class SparseSet : public ISparseSet {
public:
    
    void    add(Entity2 e, T component);
    void    remove(Entity2 e) override;

    T&      get(Entity2 e);
    bool    has(Entity2 e) const;
    
    void    clear() override;

private:

    std::vector<uint32_t>   sparse;
    std::vector<T>          dense_components;
    std::vector<Entity2>    dense_entities;

    static constexpr uint32_t invalid_index = 0xffffffff;
    friend CortexScene;
};

#include "SparseSet_Templated.h"
