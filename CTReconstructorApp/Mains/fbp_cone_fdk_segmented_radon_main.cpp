#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

using namespace shader_directory;

int main() {

	glm::ivec3 volume_dimentions(1440, 1440, 1440);
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
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, 128/*volume_dimentions.y*/, volume_dimentions.z));
	solver->set_projections_max_segment_size(glm::ivec3(volume_dimentions.x, 128/*volume_dimentions.y*/, projection_count));

	solver->set_volume_format(fbp_volume_format_to_use);
	solver->set_projection_format(fbp_projection_format_to_use);
	solver->set_projection_complex_format(fbp_projection_complex_format_to_use);

	solver->set_display_progress(true, 8);

	solver->generate_blank_projections(volume_dimentions.x, volume_dimentions.y, projection_count);
	solver->generate_blank_volume(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);
	for (int volume_segment_index = 0; volume_segment_index < solver->get_volume_segment_count().y; volume_segment_index++) {
		solver->set_volume_active_all(false);
		solver->set_volume_active_layer_y(volume_segment_index, 1, true);

		solver->volume_transfer_ram_to_vram();

		solver->generate_shepplogan();
		
		for (int projection_segment_index = 0; projection_segment_index < solver->get_projections_segment_count().y; projection_segment_index++) {
			solver->set_projections_active_all(false);
			solver->set_projections_active_layer_y(projection_segment_index, 1, true);

			FBP3D::AABB2D bounding_box = solver->get_aabb_conebeam(
				FBP3D::AABB3D(solver->get_volume_max_segment_size() * glm::ivec3(1, volume_segment_index, 1), 
							  solver->get_volume_max_segment_size() * glm::ivec3(1, volume_segment_index + 1, 1)),
				0, volume_dimentions, projection_count, 730.87f, 669.04f, 409.60f, 213.84f, 213.84, 1.0f, 0.0f);
			
			if (bounding_box.min.y < (solver->get_projections_max_segment_size() * glm::ivec3(1, projection_segment_index, 1)).y) continue;
			if (bounding_box.min.y >= (solver->get_projections_max_segment_size() * glm::ivec3(1, projection_segment_index + 1, 1)).y) continue;

			solver->projections_transfer_ram_to_vram();

			solver->project_forward_cone_to_projections(730.87f, 669.04f, 409.60f, 213.84f, 1.0f, projection_count, 0);

			solver->projections_transfer_vram_to_ram();
			solver->projections_clear_vram();
		}

		solver->volume_transfer_vram_to_ram();
		solver->volume_clear_vram();
	}

	solver->set_projections_active_all(true);
	solver->projections_transfer_ram_to_vram();

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.y, solver->get_volume_format(), 1, 0, 0);
	slice->is_bindless = false;
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.y, solver->get_projection_complex_format(), 1, 0, 0);
	slice_complex->is_bindless = false;
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.y, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	slice_white->is_bindless = false;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	int i = 0;
	frame.set_visibility(true);
	while (frame.is_running()) {
		double delta_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		i++;

		//i = i % volume_dimentions.y;
		//solver->load_volume_slice_y(i, *slice);
		i = i % projection_count;
		solver->load_projection(i, *slice);
		//i = i % volume_dimentions.y;
		//solver->load_sinogram(i, *slice);

		solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

		framebuffer->attach_color(0, slice_white);
		framebuffer->set_read_buffer(0);
		framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.y, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}