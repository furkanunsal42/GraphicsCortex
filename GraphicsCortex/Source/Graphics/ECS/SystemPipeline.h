#pragma once
#include "CortexScene.h"

struct SystemContext {
    CortexScene&    scene;
    float           delta_time;
};

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void init(CortexScene& scene)       {;}
    virtual void release(CortexScene& scene)    {;}
    virtual void update(SystemContext& ctx)     = 0;
};

class SystemPipeline {
public:
    
    //~SystemPipeline();

    template<typename T, typename... Args>
    void add_system(Args&&... args);
    
    //   remove_system<T>();

    void execute_all(SystemContext& ctx);

private:
    std::vector<std::unique_ptr<ISystem>> systems;
};

#include "SystemPipeline_Templated.h"