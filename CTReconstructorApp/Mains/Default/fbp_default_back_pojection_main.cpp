#include "CTReconstructor.h"

int main() {

	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/rekonstruktion.ini";
	//std::filesystem::path projections_path			= "C:/Users/furkan.unsal/Desktop/Data2/projektion";
	//std::filesystem::path volume_path				= "C:/Users/furkan.unsal/Desktop/CTReconstruction1";

	// pen
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/rekonstruktion.ini";
	std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/projektion";
	std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction2";

	// sage
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstruction47";

	// protezler
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/rekonstruktion.ini";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/projektion";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction5";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.input_data_type = FBP3D::Projections;
	parameters.output_data_type = FBP3D::Volume;
	parameters.input_files_path = projections_path;
	parameters.output_files_path = volume_path;
	parameters.volume_segment_max_height = 0;
	parameters.projection_segment_max_height = 0;

	//geometry.rotation_offset_radian = glm::pi<float>() / 2.0f;
	geometry.clockwise_rotation = false;
	geometry.detector_plane_offset_u = 3.71;
	geometry.detector_plane_tilt_radian = glm::radians(0.30);
	geometry.rotation_plane_offset_x = -1.86;
	geometry.rotation_plane_tilt_z = 0;

	//geometry.detector_plane_offset_u = 3.71;
	//geometry.detector_plane_tilt_radian = glm::radians(0.30);
	//geometry.rotation_plane_offset_x = -1.86;
	//geometry.rotation_plane_tilt_z = 0;

	//parameters.output_resolution = glm::ivec3(512);

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);
	
	solver.read_projections(parameters);
	//solver.projections_transfer_ram_to_vram();

	//solver.generate_blank_volume(parameters);
	//solver.generate_shepplogan();

	ct_reconstructor::back_project(solver, geometry, parameters, 
		transfer_inputs_from_ram_on_begin		|
		apply_log_normalization_to_projections	|
		apply_filter_to_projections				|
		//apply_minmax_normalization_to_volume	|
		//clip_negatives_of_volume				|	
		save_output_to_disk
		);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}
