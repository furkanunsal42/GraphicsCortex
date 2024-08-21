#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"

int main() {

	glm::ivec3 volume_dimentions(1024, 1024, 1024);
	//int projection_count = volume_dimentions.x;
	int projection_count = 1440;
	int window_width = 1024;

	int slice_index_number_length = 6;
	std::string reconstruction_path = "reconstruction/";

	Frame frame(window_width, window_width, "CTAnalyzer", 0, 1, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>();
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z));

	//solver->generate_shepplogan(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);
	//solver->project_forward_cone_to_sinogram(730.87f, 669.04f, 409.60f, 213.84f, 1, projection_count, 0);

	//solver->apply_filter_to_sinograms(FBP2D::RAM_LAK);
	
	//solver->project_backward_cone_fdk_from_sinograms(730.87f, 669.04f, 409.60f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);

	//solver->project_forward_parallel_to_projections(1, projection_count, 0);

	//solver->project_forward_cone(730.87f, 669.04f, 409.60f, 213.84f, 1, projection_count, 0);
	//solver->read_projections_as_sinograms("C:/Users/FurkanPC/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, 1440);
	solver->read_projections_as_sinograms("C:/Users/FURKAN.UNSAL/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, 1440);
	solver->log_normalize_sinograms(95.0 / 255);
	
	//solver->apply_fdk_weights_to_sinograms(730.87f, 669.04f, 409.60f);
	solver->apply_filter_to_sinograms(FBP2D::FilterType::SHEPP_LOGAN);
	//solver->project_backward_cone_fdk_from_sinograms(730.87f, 669.04f, 409.60f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);

	//solver->project_forward_cone_to_sinogram(730.87f, 669.04f, 409.60f, 213.84f, 1, projection_count, 0);
	//solver->apply_filter_to_sinograms(FBP2D::FilterType::RAM_LAK);
	solver->project_backward_cone_fdk_from_sinograms(730.87f, 669.04f, 409.60f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);
	
	//solver->project_forward_parallel_to_sinogram(1, projection_count, 0);
	//solver->apply_filter_to_sinograms(FBP2D::FilterType::RAM_LAK);
	//solver->project_backward_parallel_from_sinograms(1, volume_dimentions.x, volume_dimentions.y, 0);

	//solver->normalize_histogram();

	//
	//solver->write_volume(reconstruction_path);

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

		i = i % volume_dimentions.z;
		solver->load_volume_slice_y(i, *slice);
		//i = i % projection_count;
		//solver->load_projection(i, *slice);
		//i = i % volume_dimentions.y;
		//solver->load_sinogram(i, *slice);

		solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

		framebuffer->attach_color(0, slice_white);
		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}