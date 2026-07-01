#pragma once
#include <vector>
#include <memory>
#include "SparseSet.h"
#include "Entity.h"
#include "Newsletter.h"

class Prefab;

class CortexScene {
public:

    using Event = Newsletter<CortexScene&, Entity2>;

    Entity2     create_entity();
    Entity2     create_entity(Prefab& prefab);
    void        destroy_entity(Entity2 entity);
    bool        is_valid(Entity2 entity);

    template<typename T> 
    void        add(Entity2 entity, T component);
    template<typename T>
    void        remove(Entity2 entity);
    template<typename T>
    const T&    get(Entity2 entity) const;
    template<typename T>
    T&          replace(Entity2 entity);
    template<typename T> 
    T&          access(Entity2 entity);
    template<typename T> 
    bool        has(Entity2 entity);
    
    template<typename T> 
    std::vector<T>& get_dense_array();

    template<typename T> Event& on_add();
    template<typename T> Event& on_remove();
    template<typename T> Event& on_replace();

    template<typename T, typename... Other>
    class View {
    public:
        class Iterator {
        public:
            using entity_iterator = std::vector<Entity2>::iterator;
            Iterator(CortexScene* s, entity_iterator start, entity_iterator finish);
            Iterator&   operator++();
            bool        operator!=(const Iterator& other) const;
            Entity2     operator*() const;
        private:
            CortexScene*    scene;
            entity_iterator it;
            entity_iterator end;
            void advance_to_valid();
        };

        View(CortexScene* s, std::vector<Entity2>* entities);
        Iterator begin();
        Iterator end();
    private:
        CortexScene*            scene;
        std::vector<Entity2>*   dense_entities;
    };

    template<typename FirstComponent, typename... OtherComponents>
    View<FirstComponent, OtherComponents...> view();

private:

    class ComponentType {
    public:
        template<typename T> static uint32_t get_id();
    private:
        inline static uint32_t next_type_id = 0;
    };

    std::vector<uint32_t> entity_generations;
    std::vector<uint32_t> free_indices;
    std::vector<std::unique_ptr<ISparseSet>> component_pools;

    std::vector<Event> on_add_events;
    std::vector<Event> on_remove_events;
    std::vector<Event> on_replace_events;

};

#include "CortexScene_Templated.h"