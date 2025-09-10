#include "CTReconstructor.h"

int main() {
	std::filesystem::path working_directory = std::filesystem::absolute(".");
	working_directory = "C:\\Users\\furkan.unsal\\Desktop\\drive\\20250716111737.002\\[vg-data] 20250716111737.002";

	//ct_reconstructor::init_from_package();
	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.read(working_directory));
	parser.parameters.filter_type = FBP2D::RAM_LAK;
	
	parser.parameters.input_data_type = FBP3D::Projections;
	parser.parameters.output_data_type = FBP3D::Volume;
	//parser.geometry.detector_plane_offset.x = 0;
	//parser.geometry.detector_plane_offset.y = 0;
	//parser.geometry.rotation_plane_offset_x = 0;
	//parser.geometry.source_detector_tilt_x_radian = 0;
	//parser.geometry.rotation_radian_for_each_scan = glm::pi<float>() * 2;

	FBP3D solver(parser);

	//solver.generate_blank_volume(parser.parameters);
	//solver.render_sphere_to_volume(glm::vec3(0), 0.5, 0.5, false);
	//
	//parser.parameters.input_data_type = FBP3D::Volume;
	//parser.parameters.output_data_type = FBP3D::Projections;
	//
	//ct_reconstructor::forward_project(solver, parser,
	//	0
	//);

	solver.read_projections(parser);
	solver.projections_transfer_ram_to_vram();
	solver.compute_min_value_of_projections();
	solver.compute_max_value_of_projections();
	solver.log_normalize_projections();

	ct_reconstructor::launch_debug_window(solver);

	//parser.parameters.input_data_type = FBP3D::Projections;
	//parser.parameters.output_data_type = FBP3D::Volume;

	ct_reconstructor::filter(solver, parser,
		//transfer_inputs_from_ram_on_begin |
		//apply_log_normalization_to_projections |
		apply_filter_to_projections
	);
	
	ct_reconstructor::launch_debug_window(solver);

	solver.generate_blank_volume(parser.parameters);
	if (!ct_reconstructor::launch_preview_window_back(solver, parser)) {
		ct_reconstructor::release();
		return 0;
	}

	ct_reconstructor::back_project(solver, parser,
		0
		//apply_minmax_normalization_to_volume |
		//clip_negatives_of_volume |
		//save_output_to_disk
	);
	fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver.render_cylinder_to_volume(glm::vec3(0), glm::vec3(0, 1, 0), 1, 0, true);
		});


	ct_reconstructor::launch_debug_window(solver);
	
	parser.parameters.input_data_type = FBP3D::Volume;
	parser.parameters.output_data_type = FBP3D::Projections;
	
	if (!ct_reconstructor::launch_preview_window_forward(solver, parser)) {
		ct_reconstructor::release();
		return 0;
	}

	ct_reconstructor::forward_project(solver, parser,
		0
	);


	ct_reconstructor::launch_debug_window(solver);

	ct_reconstructor::release();
}
