#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

using namespace shader_directory;

int main() {

	glm::ivec3 volume_dimentions(1472, 1472, 1472);
	glm::vec3 voxel_size(200.0f / volume_dimentions.x, 200.0f / volume_dimentions.y, 200.0f / volume_dimentions.z);
	int projection_count = 1440;
	//int window_width = 1024;

	int slice_index_number_length = 6;
	std::string reconstruction_path = "reconstruction";

	Frame frame(volume_dimentions.x, volume_dimentions.z, "CTReconstructor", 0, 2, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
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

	fbp_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FBP/";
	ffft_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FFT/";
	util_shader_directory = "../CTReconstructor/Source/GLSL/Compute/Util/";

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(fbp_shader_defines_to_use, ffft_shader_defines_to_use);
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z));
	solver->set_projections_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y, projection_count));

	solver->set_volume_format(fbp_volume_format_to_use);
	solver->set_projection_format(fbp_projection_format_to_use);
	solver->set_projection_complex_format(fbp_projection_complex_format_to_use);

	solver->set_display_progress(true, 8);

	solver->read_projections("C:/Users/furkan.unsal/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, projection_count);

	solver->log_normalize_projections(95.0 / 255);

	solver->apply_filter_to_projections(FBP2D::FilterType::SHEPP_LOGAN);

	solver->generate_blank_volume(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);
	solver->project_backward_cone_fdk_from_projections_matrix(730.87f, 669.04f, 409.60f, 213.84f, 209.196216, 1, volume_dimentions.x, volume_dimentions.y, 0);

	//solver->project_backward_cone_fdk_from_projections(730.87f, 669.04f, 409.60f, 213.84f, 209.196216, 1, volume_dimentions.x, volume_dimentions.y, 0);
	//solver->project_backward_cone_fdk_from_projections(606.93, 1036.15, 409.60f, 200, 200, 1, volume_dimentions.x, volume_dimentions.y, 0);
	//solver->project_backward_cone_fdk_from_projections(582.017, 1036.15, 409.60f, 200, 200, 1, volume_dimentions.x, volume_dimentions.y, 0);

	solver->normalize_histogram();

	//solver->project_forward_parallel_to_projections(606.93, 1036.15, 409.60f, 400, 400, 1, volume_dimentions.x, volume_dimentions.y, 0);
	//solver->project_forward_parallel_to_projections(1, projection_count, 0);

	solver->mirror_along_x();

	solver->write_volume(reconstruction_path + "/" + compute_filename(reconstruction_path));

	ReconstructionInfo info;
	info.name = compute_filename(reconstruction_path);
	info.volume_resolution = volume_dimentions;
	info.voxel_size_mm = voxel_size;
	info.save_to_disc(reconstruction_path);

	//solver->generate_blank_volume(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);
	//for (int horizontal_layer = 0; horizontal_layer < solver->get_segment_count().y; horizontal_layer++) {
	//	solver->set_volume_active(glm::ivec3(0, 0, 0), glm::ivec3(solver->get_segment_count().x, solver->get_segment_count().y, solver->get_segment_count().z), false);
	//	solver->set_volume_active(glm::ivec3(0, horizontal_layer, 0), glm::ivec3(solver->get_segment_count().x, horizontal_layer+1, solver->get_segment_count().z), true);
	//
	//	solver->project_backward_cone_fdk_from_projections(730.87f, 669.04f, 409.60f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);
	//	solver->normalize_histogram();
	//	
	//	solver->write_volume(reconstruction_path);
	//
	//	solver->free_active_volume_segments();
	//}
	//solver->set_volume_active(glm::ivec3(0, 0, 0), glm::ivec3(solver->get_segment_count().x, solver->get_segment_count().y, solver->get_segment_count().z), true);

	//solver->project_forward_parallel(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_volume_format(), 1, 0, 0);
	slice->is_bindless = false;
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_projection_complex_format(), 1, 0, 0);
	slice_complex->is_bindless = false;
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
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
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.z, 0, 0, volume_dimentions.x, volume_dimentions.z, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}