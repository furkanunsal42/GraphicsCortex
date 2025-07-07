#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

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
