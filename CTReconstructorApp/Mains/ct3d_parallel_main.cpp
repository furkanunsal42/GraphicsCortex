#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"

int main() {

	glm::ivec3 volume_dimentions(512, 512, 512);
	int projection_count = 1440;
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor", 0, 1, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(_fbp_shader_defines_f32, _ffft_shader_defines_f32);
	solver->set_volume_format(Texture3D::ColorTextureFormat::R32F);
	solver->set_projection_format(Texture3D::ColorTextureFormat::R32F);
	solver->set_projection_complex_format(Texture3D::ColorTextureFormat::RG32F);

	solver->generate_shepplogan(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);

	solver->project_forward_parallel_to_sinograms(1, projection_count, 0);
	solver->apply_filter_to_sinograms(FBP2D::FilterType::COSINE);
	solver->project_backward_parallel_from_sinograms(1, volume_dimentions.x, volume_dimentions.y, 0);
	//solver->project_forward_parallel_to_projections(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_volume_format(), 1, 0, 0);
	slice->is_bindless = false;
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_projection_complex_format(), 1, 0, 0);
	slice_complex->is_bindless = false;
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	slice_white->is_bindless = false;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	int i = 0;
	while (frame.is_running()) {
		double delta_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		i++;
		i = i % volume_dimentions.y;

		solver->load_volume_slice_y(i, *slice);
		//solver->load_projection(i, *slice);
		//solver->load_sinogram(i, *slice);

		solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

		framebuffer->attach_color(0, slice_white);
		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}