#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "ECS/SystemPipeline.h"
#include "ECS/CortexScene.h"

#include "AdvancedRendering/DefaultRenderPasses.h"
#include "ECS_Systems/RenderingSystem.h"
#include "ECS_Systems/TransformHierarchySystem.h"

#include "AdvancedRendering/CortexRenderer.h"

#include <functional>

class DeferredRendererTest : public TestBench {
public:

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

		Renderer_Deferred	renderer;
		CortexScene			scene;
		SystemPipeline		systems;

		renderer.init(glm::ivec2(1920, 1080));

		systems.add_system<TransformHierarchySystem>(scene);
		systems.add_system<RendererSyncSystem>(scene, renderer.layer_sorter);

		//Asset	asset("../GraphicsCortex/Models/dragon_new/dragon_new.fbx");
		Asset	asset("../GraphicsCortex/Models/Sponza/scene.gltf");

		Prefab	prefab = asset.load_prefab();

		auto entity = scene.create_entity(prefab);

		Camera camera;
		camera.fov = 90;
		camera.max_distance = 200.0f;
		camera.screen_width = 1920;
		camera.screen_height = 1080;

		while (!default_window->should_close()) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();

			scene.replace<TransformComponent2>(entity).local_position += glm::vec3(10, 0, 0) * (float)deltatime;

			SystemContext ctx{
				.scene = scene,
				.delta_time = 0.16f,
			};

			systems.execute_all(ctx);
			renderer.render(camera);

			default_window->swap_buffers();
		}

		std::cin.get();
		return true;
	}
};