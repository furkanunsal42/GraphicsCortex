#include "CTReconstructor.h"

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
	std::filesystem::path descriptor_file_path = "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path projections_path = "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path volume_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction3/MAR2";

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

	bool synthetic_volume = false;

	if (!synthetic_volume) {
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
	}
	else {
		solver.generate_blank_volume(parameters);
		solver.generate_shepplogan();
		//solver.render_sphere(glm::vec3(0), 2, 0.5);
	}

	FBP3D solver_forward(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	FBP3D::ReconstructionParameters parameters_forward;
	parameters_forward.input_data_type = FBP3D::Volume;
	parameters_forward.output_data_type = FBP3D::Projections;
	parameters_forward.output_resolution = glm::ivec3(parameters.output_resolution.x, parameters.output_resolution.y, parameters.input_files_count);

	solver.move_volume(solver_forward);
	solver_forward.generate_blank_projections(parameters_forward);
	solver_forward.project_forward(geometry);
	solver_forward.move_volume(solver);

	ct_reconstructor::launch_debug_window(solver, "Reconstruction");
	ct_reconstructor::launch_debug_window(solver_forward, "Forward Projection");

	if (synthetic_volume) {
		solver_forward.move_projections(solver);
		ct_reconstructor::back_project(solver, geometry, parameters,
			//transfer_inputs_from_ram_on_begin |
			apply_filter_to_projections 
			//apply_log_normalization_to_projections
			//clip_negatives_of_volume
			//apply_minmax_normalization_to_volume |
		);
		ct_reconstructor::launch_debug_window(solver, "Reconstruction");
	}

	ct_reconstructor::release();
}