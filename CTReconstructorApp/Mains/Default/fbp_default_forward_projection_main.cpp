#include "CTReconstructor.h"

int main() {

	// pipe
	//std::filesystem::path descriptor_file_path		= "C:/Users/furkan.unsal/Desktop/Data2/scaninfo.ini";

	// pen
	//std::filesystem::path descriptor_file_path	= "C:/Users/FurkanPC/Desktop/Data3/[vg-data]20240802111906.478/scaninfo.ini";

	// sage
	//std::filesystem::path descriptor_file_path	= "C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt";
	//std::filesystem::path projections_path		= "C:/Users/furkan.unsal/Desktop/Projektionen";
	//std::filesystem::path volume_path			= "C:/Users/furkan.unsal/Desktop/CTReconstruction3/CTReconstruction47";

	// demlik (tiltless)
	//std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/Demlik/[vg-data] 20250818145043.850";

	// bardak (tiltless)
	//std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/Bardak tilt 0/[vg-data] 20250818135056.904";

	// xbox (tiltless)
	std::filesystem::path descriptor_file_path = "C:/Users/FurkanPC/Desktop/drive/new_data/xbox/[vg-data] 20250818150439.488";

	ct_reconstructor::init();

	ParameterParser parser;
	ASSERT(parser.read(descriptor_file_path));

	parser.geometry.detector_plane_offset.x = 0;
	parser.geometry.detector_plane_offset.y = 0;
	parser.geometry.detector_plane_tilt_radian = glm::radians(00.0f);
	parser.geometry.rotation_plane_offset_x = 0;
	parser.geometry.source_detector_tilt_x_radian = glm::radians(80.0f);

	FBP3D solver(parser);

	solver.generate_blank_volume(parser);
	fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver.generate_shepplogan();
		//solver.render_sphere(glm::vec3(0), 1, 1);
		});

	ct_reconstructor::forward_project(solver, parser, 0);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}