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
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.parse(descriptor_file_path, true));

	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);
	FBP3D::ReconstructionParameters parameters(parser);
	parameters.output_files_path = volume_path;
	parameters.input_files_path = projections_path;

	geometry.detector_plane_offset_u = 35;
	geometry.detector_plane_tilt_radian = glm::radians(0.0f);
	//geometry.rotation_plane_offset_x = -1.86;
	geometry.rotation_plane_offset_x = 0;
	geometry.rotation_plane_tilt_z = 0;

	FBP3D solver(
		FBP3D::FloatingPointPrecision::fp16,
		parameters.projection_segment_max_height,
		parameters.volume_segment_max_height
	);

	solver.generate_blank_volume(parameters);
	fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver.generate_shepplogan();
		solver.render_sphere(glm::vec3(0), 1, 1);
		});

	ct_reconstructor::forward_project(solver, geometry, parameters, 0);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}