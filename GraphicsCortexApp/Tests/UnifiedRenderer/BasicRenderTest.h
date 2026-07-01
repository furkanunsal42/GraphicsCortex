#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#include "AdvancedRendering/DefaultRenderPasses.h"

class BasicRenderTest : public TestBench {
public:

	class Renderer_FlatColor {
	public:

		URenderPass_FlatColor	flatcolor;
		URenderPass_Presnet		present;

		void setup() {

			flatcolor.init(glm::ivec2(1920, 1080));

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

		UnifiedRenderer&	renderer = UnifiedRenderer::get();
		Renderer_FlatColor	flatcolor;

		std::vector<float> vertices{
			0, 0, 0,	1, 0, 0, 1,
			1, 0, 0,	0, 1, 0, 1,
			0, 1, 0,	0, 0, 1, 1,
			1, 1, 0,	1, 0, 1, 1,
		};

		std::vector<uint32_t> indices{
			0, 1, 3,
			0, 3, 2,
		};

		uint32_t mesh = renderer.create_mesh();
		renderer.get_mesh(mesh).allocate_vertices(vertices.size() * sizeof(float));
		renderer.get_mesh(mesh).allocate_indices(indices.size() * sizeof(uint32_t));

		renderer.get_mesh(mesh).load_vertices(vertices.data(), vertices.size() * sizeof(float));
		renderer.get_mesh(mesh).load_indices(indices.data(), indices.size() * sizeof(uint32_t));

		renderer.get_mesh(mesh).set_attribute(0, UnifiedRenderer::f32_3, 0);
		renderer.get_mesh(mesh).set_attribute(1, UnifiedRenderer::f32_4, sizeof(glm::vec3));

		renderer.get_mesh(mesh).set_vertex_stride(sizeof(glm::vec3) + sizeof(glm::vec4));

		std::shared_ptr<Texture2D> green_texture = std::make_shared<Texture2D>(
			512, 512, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0
		);
		green_texture->is_bindless = true;
		green_texture->clear(glm::vec4(0, 1, 0, 1));

		uint32_t red = renderer.create_texture(green_texture);

		uint32_t material = renderer.create_material();
		renderer.get_material(material).push_back(red);

		uint32_t layer	= renderer.create_render_layer();
		flatcolor.flatcolor.render_pass().set_render_layer(layer);

		uint32_t object = renderer.create_object(mesh, glm::mat4(1));

		renderer.get_render_layer(layer).object_add(object, material);

		flatcolor.setup();

		Camera camera;
		camera.fov = 70;
		camera.max_distance  = 200.0f;
		camera.screen_width	 = 1920;
		camera.screen_height = 1080;

		while (!default_window->should_close()) {

			double deltatime = default_window->handle_events(true);
			camera.handle_movements((GLFWwindow*)default_window->get_handle(), deltatime);
			camera.update_matrixes();
			
			flatcolor.render(camera);

			default_window->swap_buffers();
		}

		std::cin.get();
		return true;
	}
};