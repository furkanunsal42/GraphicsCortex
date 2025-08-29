#include "CTReconstructor.h"

#include "Windows.h"

std::filesystem::path get_executable_path_windows() {
	wchar_t ownPth[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
	
	return std::filesystem::path(ownPth);
}

int main() {

	std::filesystem::path working_directory = std::filesystem::absolute(".");
	std::filesystem::path executable_directory = get_executable_path_windows().parent_path();
	
	std::filesystem::path kernels_directory_ct = std::filesystem::canonical(executable_directory / "../CTReconstructor/Source/GLSL/");
	std::filesystem::path kernels_directory_cortex = std::filesystem::canonical(executable_directory / "../GraphicsCortex/Source/GLSL/");

	std::filesystem::path descriptor_file_path = working_directory;

	ct_reconstructor::init(
		kernels_directory_ct,
		kernels_directory_cortex
	);

	ParameterParser parser;
	ASSERT(parser.read(descriptor_file_path));
	
	FBP3D solver(parser);

	solver.read_projections(parser);

	ct_reconstructor::filter(solver, parser, 
		transfer_inputs_from_ram_on_begin		|
		apply_log_normalization_to_projections	|
		apply_filter_to_projections				
		);
	
	solver.generate_blank_volume(parser.parameters);

	bool canceled = !ct_reconstructor::launch_preview_window(solver, parser, executable_directory);
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
