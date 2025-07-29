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

	solver.read_projections(parameters);
	ct_reconstructor::back_project(solver, geometry, parameters,
		transfer_inputs_from_ram_on_begin |
		apply_filter_to_projections |
		apply_log_normalization_to_projections
		//clip_negatives_of_volume
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
	ct_reconstructor::launch_debug_window(solver, "original projections");

	solver.mar_normalize_projections(solver_prior);

	ct_reconstructor::launch_debug_window(solver, "MAR normalized projections");

	solver.mar_interpolate_projections(solver_metal);

	ct_reconstructor::launch_debug_window(solver, "MAR Interpolation");

	solver.mar_denormalize_projections(solver_prior);

	ct_reconstructor::launch_debug_window(solver, "MAR Projections");

	ct_reconstructor::back_project(solver, geometry, parameters,
		apply_filter_to_projections |
		clip_negatives_of_volume |
		apply_minmax_normalization_to_volume |
		save_output_to_disk
	);

	ct_reconstructor::launch_debug_window(solver, "MAR Reconstruction");

	ct_reconstructor::release();
}