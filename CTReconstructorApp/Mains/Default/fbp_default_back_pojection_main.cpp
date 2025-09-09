#include "CTReconstructor.h"

int main() {
	std::filesystem::path working_directory = std::filesystem::absolute(".");
	working_directory = "C:\\Users\\furkan.unsal\\Desktop\\drive\\20250716111737.002\\[vg-data] 20250716111737.002";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.read(working_directory));

	FBP3D solver(parser);

	solver.read_projections(parser);
	
	ct_reconstructor::filter(solver, parser, 
		transfer_inputs_from_ram_on_begin		|
		apply_log_normalization_to_projections	|
		apply_fdk_weights_to_projections		|
		apply_filter_to_projections				
		);
	
	solver.generate_blank_volume(parser.parameters);
	
	bool canceled = !ct_reconstructor::launch_preview_window(solver, parser);
	if (canceled) {
		ct_reconstructor::release();
		return 0;
	}
	
	ct_reconstructor::back_project(solver, parser,
		apply_minmax_normalization_to_volume	|
		clip_negatives_of_volume |
		save_output_to_disk
	);
	
	ct_reconstructor::launch_debug_window(solver);

	ct_reconstructor::release();
}
