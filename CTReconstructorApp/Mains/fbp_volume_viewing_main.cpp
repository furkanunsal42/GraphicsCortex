#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"

int main() {
	
	int file_width = 2048;
	glm::ivec2 volume_dimentions(1440, 1440);

	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor", 0, 2, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	int floating_point_precision = 16;
	std::vector<std::pair<std::string, std::string>> fbp_shader_defines_to_use =
		floating_point_precision == 16 ? _fbp_shader_defines_f16 : _fbp_shader_defines_f32;

	std::vector<std::pair<std::string, std::string>> ffft_shader_defines_to_use =
		floating_point_precision == 16 ? _ffft_shader_defines_f16 : _ffft_shader_defines_f32;

	Texture2D::ColorTextureFormat fbp_volume_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
	Texture2D::ColorTextureFormat fbp_projection_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
	Texture2D::ColorTextureFormat fbp_projection_complex_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::RG16F : Texture2D::ColorTextureFormat::RG32F;

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(fbp_shader_defines_to_use, ffft_shader_defines_to_use);

	solver->set_volume_format(fbp_volume_format_to_use);
	solver->set_projection_format(fbp_projection_format_to_use);
	solver->set_projection_complex_format(fbp_projection_complex_format_to_use);

	solver->set_display_progress(true);

	//solver->generate_shepplogan(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);
	//solver->project_forward_cone_to_projections(730.87f, 669.04f, 409.60f, 213.84f, 1, projection_count, 0);
	//solver->project_forward_cone_to_sinograms(730.87f, 669.04f, 409.60f, 213.84f, 1, projection_count, 0);
	//solver->read_projections("C:/Users/FurkanPC/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, 1440);
	
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y, volume_dimentions.x));
	//solver->read_volume("C:/Users/FURKAN.UNSAL/Desktop/Projektionen", 2048, 512, 1, 2, volume_dimentions.x);
	solver->read_volume("C:/Users/furkan.unsal/Desktop/raw_data/slices", file_width, volume_dimentions.y, 1, 2, volume_dimentions.x);

	solver->set_volume_active(glm::ivec3(0, 0, 0), glm::ivec3(solver->get_segment_count().x, solver->get_segment_count().y, solver->get_segment_count().z), true);

	//solver->project_forward_parallel(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.x, solver->get_volume_format(), 1, 0, 0);
	slice->is_bindless = false;
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.x, solver->get_projection_complex_format(), 1, 0, 0);
	slice_complex->is_bindless = false;
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.x, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	slice_white->is_bindless = false;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	int i = 0;
	frame.set_visibility(true);
	while (frame.is_running()) {
		double delta_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		i++;

		i = i % volume_dimentions.y;
		solver->load_volume_slice_y(i, *slice);
		//i = i % projection_count;
		//solver->load_projection(i, *slice);
		//i = i % volume_dimentions.y;
		//solver->load_sinogram(i, *slice);

		solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

		framebuffer->attach_color(0, slice_white);
		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.x, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}