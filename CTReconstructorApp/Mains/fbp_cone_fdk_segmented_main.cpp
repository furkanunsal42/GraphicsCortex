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
	int window_width = 1024;

	int slice_index_number_length = 6;
	std::string reconstruction_path = "reconstruction";

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

	fbp_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FBP/";
	ffft_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FFT/";
	util_shader_directory = "../CTReconstructor/Source/GLSL/Compute/Util/";

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(fbp_shader_defines_to_use, ffft_shader_defines_to_use);
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, 256/*volume_dimentions.y*/, volume_dimentions.z));
	solver->set_projections_max_segment_size(glm::ivec3(volume_dimentions.x, 256/*volume_dimentions.y*/, projection_count));

	solver->set_volume_format(fbp_volume_format_to_use);
	solver->set_projection_format(fbp_projection_format_to_use);
	solver->set_projection_complex_format(fbp_projection_complex_format_to_use);

	solver->set_display_progress(true, 8);

	solver->read_projections("C:/Users/furkan.unsal/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, projection_count);

	for (int projections_index = 0; projections_index < solver->get_projections_segment_count().y; projections_index++) {
		solver->set_projections_active_all(false);
		solver->set_projections_active_layer_y(projections_index, 1, true);
	
		solver->log_normalize_projections(95.0 / 255);
		solver->apply_fdk_weights_to_projections(730.87f, 669.04f, 409.60f);
		solver->apply_filter_to_projections(FBP2D::FilterType::SHEPP_LOGAN);
		
		solver->projections_transfer_vram_to_ram();
		solver->projections_clear_vram();
	}

	std::shared_ptr<Texture1D> min_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	min_texture->is_bindless = false;
	std::shared_ptr<Texture1D> max_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	max_texture->is_bindless = false;

	solver->generate_blank_volume(volume_dimentions.x, volume_dimentions.y, volume_dimentions.y);
	for (int projections_index = 0; projections_index < solver->get_projections_segment_count().y; projections_index++) {
		solver->set_projections_active_all(false);
		solver->set_projections_active_layer_y(projections_index, 1, true);
		
		solver->projections_transfer_ram_to_vram();
	
		for (int horizontal_layer = 0; horizontal_layer < solver->get_volume_segment_count().y; horizontal_layer++) {
			if (horizontal_layer * solver->get_volume_max_segment_size().y  >= (projections_index + 1) * solver->get_projections_max_segment_size().y) continue;
			if ((horizontal_layer + 1) * solver->get_volume_max_segment_size().y < projections_index * solver->get_projections_max_segment_size().y) continue;

			solver->set_volume_active_all(false);
			solver->set_volume_active_layer_y(horizontal_layer, 1, true);
			
			solver->volume_transfer_ram_to_vram();

			solver->project_backward_cone_fdk_from_projections(730.87f, 669.04f, 409.60f, 213.84f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);
		
			solver->compute_min_value_of_volume(*min_texture);
			solver->compute_max_value_of_volume(*max_texture);
	
			solver->volume_transfer_vram_to_ram();
			solver->volume_clear_vram();
		}
	
		solver->projections_clear_vram();
		solver->projections_clear_ram();
	}
	
	for (int horizontal_layer = 0; horizontal_layer < solver->get_volume_segment_count().y; horizontal_layer++) {
		solver->set_volume_active_all(false);
		solver->set_volume_active_layer_y(horizontal_layer, 1, true);
		solver->volume_transfer_ram_to_vram();
	
		solver->normalize_histogram(*min_texture, *max_texture);
		solver->mirror_along_x();

		solver->volume_transfer_vram_to_ram();
		solver->volume_clear_vram();
	}
	solver->set_volume_active_all(true);
	solver->set_projections_active_all(true);

	solver->volume_transfer_ram_to_vram();
	solver->volume_clear_ram();
	
	ReconstructionInfo info;
	info.name = compute_filename(reconstruction_path);
	info.volume_resolution = volume_dimentions;
	info.voxel_size_mm = voxel_size;
	info.save_to_disc(reconstruction_path);

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
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}