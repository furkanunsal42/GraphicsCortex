#include "CTReconstructor.h"

#include <mutex>

int main() {

	CTReconstructorLibrary library;
	library.init();

	ParameterParser parser;

	// pipe
	//ASSERT(parser.parse("C:/Users/furkan.unsal/Desktop/Data2/rekonstruktion.ini", true))
	//std::filesystem::path source_projections_path = "C:/Users/furkan.unsal/Desktop/Data2/projektion";
	//std::filesystem::path reconstruction_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction1";

	// pen
	//ASSERT(parser.parse("C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/rekonstruktion.ini", true))
	//std::filesystem::path source_projections_path = "C:/Users/furkan.unsal/Desktop/Data3/[vg-data]20240802111906.478/projektion";
	//std::filesystem::path reconstruction_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction2";

	// sage
	ASSERT(parser.parse("C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt", true));
	std::filesystem::path source_projections_path = "C:/Users/furkan.unsal/Desktop/Projektionen";
	std::filesystem::path reconstruction_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction3";

	bool save = true;
	glm::ivec3 volume_resolution(1024, 1024, 1024);

	glm::ivec2 projections_resolution		= glm::ivec2(parser.query<int32_t>(ParameterParser::input_projection_resolution));
	int32_t projection_count				= parser.query<float>(ParameterParser::input_projection_count);
	int32_t projections_channel_count		= 1;
	int32_t projections_byte_per_channel	= 2;
	
	FBP3D::ReconstructionGeometry_Conebeam geometry(parser);

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(
		FBP3D::FloatingPointPrecision::fp16,
		glm::ivec3(volume_resolution.x, volume_resolution.y / 1, projection_count),
		glm::ivec3(volume_resolution.x, volume_resolution.y / 1, volume_resolution.z)
	);

	solver->read_projections(
		source_projections_path.string(),
		projections_resolution.x, projections_resolution.y,
		projections_channel_count,
		projections_byte_per_channel,
		volume_resolution.x, volume_resolution.y,
		geometry.mirror_input_projections_v,
		projection_count
	);

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, false, true, [&](glm::ivec3 projection_segment_index) {
		if (geometry.mirror_input_projections_u)
			solver->mirror_projections_along_x();
		solver->compute_min_value_of_projections();
		solver->compute_max_value_of_projections();
		});

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, false, false, [&](glm::ivec3 projection_segment_index) {
		solver->log_normalize_projections();
		solver->apply_fdk_weights_to_projections(geometry);
		solver->apply_filter_to_projections(FBP2D::FilterType::RAM_LAK);
		});
	
	solver->generate_blank_volume(volume_resolution.x, volume_resolution.y, volume_resolution.x);
	
	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, true, false, [&](glm::ivec3 projection_index) {
		fbp_segmented_memory::iterate_horizontal_volume_segments_per_projection(*solver, projection_index.y, geometry, [&](glm::ivec3 volume_segment_index) {
			if (solver->get_volume_segment_count() == glm::ivec3(1, 1, 1))
				solver->projections_clear_ram();
			solver->project_backward(geometry);
			});
		});
	
	//solver->projections_clear_ram();
	//solver->projections_clear_vram();
	
	fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver->clip_negatives_of_volume();
		solver->compute_min_value_of_volume();
		solver->compute_max_value_of_volume();
		});
	
	fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver->normalize_min_max_values_of_volume();
		if (save)
			solver->write_volume(reconstruction_path / reconstruction_path.filename());
		});
	
	ReconstructionInfo info;
	info.name = reconstruction_path.filename().string();
	info.volume_resolution = volume_resolution;
	info.voxel_size_mm = glm::vec3(geometry.volume_width, geometry.volume_height, geometry.volume_width) / glm::vec3(volume_resolution);
	
	if (save)
		info.save_to_disc(reconstruction_path);
	
	init_debug_window(*library.window, *solver);
}
