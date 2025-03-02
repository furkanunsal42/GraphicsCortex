#pragma once

#include "Component/Component.h"
#include "glm.hpp"
#include "gtx/quaternion.hpp"

class TransformComponent : public Component {
public:

	struct Decomposition {
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
	};

	glm::mat4 transform = glm::mat4(1);

	glm::vec3 multiply_point(glm::vec3 point);
	glm::vec3 multiply_vector(glm::vec3 vector);

	void translate(glm::vec3 translation);
	void set_positon(glm::vec3 position);
	glm::vec3 get_position();
	glm::vec3 get_x_direction();
	glm::vec3 get_y_direction();
	glm::vec3 get_z_direction();

	void scale(glm::vec3 scale);
	void set_scale(glm::vec3 scale);
	glm::vec3 get_scale();

	void rotate(glm::quat rotation);
	void rotate(glm::vec3 euler_angle);
	void set_rotation(glm::quat rotation);
	void set_rotation(glm::vec3 euler_angles_radian);
	glm::quat get_rotation();
	glm::vec3 get_rotation_euler();

	void set_direction(glm::vec3 direction);
	//void look_at(glm::vec3 self_positon, glm::vec3 target_position);

	Decomposition decompose();
	void clear();
};