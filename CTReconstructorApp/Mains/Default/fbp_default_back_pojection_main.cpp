#include "CTReconstructor.h"

int main() {

	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/scaninfo.ini";
	 
	// pen
	std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/scaninfo.ini";

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
	ASSERT(parser.read(descriptor_file_path));

	FBP3D solver(parser);
	
	solver.read_projections(parser);

	ct_reconstructor::back_project(solver, parser, 
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
