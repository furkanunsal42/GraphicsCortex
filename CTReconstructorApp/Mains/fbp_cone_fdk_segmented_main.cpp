#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "FilteredBackProjection/FBP3DSegmentedMemory.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"
#include "FileParser/ParameterParser.h"

#include <limits>

using namespace shader_directory;

int main() {

	ParameterParser parser;

	parser.parse("C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt");
	std::filesystem::path source_projections_path = "C:/Users/furkan.unsal/Desktop/Projektionen";

	glm::ivec2 projections_resolution = glm::ivec2(parser.query<int32_t>(ParameterParser::input_projection_resolution));
	int32_t projection_count = parser.query<float>(ParameterParser::input_projection_count);
	int32_t projections_channel_count = 1;
	int32_t projections_byte_per_channel = 2;
	glm::vec2 volume_dimensitons = 
		glm::ivec2(
			parser.query<int32_t>(ParameterParser::volume_width),
			parser.query<int32_t>(ParameterParser::volume_height)
		);
	float xray_source_distance = parser.query<float>(ParameterParser::xray_distance);
	float detector_plane_distance = parser.query<float>(ParameterParser::detector_plane_distance);
	float detector_width = parser.query<float>(ParameterParser::detector_plane_width);

	//glm::ivec2 projections_resolution = glm::ivec2(2048);
	//int32_t projections_channel_count = 1;
	//int32_t projections_byte_per_channel = 2;
	//glm::vec2 volume_dimensitons = glm::vec2(213.84f, 213.84f);
	//float xray_source_distance = 730.87f;
	//float detector_panel_distance = 669.04f;
	//float detector_width = 409.60f;
	//int projection_count = 1440;
	
	int slice_index_number_length = 6;
	std::string reconstruction_path = "reconstruction";
	glm::ivec3 volume_resolution(1024, 1024, 1024);
	int window_width = 1024;

	fbp_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FBP/";
	ffft_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FFT/";
	util_shader_directory = "../CTReconstructor/Source/GLSL/Compute/Util/";

	WindowDescription desc;
	desc.w_resolution = glm::ivec2(window_width);
	desc.w_name = "CTReconstructor";
	desc.w_scale_framebuffer_size = false;
	desc.w_scale_window_size = false;
	desc.w_visible = false;

	Window window(desc);

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(
		FBP3D::FloatingPointPrecision::fp16,
		glm::ivec3(volume_resolution.x, volume_resolution.y / 1, projection_count),
		glm::ivec3(volume_resolution.x, volume_resolution.y / 1, volume_resolution.z)
	);

	solver->set_display_progress(true, 128);

	std::shared_ptr<Texture1D> volume_min_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	std::shared_ptr<Texture1D> volume_max_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	std::shared_ptr<Texture1D> projections_min_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	std::shared_ptr<Texture1D> projections_max_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);

	volume_min_texture->clear(std::numeric_limits<int32_t>::max());
	volume_max_texture->clear(std::numeric_limits<int32_t>::min());
	projections_min_texture->clear(std::numeric_limits<int32_t>::max());
	projections_max_texture->clear(std::numeric_limits<int32_t>::min());

	solver->read_projections(
		source_projections_path.string(),
		projections_resolution.x, projections_resolution.y,
		projections_channel_count,
		projections_byte_per_channel,
		volume_resolution.x, volume_resolution.y,
		projection_count
	);

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, false, true, [&](glm::ivec3 projection_segment_index) {
		solver->compute_min_value_of_projections(*projections_min_texture);
		solver->compute_max_value_of_projections(*projections_max_texture);
		});

	float projections_min_value;
	float projections_max_value;
	{
		int32_t sensitivity = 256 * 256 * 256;
		auto projections_min_image = projections_min_texture->get_image(Texture1D::ColorFormat::RED_INTEGER, Texture1D::Type::INT, 0);
		projections_min_value = *(int32_t*)projections_min_image->get_image_data() / (float)sensitivity;

		auto projections_max_image = projections_max_texture->get_image(Texture1D::ColorFormat::RED_INTEGER, Texture1D::Type::INT, 0);
		projections_max_value = *(int32_t*)projections_max_image->get_image_data() / (float)sensitivity;
	}

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, false, false, [&](glm::ivec3 projection_segment_index) {
		solver->log_normalize_projections(projections_max_value);
		solver->apply_fdk_weights_to_projections(xray_source_distance, detector_plane_distance, detector_width);
		solver->apply_filter_to_projections(FBP2D::FilterType::RAM_LAK);
		});

	solver->generate_blank_volume(volume_resolution.x, volume_resolution.y, volume_resolution.y);
	
	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, true, false, [&](glm::ivec3 projection_index) {
		fbp_segmented_memory::iterate_horizontal_volume_segments_per_projection(*solver, projection_index.y, [&](glm::ivec3 volume_segment_index) {
			if (solver->get_volume_segment_count() == glm::ivec3(1, 1, 1))
				solver->projections_clear_ram();
			solver->project_backward_cone_fdk_from_projections_matrix(xray_source_distance, detector_plane_distance, detector_width, volume_dimensitons.x, volume_dimensitons.y, 1, volume_resolution.x, volume_resolution.y, 0);
			});
		});
	
	solver->projections_clear_ram();
	solver->projections_clear_vram();
	
	fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver->clip_negatives_of_volume();
		solver->compute_min_value_of_volume(*volume_min_texture);
		solver->compute_max_value_of_volume(*volume_max_texture);
		});
	
	fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver->normalize_min_max_values_of_volume(*volume_min_texture, *volume_max_texture);
		});

	ReconstructionInfo info;
	info.name = compute_filename(reconstruction_path);
	info.volume_resolution = volume_resolution;
	info.voxel_size_mm = glm::vec3(volume_dimensitons.x, volume_dimensitons.y, volume_dimensitons.x) / glm::vec3(volume_resolution);

	info.save_to_disc(reconstruction_path);
	
	//solver->project_forward_parallel(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_resolution.x, volume_resolution.z, solver->get_volume_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_resolution.x, volume_resolution.z, solver->get_projection_complex_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_resolution.x, volume_resolution.z, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	window.set_window_visibility(true);

	while (true) {
		fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		
			int segment_height = solver->get_volume_max_segment_size().y;
			for (int projection_id = volume_segment_index.y * segment_height; projection_id < (volume_segment_index.y + 1) * segment_height; projection_id++) {
				if (window.should_close()) exit(0);
				double delta_time = window.handle_events(true);
				primitive_renderer::clear(0, 0, 0, 1);

				solver->load_volume_slice_y(projection_id, *slice);
				//solver->load_projection(slice_id, *slice);
				//solver->load_sinogram(slice_id, *slice);

				solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

				framebuffer->attach_color(0, slice_white);
				framebuffer->set_read_buffer(0);
				framebuffer->blit_to_screen(0, 0, volume_resolution.x, volume_resolution.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

				window.swap_buffers();
				std::this_thread::sleep_for(std::chrono::milliseconds(8));
			}
			});
	}
}