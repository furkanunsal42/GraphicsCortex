#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

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
	
	//glm::mat4x4 ortogonal_projection = glm::ortho<float>(-detector_width / detector_resolution_x, detector_width / detector_resolution_x, -detector_height / volume_resolution_y, detector_height / volume_resolution_y, 0, 1);
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

}
