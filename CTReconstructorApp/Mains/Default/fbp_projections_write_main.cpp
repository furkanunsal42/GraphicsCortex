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
	parameters.projections_path = volume_path / "CTReconstructionProjections";
	parameters.volume_resolution = glm::ivec3(1440);
	parameters.projection_resolution = glm::ivec2(1440);
	parameters.projection_count = 1440;

	parameters.volume_segment_max_height = 0;
	parameters.projection_segment_max_height = 0;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);
	
	solver.generate_blank_volume(parameters);
	solver.generate_shepplogan();

	ct_reconstructor::forward_project(solver, geometry, parameters, 
		apply_filter_to_projections				|
		//apply_log_normalization_to_projections	
		save_ouptut_to_disk
		);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
