#include "CTReconstructor.h"

int main() {

	std::filesystem::path working_directory = std::filesystem::absolute(".");
	working_directory = "C:\\Users\\furkan.unsal\\Desktop\\Data3\\[vg-data]20240802111906.478";

	ct_reconstructor::init_from_package();

	ParameterParser parser;
	ASSERT(parser.read(working_directory, 0));

	parser.parameters.input_data_type = FBP3D::Volume;
	parser.parameters.output_data_type = FBP3D::Projections;
	parser.parameters.input_resolution = glm::ivec3(1024);
	parser.parameters.output_resolution = glm::ivec3(1024);
	parser.validify();
	parser.print();

	FBP3D solver(parser);

	solver.generate_blank_volume(parser);
	fbp_segmented_memory::iterate_horizontal_volume_segments(solver, false, false, [&](glm::ivec3 volume_segment_index) {
		//solver.generate_shepplogan();
		solver.render_sphere(glm::vec3(0), 10, 1);
		});
	
	ct_reconstructor::launch_debug_window(solver);

	ct_reconstructor::forward_project(solver, parser, 0);

	ct_reconstructor::launch_debug_window(solver);
	ct_reconstructor::release();
}