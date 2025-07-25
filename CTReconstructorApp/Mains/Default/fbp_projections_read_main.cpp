#include "CTReconstructor.h"

int main() {

	// synthetic projections
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path input_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstructionProjections3F/CTReconstructionProjections3F";
	std::filesystem::path output_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstructionSegmentedTestS";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.input_data_type = FBP3D::Projections;
	parameters.input_files_byte_per_channel = 2;
	parameters.input_files_type = FBP3D::RAW_F;
	parameters.input_files_count = 1440;
	parameters.input_files_resolution = glm::ivec2(1440);
	
	parameters.output_data_type = FBP3D::Volume;
	parameters.output_resolution = glm::ivec3(1440, 1440, 1440);

	parameters.volume_segment_max_height = 0;
	parameters.projection_segment_max_height = 0;

	parameters.input_files_path = input_path;
	parameters.output_files_path = output_path;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);
	
	solver.read_projections(parameters);
	solver.projections_transfer_ram_to_vram();

	ct_reconstructor::back_project(solver, geometry, parameters, 
		transfer_inputs_from_ram_on_begin		|
		//apply_filter_to_projections				|
		//apply_log_normalization_to_projections	|
		//apply_minmax_normalization_to_volume	|
		//clip_negatives_of_volume				|
		save_output_to_disk
		);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
