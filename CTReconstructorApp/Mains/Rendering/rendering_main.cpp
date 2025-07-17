#include "CTReconstructor.h"


int main() {
	
	ct_reconstructor::init();

	FBP3D solver;

	std::shared_ptr<Program> volumetric_program = std::make_shared<Program>(
		Shader("../GraphicsCortex/Source/GLSL/Surface/surface.vert",
			"../GraphicsCortex/Source/GLSL/Surface/surface.geom",
			"../CTReconstructor/Source/GLSL/VolumetricRendering/volumetric.frag"));

	volumetric_program->update_uniform("volume", *solver.volume[0][0][0].get_texture());

	SingleModel box_model;
	std::shared_ptr<Mesh> mesh;

	Camera camera;
	camera.fov = 90;
	camera.screen_width = ct_reconstructor::context->get_window_resolution().x;
	camera.screen_height = ct_reconstructor::context->get_window_resolution().y;

	while (!ct_reconstructor::context->should_close()) {
		double deltatime = ct_reconstructor::context->handle_events(true);
		primitive_renderer::clear(0, 0, 0, 0);

		camera.handle_movements((GLFWwindow*)ct_reconstructor::context->get_handle(), deltatime);

		RenderParameters params(true);
		params.cull_face = true;
		params.cull_face_direction = RenderParameters::CullDirection::FRONT;
		primitive_renderer::render(
			
		);
	}

	ct_reconstructor::release();
}