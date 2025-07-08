#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"
#include "FileParser/ParameterParser.h"

/*
int main() {

	float xray_source_distance = 600;
	float detector_panel_distance = 400;
	
	float detector_width = 409.60f;
	float detector_height = 409.60f;

	float volume_width = 209.196216;
	float volume_height = 209.196216;
	float volume_depth = 209.196216;

	float full_rotation_count = 1;
	
	int volume_resolution_x = 1024;
	int volume_resolution_y = 1024;

	int detector_resolution_x = 1024;
	int detector_resolution_y = 1024;

	float radian = 0;

	float Ou = 0;
	float Ov = 0;
	float Ocorr = 0;

	glm::mat4x4 model = glm::identity<glm::mat4x4>();
	model = glm::translate(model, -glm::vec3(volume_width / 2, volume_height / 2, volume_width / 2));
	model = glm::scale(model, glm::vec3(volume_width / volume_resolution_x, volume_height / volume_resolution_y, volume_depth / volume_resolution_x));

	glm::mat4x4 view = glm::identity<glm::mat4x4>();
	view = glm::rotate(view, 3 * glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	view = glm::translate(view, -glm::vec3(detector_panel_distance, 0, 0));
	view = glm::rotate(view, radian, glm::vec3(0, 1, 0));
	
	glm::mat4x4 ortogonal_projection(
		glm::vec4(1 / detector_width * detector_resolution_x, 0, 0, 0),
		glm::vec4(0, 1 / detector_height * detector_resolution_y, 0, 0),
		glm::vec4(0, 0, -1, 0),
		glm::vec4(0, 0, 0, 1)
	);
	
	ortogonal_projection = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(detector_resolution_x / 2, detector_resolution_y / 2, 0)) * ortogonal_projection;

	glm::mat4x4 perspective_projection(
		glm::vec4((xray_source_distance + detector_panel_distance) / detector_width * detector_resolution_x, 0, 0, 0),
		glm::vec4(0, (xray_source_distance + detector_panel_distance) / detector_width * detector_resolution_y, 0, 0),
		glm::vec4(0, 0, 0, -1),
		glm::vec4(0, 0, 1, 0)
	);
	perspective_projection = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(detector_resolution_x / 2, detector_resolution_y / 2, 0)) * perspective_projection;

	glm::vec3 pos(512, 1024, 512);

	glm::vec4 coord = perspective_projection * view * model * glm::vec4(pos, 1);
	float depth = coord.w == 1 ? coord.z : coord.w;
	coord = coord / coord.w;
	std::cout << coord.x << " " << coord.y << " " << depth << " " << coord.w << std::endl;

}*/

double get_linear_depth(double d, double n, double f) {
	return 2.0 * f * n / (-d * (f - n) + (f + n));
}

double get_logarithm_depth(double d, double n, double f) {
	return (f+n-2*f*n/d)/(f-n);
}

