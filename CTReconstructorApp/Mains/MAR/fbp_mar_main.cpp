#include "CTReconstructor.h"

void launch_distance_projections_window(FBP3D& solver, FBP3D::ReconstructionParameters& parameters) {
	std::shared_ptr<Texture2D> slice			= std::make_shared<Texture2D>(parameters.output_resolution.x, parameters.output_resolution.z, solver.get_volume_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_complex	= std::make_shared<Texture2D>(parameters.output_resolution.x, parameters.output_resolution.z, solver.get_projection_complex_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_white		= std::make_shared<Texture2D>(parameters.output_resolution.x, parameters.output_resolution.z, Texture2D::ColorTextureFormat::RGBA32F, 1, 0, 0);
	std::shared_ptr<Texture2D> slice_distanced	= std::make_shared<Texture2D>(parameters.output_resolution.x, parameters.output_resolution.z, solver.get_distanced_projection_format(), 1, 0, 0);

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	//int32_t material_index = 0;
	//ct_reconstructor::context->newsletters->on_key_events.subscribe([&](const Window::KeyPressResult& event) {
	//	if (event.key == Window::Key::NUM_0 && event.action == Window::PressAction::PRESS)
	//		material_index = 0;
	//	if (event.key == Window::Key::NUM_1 && event.action == Window::PressAction::PRESS)
	//		material_index = 1;
	//	if (event.key == Window::Key::NUM_2 && event.action == Window::PressAction::PRESS)
	//		material_index = 2;
	//	if (event.key == Window::Key::NUM_3 && event.action == Window::PressAction::PRESS)
	//		material_index = 3;
	//
	//	std::cout << material_index << std::endl;
	//	});

	ct_reconstructor::context->set_window_visibility(true);
	while (true) {
		fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {

			int segment_height = solver.get_volume_max_segment_size().y;
			for (int slice_id = volume_segment_index.y * segment_height; slice_id < (volume_segment_index.y + 1) * segment_height; slice_id++) {
				if (ct_reconstructor::context->should_close()) {
					ct_reconstructor::release();
					exit(0);
				}
				double delta_time = ct_reconstructor::context->handle_events(true);
				primitive_renderer::clear();

				//solver.load_volume_slice_y(slice_id, *slice);
				//solver.load_projection(slice_id, *slice);
				//solver.load_sinogram(slice_id, *slice);
				solver.load_distanced_projection(slice_id, 1, *slice_distanced);

				//solver.fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);
				//framebuffer->attach_color(0, slice_white);
				framebuffer->attach_color(0, slice_distanced);

				framebuffer->set_read_buffer(0);
				framebuffer->blit_to_screen(0, 0, parameters.output_resolution.x, parameters.output_resolution.z, 0, 0, ct_reconstructor::context->get_window_resolution().x, ct_reconstructor::context->get_window_resolution().y, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);

				ct_reconstructor::context->swap_buffers();
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			});
	}
}

void launch_distance_atteunation_graph_points_window(FBP3D& solver) {
	Camera camera;
	camera.fov = 90;
	camera.screen_width = ct_reconstructor::context->get_window_resolution().x;
	camera.screen_height = ct_reconstructor::context->get_window_resolution().y;

	ct_reconstructor::context->set_window_visibility(true);
	while (!ct_reconstructor::context->should_close()) {
		double deltatime = ct_reconstructor::context->handle_events(true);
		primitive_renderer::clear(1, 1, 1, 1);
		camera.handle_movements((GLFWwindow*)ct_reconstructor::context->get_handle(), deltatime);

		solver.render_distance_attenuation_graph(camera);

		ct_reconstructor::context->swap_buffers();
	}
}

int main() {
	
	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/rekonstruktion.ini";
	//std::filesystem::path projections_path			= "C:/Users/furkan.unsal/Desktop/Data2/projektion";
	//std::filesystem::path volume_path				= "C:/Users/furkan.unsal/Desktop/CTReconstruction1";

	// pen
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/rekonstruktion.ini";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/projektion";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction2";

	// sage
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/MAR";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.input_files_path = projections_path;
	parameters.output_files_path = volume_path;
	parameters.output_resolution = glm::ivec3(512);

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	solver.read_projections(parameters);
	ct_reconstructor::back_project(solver, geometry, parameters,
		transfer_inputs_from_ram_on_begin |
		apply_filter_to_projections |
		apply_log_normalization_to_projections |
		clip_negatives_of_volume
		//apply_minmax_normalization_to_volume |
	);
	
	solver.read_projections(parameters);
	solver.projections_transfer_ram_to_vram();
	solver.compute_min_value_of_projections();
	solver.log_normalize_projections();

	float metal_threshold = 30;
	float prior_image_threshold = 14;
	
	FBP3D solver_metal(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	FBP3D::ReconstructionParameters parameters_metal;
	parameters_metal.input_data_type = FBP3D::Volume;
	parameters_metal.output_data_type = FBP3D::Projections;
	parameters_metal.output_resolution = solver.get_projections_size();

	solver.move_volume(solver_metal);
	solver_metal.generate_blank_projections(parameters_metal);
	solver_metal.project_forward(geometry, glm::vec2(metal_threshold, std::numeric_limits<int16_t>::max()));
	solver_metal.move_volume(solver);

	ct_reconstructor::launch_debug_window(solver_metal, "MAR Metal Image");

	FBP3D solver_prior(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	FBP3D::ReconstructionParameters parameters_prior;
	parameters_prior.input_data_type = FBP3D::Volume;
	parameters_prior.output_data_type = FBP3D::Projections;
	parameters_prior.output_resolution = solver.get_projections_size();

	solver.move_volume(solver_prior);
	solver_prior.generate_blank_projections(parameters_prior);
	solver_prior.project_forward(geometry, glm::vec2(std::numeric_limits<int16_t>::min(), prior_image_threshold));
	solver_prior.move_volume(solver);

	ct_reconstructor::launch_debug_window(solver_prior, "MAR Prior Image");

	solver.mar_normalize_projections(solver_prior);
	solver.mar_interpolate_projections(solver_metal);
	solver.mar_denormalize_projections(solver_prior);

	ct_reconstructor::back_project(solver, geometry, parameters,
		transfer_inputs_from_ram_on_begin		|
		apply_filter_to_projections				|
		apply_log_normalization_to_projections	|
		clip_negatives_of_volume				|
		apply_minmax_normalization_to_volume	|
		save_output_to_disk
	);

	ct_reconstructor::launch_debug_window(solver, "MAR Reconstruction");

	ct_reconstructor::release();
}