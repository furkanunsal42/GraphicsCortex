#include "CTReconstructor.h"

int main() {

	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/scaninfo.ini";
	 
	// pen
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478";

	// sage
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstruction47";

	// demlik (tiltless)
	//std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/Demlik/[vg-data] 20250818145043.850";

	// bardak (tiltless)
	//std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/Bardak tilt 0/[vg-data] 20250818135056.904";

	// xbox (tiltless)
	//std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/xbox/[vg-data] 20250818150439.488";


	// protezler
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/rekonstruktion.ini";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Protezler/Ornek/20250217164519.485-acetabelum/[vg-data] 20250217164519.485/projektion";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction5";
	
	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.read(descriptor_file_path));
	
	parser.parameters.output_resolution = glm::ivec3(1000);
	parser.geometry.detector_plane_tilt_radian = 0;
	parser.geometry.detector_plane_offset.x = 1.2;
	parser.parameters.filter_type = FBP2D::COSINE;

	//parser.parameters.output_resolution = glm::ivec3(1024);
	//parser.geometry.rotation_plane_offset_x = 1.859;
	//parser.geometry.rotation_plane_offset_x = 0.016;

	// clip border pixels
	//parser.geometry.detector_plane_offset = glm::vec2(2.402);

	FBP3D solver(parser);

	solver.read_projections(parser);

	ct_reconstructor::filter(solver, parser, 
		transfer_inputs_from_ram_on_begin		|
		apply_log_normalization_to_projections	|
		apply_filter_to_projections				
		);
	
	solver.generate_blank_volume(parser.parameters);

	ct_reconstructor::launch_preview_window(solver, parser);

	ct_reconstructor::back_project(solver, parser,
		//apply_minmax_normalization_to_volume	|
		clip_negatives_of_volume |
		save_output_to_disk
	);

	//ct_reconstructor::forward_project(solver, parser, 0);
	
	//FBP3D solver_projections(parser);
	//solver_projections.read_projections(parser);
	//solver_projections.projections_transfer_ram_to_vram();
	//solver_projections.compute_min_value_of_projections();
	//solver_projections.compute_max_value_of_projections();
	//solver_projections.log_normalize_projections();
	//
	//ct_reconstructor::launch_debug_window(solver_projections);
	ct_reconstructor::launch_debug_window(solver);

	ct_reconstructor::release();
}