/*
int main() {

	double xray_source_distance = 600;
	double detector_panel_distance = 400;

	double detector_width = 409.60f;
	double detector_height = 409.60f;

	double volume_width = 209.196216;
	double volume_height = 209.196216;
	double volume_depth = 209.196216;

	double full_rotation_count = 1;

	int volume_resolution_x = 1024;
	int volume_resolution_y = 1024;

	int detector_resolution_x = 1024;
	int detector_resolution_y = 1024;

	volume_width = xray_source_distance * detector_width / (xray_source_distance + detector_panel_distance + detector_width / 2.0f);
	volume_height = volume_width;

	double radian = glm::pi<double>() * 0 / 2.0;
	
	double dx = volume_width / volume_resolution_x;
	double dy = volume_height / volume_resolution_y;
	double dz = volume_depth / volume_resolution_x;

	size_t Nx = volume_resolution_x;
	size_t Ny = volume_resolution_y;
	size_t Nz = volume_resolution_x;

	double dso = xray_source_distance;
	double ddo = detector_panel_distance;
	double dsd = dso + ddo;

	glm::dmat4x4 model = glm::identity<glm::dmat4x4>();
	model = glm::scale(model, glm::dvec3(dx, dy, dz));
	model = glm::translate(model, -glm::dvec3(Nx, Ny, Nz)/2.0);
	
	glm::dmat4x4 view = glm::identity<glm::dmat4x4>();
	//view = glm::rotate(view, 3 * glm::pi<double>() / 2, glm::vec3(0, 1, 0));
	view = glm::translate(view, glm::dvec3(0, 0, dso));
	view = glm::rotate(view, -(double)radian, glm::dvec3(0, 1, 0));

	double z_near = 0.1;
	double z_far = 2048 * 32;

	glm::dmat4x4 projection = glm::perspectiveLH(glm::atan((detector_width / 2) / dsd)*2, 1.0, z_near, z_far);
	projection = glm::scale(glm::identity<glm::dmat4x4>(),		glm::dvec3(detector_resolution_x / 2.0, detector_resolution_y / 2.0, 1)) * projection;
	projection = glm::translate(glm::identity<glm::dmat4x4>(),	glm::dvec3(detector_resolution_x, detector_resolution_y, 0)/2.0) * projection;

	glm::dmat4x4 matrix = projection * view * model;
	glm::dmat4x4 inv_matrix = glm::inverse(matrix);

	glm::dvec3 pos(1024, 1024, 0);
	glm::dvec4 coord = matrix * glm::dvec4(pos, 1);
	glm::dvec3 coord3 = glm::dvec3(coord) / coord.w;
	double linear_depth = get_linear_depth(coord3.z, z_near, z_far);
	double logarithm_depth = get_logarithm_depth(linear_depth, z_near, z_far);

	glm::dvec3 detector_pos(700, 700, get_logarithm_depth(dso + (0 - 512) * dz, z_near, z_far));
	glm::dvec3 voxel_pos = glm::dvec3(inv_matrix * glm::dvec4(detector_pos, 1)) / (inv_matrix * glm::dvec4(detector_pos, 1)).w;

	std::cout << "volume_width: "		<< volume_width << std::endl;
	std::cout << "voxel_pos: "			<< voxel_pos.x << " " << voxel_pos.y << " " << voxel_pos.z << std::endl;
	//std::cout << "coord: "				<< coord.x << " " << coord.y << " " << coord.z << " " << coord.w << std::endl;
	std::cout << "coord3: "				<< coord3.x << " " << coord3.y << " " << coord3.z << std::endl;
	std::cout << "linear_depth: "		<< linear_depth << std::endl;
	std::cout << "logarithm_depth: "	<< logarithm_depth << std::endl;

	std::cin.get();

	return 0;
}
*/

glm::vec2 ray_aabb_intersect(glm::vec3 bounds_min, glm::vec3 bounds_max, glm::vec3 ray_origin, glm::vec3 ray_direction) {

	glm::vec3 inverse_ray_direction = 1.0f / ray_direction;

	glm::vec3 t0 = (bounds_min - ray_origin) * inverse_ray_direction;
	glm::vec3 t1 = (bounds_max - ray_origin) * inverse_ray_direction;
	glm::vec3 tmin = min(t0, t1);
	glm::vec3 tmax = max(t0, t1);

	float distance_a = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
	float distance_b = glm::min(tmax.x, glm::min(tmax.y, tmax.z));

	float distance_to_box = glm::max(0.0f, distance_a);
	float distance_inside_box = glm::max(0.0f, distance_b - distance_to_box);

	return glm::vec2(distance_to_box, distance_inside_box);
}

