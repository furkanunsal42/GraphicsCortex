#include "CTReconstructor.h"

int main() {
	
	int32_t window_width = 1024;
	WindowDescription desc;
	desc.w_resolution = glm::ivec2(window_width);
	desc.w_name = "CTReconstructor";
	desc.w_scale_framebuffer_size = false;
	desc.w_scale_window_size = false;
	desc.w_visible = false;
	
	Window window(desc);

	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/rekonstruktion.ini";
	//std::filesystem::path projections_path			= "C:/Users/furkan.unsal/Desktop/Data2/projektion";
	//std::filesystem::path volume_path				= "C:/Users/furkan.unsal/Desktop/CTReconstruction1";

	// pen
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/rekonstruktion.ini";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/projektion";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction2";

	// sage
	std::filesystem::path descriptor_file_path = "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path projections_path = "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path volume_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction3";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.volume_path = volume_path;
	parameters.projections_path = projections_path;
	parameters.volume_resolution = glm::ivec3(512);
	//parameters.projection_resolution = glm::ivec2(512);
	//parameters.projection_count = 1440;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	solver.read_projections(parameters);
	ct_reconstructor::back_project(solver, geometry, parameters, CTOption::transfer_inputs_from_ram_on_begin);

	solver.generate_blank_distanced_projections(parameters.volume_resolution.x, parameters.volume_resolution.y, parameters.volume_resolution.y/*projection_count*/, 2);
	solver.bh_project_forward_parallel_to_distanced_projections({ 2, 15, 30, 100 }, 1, parameters.volume_resolution.y/*projection_count*/, 0);

	//solver->project_forward_parallel(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(parameters.volume_resolution.x, parameters.volume_resolution.z, solver.get_volume_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(parameters.volume_resolution.x, parameters.volume_resolution.z, solver.get_projection_complex_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(parameters.volume_resolution.x, parameters.volume_resolution.z, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	std::shared_ptr<Texture2D> slice_distanced = std::make_shared<Texture2D>(parameters.volume_resolution.x, parameters.volume_resolution.z, solver.get_distanced_projection_format(), 1, 0, 0);

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	window.set_window_visibility(true);
	while (true) {
		fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {

			int segment_height = solver.get_volume_max_segment_size().y;
			for (int slice_id = volume_segment_index.y * segment_height; slice_id < (volume_segment_index.y + 1) * segment_height; slice_id++) {
				if (window.should_close()) exit(0);
				double delta_time = window.handle_events(true);
				primitive_renderer::clear();

				solver.load_volume_slice_y(slice_id, *slice);
				//solver.load_projection(slice_id, *slice);
				//solver.load_sinogram(slice_id, *slice);
				//solver.load_distanced_projection(slice_id, 0, *slice_distanced);

				//solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);

				//framebuffer->attach_color(0, slice_white);
				framebuffer->attach_color(0, slice_distanced);

				framebuffer->set_read_buffer(0);
				framebuffer->blit_to_screen(0, 0, parameters.volume_resolution.x, parameters.volume_resolution.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
				
				window.swap_buffers();
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			});
	}
}