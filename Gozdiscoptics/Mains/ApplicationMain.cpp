
#include "GraphicsCortex.h"
#include "CurvedSurface/CurvedSurface.h"
#include "Tools/ImmidiateRendering/ImmidiateRenderer.h"

int main(){

	WindowDescription desc;
	desc.w_resolution = glm::ivec2(1024, 1024);
	desc.f_multisample_count = 16;
	desc.w_name = "Gozdiscoptics";
	Window window(desc);

	CurvedSurface surface;
	surface.define_surface("-x*x + y*y");


	//surface.set_rotation(glm::quat(glm::vec3(glm::pi<float>() / 2, 0, 0)));

	ImmidiateRenderer immidiate(1024 * 32);

	immidiate.set_line_color(glm::vec4(1, 0, 0, 1));

	glm::ivec2 ray_count(8, 8);

	//for (int32_t y = 0; y < ray_count.y; y++) {
	//	for (int32_t x = 0; x < ray_count.x; x++) {
	//		immidiate.draw_line(
	//			glm::vec3((float)x / ray_count.x - 0.5, (float)y / ray_count.y - 0.5, -0.3),
	//			glm::vec3((float)x / ray_count.x - 0.5, (float)y / ray_count.y - 0.5, -1)
	//		);
	//	}
	//}

	Camera camera;

	while (!window.should_close()) {
		double deltatime = window.handle_events();
		primitive_renderer::clear(32 / 255.0, 32 / 255.0, 32 / 255.0, 1);

		camera.handle_movements((GLFWwindow*)window.get_handle(), deltatime);

		surface.render(camera);
		immidiate.render_without_clear(camera);

		window.swap_buffers();
	}
}