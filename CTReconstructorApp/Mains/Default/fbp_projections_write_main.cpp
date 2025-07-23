#include "CTReconstructor.h"

int main() {

	
	// sage
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstruction40";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.volume_path = volume_path;
	//parameters.projections_path = projections_path;
	parameters.projections_path = volume_path / "CTReconstructionProjections";
	parameters.volume_resolution = glm::ivec3(1440);
	parameters.projection_count = 1440;

	parameters.volume_segment_max_height = 0;
	parameters.projection_segment_max_height = 0;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);
	
	//solver.read_projections(parameters);
	solver.generate_blank_volume(parameters);
	solver.generate_shepplogan();

	ct_reconstructor::forward_project(solver, geometry, parameters, 
		apply_filter_to_projections				|
		//apply_log_normalization_to_projections	
		save_ouptut_to_disk
		);


	solver.volume_transfer_vram_to_ram();

	//ct_reconstructor::back_project(solver, geometry, parameters, 
	//	transfer_inputs_from_ram_on_begin		|
	//	apply_filter_to_projections				|
	//	apply_log_normalization_to_projections	|
	//	apply_minmax_normalization_to_volume	|
	//	clip_negatives_of_volume				//|
	//	//save_ouptut_to_disk
	//	);

	
	//solver.write_volume(parameters);
	//ReconstructionInfo info(geometry, parameters);
	//info.save_to_disc(parameters.volume_path);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
