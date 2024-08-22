#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

int main() {

	float xray_source_distance = 730.87f;
	float detector_panel_distance = 669.04f;
	float detector_width = 409.60f;
	float volume_width = 213.84f;
	float volume_height = 209.196216;
	float full_rotation_count = 1;
	int volume_resolution_horizontal = 1024;
	int volume_resolution_vertical = 1024;
	float rotation_offset_radian = 0;

	int invocation_angle_radian = 0;

	float Dso = xray_source_distance;
	float Dsd = xray_source_distance + detector_panel_distance;
	float du = detector_width / volume_resolution_horizontal;
	float dv = detector_width / volume_resolution_vertical;
	float dx = volume_width / volume_resolution_horizontal;
	float dy = volume_width / volume_resolution_horizontal;
	float dz = volume_height / volume_resolution_vertical;
	int Nu = volume_resolution_horizontal;
	int Nv = volume_resolution_vertical;
	int Nx = volume_resolution_horizontal;
	int Ny = volume_resolution_horizontal;
	int Nz = volume_resolution_vertical;
	float A = invocation_angle_radian;
	float Ou = 0;
	float Ov = 0;
	float Ocorr = 0;

	/*glm::mat4x4 model = glm::mat4x4(
		dx,	 0	,	0,	(1 - dx * Nx) / 2.0f,
		0 ,	 -dy,	0,	(dy * Ny) / 2.0f,
		0 ,	 0	,	dz,	(dz * Nz) / 2.0f,
		0 ,	 0	,	0,	1);
	model = glm::transpose(model);

	glm::mat4x4 view = glm::mat4x4(
		glm::cos(A),	-glm::sin(A),	0,	Ocorr,
		0,				0,				-1,	0,
		glm::sin(A),	glm::cos(A),	0,	Dso,
		0,				0,				0,	1);
	view = glm::transpose(view);

	glm::mat4x4 projection = glm::mat4x4(
		Dsd / du,	0,			(Nu - 1) / 2.0f + Ou,	0,
		0,			Dsd / dv,	(Nv - 1) / 2.0f + Ov,	0,
		0,			0,			0,						Dsd,
		0,			0,			0,						0);
	projection = glm::transpose(projection);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << model[j][i] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << view[j][i] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << projection[j][i] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	glm::mat4x4 matrix = projection * view * model;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << matrix[j][i] << " ";
		std::cout << std::endl;
	}

	glm::vec4 position(512, 512, 512, 1);
	matrix = glm::transpose(matrix);
	float z = glm::dot(matrix[2], position);
	float x = glm::dot(matrix[0], position) / z;
	float y = glm::dot(matrix[1], position) / z;

	//glm::vec3 xyz = matrix * position;
	//xyz.x /= xyz.z;
	//xyz.y /= xyz.z;

	//glm::vec4 detector_homoheneous = matrix * glm::vec4(position, 1);
	//glm::vec2 detector = glm::vec2(detector_homoheneous.x, detector_homoheneous.y) / detector_homoheneous.z;

	std::cout << x << " " << y << " " << z << std::endl;

	*/

	glm::mat4x4 model = glm::identity<glm::mat4x4>();
	//model = glm::translate(model, -glm::vec3(volume_width / 2, volume_height / 2, volume_width / 2));
	model = glm::scale(model, glm::vec3(volume_width / volume_resolution_horizontal, volume_height / volume_resolution_vertical, volume_width / volume_resolution_horizontal));

	glm::mat4x4 view = glm::identity<glm::mat4x4>();
	view = glm::rotate(view, glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	view = glm::translate(view, -glm::vec3(Dsd - Dso, 0, 0));
	view = glm::rotate(view, A, glm::vec3(0, 1, 0));
	
	glm::mat4x4 ortogonal_projection = glm::ortho<float>(0, detector_width / volume_resolution_horizontal, 0, detector_width / volume_resolution_vertical, 0, -1);
	
	glm::vec3 pos(1024, 512, 512);

	glm::vec4 world = ortogonal_projection * view * model * glm::vec4(pos, 1);
	world = world / world.w;

	std::cout << world.x << " " << world.y << " " << world.z << " " << world.w << std::endl;

}
