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
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3";

	// protezler
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/rekonstruktion.ini";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/projektion";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction5";


	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.volume_path = volume_path;
	parameters.projections_path = projections_path;
	//parameters.volume_resolution = glm::ivec3(1024);

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	solver.read_projections(parameters);
	//solver.projections_transfer_ram_to_vram();
	ct_reconstructor::back_project(solver, geometry, parameters, 
		save_ouptut_to_disk						|
		apply_filter_to_projections				|
		apply_log_normalization_to_projections	|
		apply_minmax_normalization_to_volume	|
		clip_negatives_of_volume				|
		transfer_inputs_from_ram_on_begin);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
