#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "ECS/CortexScene.h"
#include "ECS/SystemPipeline.h"

class ECSTest : public TestBench {
public:

	struct ComponentA {
		float a = 12;
	};

	struct ComponentB {
		int32_t b = 2;
	};

	class PrintASystem : public ISystem {
		
		void update(SystemContext& ctx) override {
			auto view = ctx.scene.view<ComponentA>();

			for (auto e : view) {
				auto& comp_a = ctx.scene.get<ComponentA>(e);
				std::cout << comp_a.a << ", " << std::endl;
			}
		}
	};

	class PrintABSystem : public ISystem {

		void update(SystemContext& ctx) override {
			auto view = ctx.scene.view<ComponentA, ComponentB>();

			for (auto e : view) {
				auto& comp_a = ctx.scene.get<ComponentA>(e);
				auto& comp_b = ctx.scene.get<ComponentB>(e);
				std::cout << "[" << comp_a.a << ", " << comp_b.b << "] " << std::endl;
			}
		}
	};

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(0, 0);
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

		
		SystemPipeline system_pipeline;
		system_pipeline.add_system<PrintABSystem>();
		system_pipeline.add_system<PrintASystem>();

		CortexScene scene;

		std::vector<Entity2> entities;
		entities.push_back(scene.create_entity());
		entities.push_back(scene.create_entity());
		entities.push_back(scene.create_entity());
		entities.push_back(scene.create_entity());

		scene.add(entities[0], ComponentA{ .a = 32.5 });
		scene.add(entities[0], ComponentB{ .b = 1 });

		scene.add(entities[1], ComponentA{ .a = 42.5 });

		scene.add(entities[2], ComponentB{ .b = 2 });

		SystemContext context{
			.scene = scene,
			.delta_time = 0
		};
		system_pipeline.execute_all(context);

		std::cin.get();
		return true;
	}
};