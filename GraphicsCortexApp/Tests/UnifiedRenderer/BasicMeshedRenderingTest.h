#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "ECS/SystemPipeline.h"
#include "ECS/CortexScene.h"

#include "AdvancedRendering/DefaultRenderPasses.h"
#include "ECS_Systems/RenderingSystem.h"
#include "ECS_Systems/TransformHierarchySystem.h"

#include <functional>

class BasicMeshedRenderingTest : public TestBench {
public:

	class Renderer_FlatColor {
	public:

		uint32_t flat_layer = UnifiedRenderer::invalid_id;

		RendererSyncSystem::LayerResolver layer_sorter = [this](Entity2 e, CortexScene& scene) -> uint32_t {
			return this->flat_layer;
			};

		URenderPass_FlatColor	flatcolor;
		URenderPass_Presnet		present;

		void setup() {

			if (flat_layer == UnifiedRenderer::invalid_id)
				flat_layer = UnifiedRenderer::get().create_render_layer();

			flatcolor.init(glm::ivec2(1920, 1080));
			flatcolor.render_pass().set_render_layer(flat_layer);

			present.in_color = flatcolor.out_color;

		}

		void render(Camera& camera) {
			present.draw_begin();
			flatcolor.render(camera);
			present.draw_end();
		}

	};

	bool run() {

		default_window_desc = new WindowDescription();
		default_window_desc->w_resolution = glm::ivec2(1920, 1080);
		default_window_desc->f_swap_interval = 0;
		default_window_desc->w_scale_framebuffer_size = false;
		default_window_desc->w_scale_window_size = false;

		default_init();

		Renderer_FlatColor	renderer;
		CortexScene			scene;
		SystemPipeline		systems;
		
		renderer.setup();

		systems.add_system<TransformHierarchySystem>(scene);
		systems.add_system<RendererSyncSystem>(scene, renderer.layer_sorter);

		auto rect = scene.create_entity();
		scene.add(rect, TransformComponent2());
		scene.add(rect, MaterialComponent2());
		scene.add(rect, MeshComponent2());
		scene.add(rect, RendererComponent2());

		Camera camera;
		camera.fov = 70;
		camera.max_distance  = 200.0f;
		camera.screen_width	 = 1920;
		camera.screen_height = 1080;

		while (!default_window->should_close()) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();
			
			SystemContext ctx {
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