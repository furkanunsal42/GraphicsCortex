#include "CTReconstructor.h"

int main() {

	// sage
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path input_path			= "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path output_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstructionProjections3F";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);

	parameters.input_data_type = FBP3D::Projections;
	parameters.input_files_path = input_path;
	//parameters.input_files_count = 2048;
	//parameters.input_files_resolution = glm::ivec2(2048);
	
	parameters.output_data_type = FBP3D::Projections;
	parameters.output_resolution = glm::ivec3(1440, 1440, 1440);
	parameters.output_files_path = output_path;
	parameters.output_files_byte_per_channel = 2;
	parameters.output_files_type = FBP3D::RAW_F;

	parameters.volume_segment_max_height = 0;
	parameters.projection_segment_max_height = 0;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);
	
	solver.read_projections(parameters);
	solver.projections_transfer_ram_to_vram();
	solver.compute_min_value_of_projections();
	solver.compute_max_value_of_projections();
	solver.log_normalize_projections();
	solver.apply_filter_to_projections(FBP2D::FilterType::RAM_LAK);
	solver.projections_transfer_vram_to_ram();
	solver.write_projections(parameters);
	ReconstructionInfo info(geometry, parameters);
	info.save_to_disc(parameters);

	//solver.generate_blank_volume(parameters);
	//solver.generate_shepplogan();

	//ct_reconstructor::forward_project(solver, geometry, parameters, 
	//	apply_filter_to_projections				|
	//	//apply_log_normalization_to_projections	
	//	save_output_to_disk
	//	);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