void main() {

	WindowDescription desc;
	Window window(desc);

	ParameterParser parser;
	parser.parse("C:/Users/furkan.unsal/Desktop/deneme_21_03_2023.txt", true);
	std::filesystem::path source_projections_path = "C:/Users/furkan.unsal/Desktop/Projektionen";
	//std::filesystem::path reconstruction_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction3";
	std::filesystem::path reconstruction_path = "C:/Users/furkan.unsal/Desktop/CTReconstruction4";
	bool save = true;
	glm::ivec3 volume_resolution(1024);
	int window_width = 1024;
	int slice_index_number_length = 6;

	glm::ivec2 projections_resolution = glm::ivec2(volume_resolution.x, volume_resolution.y);
	int32_t projection_count = 1024;
	int32_t projections_channel_count = 1;
	int32_t projections_byte_per_channel = 2;


	FBP3D::ReconstructionGeometry_Conebeam geometry;
	geometry.volume_width = parser.query<int32_t>(ParameterParser::volume_width);
	geometry.volume_height = parser.query<int32_t>(ParameterParser::volume_height);
	geometry.xray_source_distance = parser.query<float>(ParameterParser::xray_distance);
	geometry.detector_plane_distance = parser.query<float>(ParameterParser::detector_plane_distance);
	geometry.detector_plane_width = parser.query<float>(ParameterParser::detector_plane_width);
	geometry.rotation_radian = parser.query<float>(ParameterParser::rotation_radian);
	geometry.rotation_offset_radian = parser.query<float>(ParameterParser::rotation_offset_radian);
	geometry.detector_plane_offset_u = parser.query<float>(ParameterParser::detector_plane_offset_u);
	geometry.detector_plane_offset_v = parser.query<float>(ParameterParser::detector_plane_offset_v);
	geometry.detector_plane_tilt_radian = parser.query<float>(ParameterParser::detector_plane_tilt_radian);
	geometry.rotation_plane_offset_x = parser.query<float>(ParameterParser::rotation_plane_offset_x);
	geometry.rotation_plane_tilt_z = parser.query<float>(ParameterParser::rotation_plane_tilt_z);
	geometry.clockwise_rotation = parser.query<bool>(ParameterParser::clockwise_rotation);
	geometry.mirror_input_projections_u = parser.query<bool>(ParameterParser::input_projection_mirror_u);
	geometry.mirror_input_projections_v = parser.query<bool>(ParameterParser::input_projection_mirror_v);

	shader_directory::fbp_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FBP/";
	shader_directory::ffft_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FFT/";
	shader_directory::util_shader_directory = "../CTReconstructor/Source/GLSL/Compute/Util/";

	FBP3D solver(FBP3D::fp16);
	
	solver.generate_blank_volume(volume_resolution.x, volume_resolution.y, volume_resolution.z);
	solver.generate_blank_projections(volume_resolution.x, volume_resolution.y, projection_count);

	int32_t invocation_id = 0;
	int32_t projection_id = invocation_id;
	float invocation_angle_radian = geometry.rotation_radian / projection_count * invocation_id;
	invocation_angle_radian += geometry.rotation_offset_radian;

	glm::vec2 projection_vector = glm::vec2(glm::cos(invocation_angle_radian), sin(invocation_angle_radian));

	glm::mat4 projection_matrix = solver._get_projection_matrix(projection_id, projection_count, geometry);
	glm::mat4 inverse_projection_matrix = glm::inverse(projection_matrix);

	glm::ivec3 id = glm::ivec3(300, 300, 0);
	
	// shader

	//if (id.y - volume_offset.y >= volume_segment_size.y) return;
	//if (id.y - volume_offset.y < 0) return;
	glm::vec3 detector_plane_normal = -glm::vec3(projection_vector.x, 0, projection_vector.y);
	glm::vec3 detector_plane_vector_x = glm::vec3(detector_plane_normal.z, 0, -detector_plane_normal.x);
	glm::vec3 detector_plane_vector_y = glm::vec3(0, 1, 0);

	glm::vec3 detector_position0 = glm::vec3(id.x, id.y, get_logarithm_depth(100, solver.z_near, solver.z_far));
	glm::vec4 voxel_position0_h = glm::vec4(inverse_projection_matrix * glm::vec4(detector_position0, 1));
	glm::vec3 voxel_position0 = glm::vec3(voxel_position0_h) / voxel_position0_h.w;

	glm::vec3 detector_position1 = glm::vec3(id.x, id.y, get_logarithm_depth(200, solver.z_near, solver.z_far));
	glm::vec4 voxel_position1_h = glm::vec4(inverse_projection_matrix * glm::vec4(detector_position1, 1));
	glm::vec3 voxel_position1 = glm::vec3(voxel_position1_h) / voxel_position1_h.w;

	glm::vec3 voxel_space_ray_direction = glm::normalize(voxel_position1 - voxel_position0);
	glm::vec3 voxel_space_ray_position = voxel_position0;

	glm::vec2 ray_hit_info = ray_aabb_intersect(glm::vec3(0), volume_resolution, voxel_space_ray_position, voxel_space_ray_direction);
	glm::vec3 voxel_space_ray_hit_position = voxel_position0 + voxel_space_ray_direction * ray_hit_info.x;

	// ----

	glm::vec3 voxel_space_ray_end_position = voxel_position0 + voxel_space_ray_direction * (ray_hit_info.x + ray_hit_info.y);


	//glm::dvec3 pos(1024, 1024, 1024);
	//glm::dvec4 coord = projection_matrix * glm::dvec4(pos, 1);
	//glm::dvec3 coord3 = glm::dvec3(coord) / coord.w;
	//std::cout << std::format("{}, {}, {}", coord3.x, coord3.y, coord3.z) << std::endl;

	std::cout << std::format("{}, {}, {}", voxel_position0.x, voxel_position0.y, voxel_position0.z) << std::endl;
	std::cout << std::format("{}, {}, {}", voxel_position1.x, voxel_position1.y, voxel_position1.z) << std::endl;
	std::cout << std::format("{}, {}, {}", voxel_space_ray_hit_position.x, voxel_space_ray_hit_position.y, voxel_space_ray_hit_position.z) << std::endl;
	std::cout << std::format("{}, {}, {}", voxel_space_ray_end_position.x, voxel_space_ray_end_position.y, voxel_space_ray_end_position.z) << std::endl;
	std::cout << std::format("{}, {}",	   ray_hit_info.x, ray_hit_info.y) << std::endl;

}