#include "Component_Transform.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtx/matrix_decompose.hpp"

glm::vec3 TransformComponent::multiply_point(glm::vec3 point)
{
	glm::vec4 point4 = glm::vec4(point, 1);
	point4 = transform * point4;
	point4 = point4 / point4.a;
	return glm::vec3(point4);
}

glm::vec3 TransformComponent::multiply_vector(glm::vec3 vector)
{
	return glm::mat3(transform) * vector;
}

void TransformComponent::translate(glm::vec3 translation)
{
	transform = glm::translate(transform, translation);
}

void TransformComponent::set_positon(glm::vec3 position){
	transform = glm::translate(glm::mat4(glm::mat3(transform)), position);
}

glm::vec3 TransformComponent::get_position(){
	return multiply_point(glm::vec3(0, 0, 0));
}

glm::vec3 TransformComponent::get_x_direction()
{
	return multiply_vector(glm::vec3(1, 0, 0));
}

glm::vec3 TransformComponent::get_y_direction()
{
	return multiply_vector(glm::vec3(0, 1, 0));
}

glm::vec3 TransformComponent::get_z_direction()
{
	return multiply_vector(glm::vec3(0, 0, 1));
}

void TransformComponent::scale(glm::vec3 scale)
{
	transform = glm::scale(transform, scale);
}

void TransformComponent::set_scale(glm::vec3 scale){
	transform[0] = transform[0] / glm::length(transform[0]) * scale.x;
	transform[1] = transform[1] / glm::length(transform[1]) * scale.y;
	transform[2] = transform[2] / glm::length(transform[2]) * scale.z;
}

glm::vec3 TransformComponent::get_scale(){
	glm::vec3 scale = glm::vec3(
		glm::length(glm::vec3(transform[0])),
		glm::length(glm::vec3(transform[1])),
		glm::length(glm::vec3(transform[2]))
	);
	return scale;
}

void TransformComponent::rotate(glm::quat rotation)
{
	transform = glm::mat4_cast(rotation) * transform;
}

void TransformComponent::rotate(glm::vec3 euler_angle)
{
	rotate(glm::quat(euler_angle));
}

void TransformComponent::set_rotation(glm::quat rotation){
	const glm::mat3 scale3(
		glm::vec3(glm::length(transform[0]), 0, 0),
		glm::vec3(0, glm::length(transform[0]), 0),
		glm::vec3(0, 0, glm::length(transform[0])));

	glm::vec3 translate = get_position();
	glm::mat4 rotation_m = glm::mat4_cast(rotation);
	glm::vec3 scale = get_scale();

	transform = glm::translate(rotation_m * glm::scale(glm::mat4(1), scale), translate);
}

void TransformComponent::set_rotation(glm::vec3 euler_angles_radian)
{
	set_rotation(glm::quat(euler_angles_radian));
}

glm::quat TransformComponent::get_rotation(){
	return glm::quat_cast(transform);
}

glm::vec3 TransformComponent::get_rotation_euler(){
	return glm::eulerAngles(get_rotation());
}

void TransformComponent::set_direction(glm::vec3 direction){
	if (direction == glm::vec3(0, 1, 0) || direction == glm::vec3(0, -1, 0))
		direction += glm::vec3(0.01, 0, 0.01);

	glm::vec3 position = get_position();
	std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	
	glm::vec3 up = glm::vec3(0, 1, 0);

	//glm::vec3 rotation_axis = glm::normalize(glm::cross(glm::vec3(1, 0, 0), direction));
	//float cosandle = glm::dot(direction, glm::vec3(1, 0, 0));
	//glm::quat rotation = glm::rotate(glm::quat(), glm::acos(cosandle), rotation_axis);

	glm::quat rotation(direction, glm::vec3(0, 1, 0));

	set_rotation(rotation);
	
	position = get_position();
	glm::vec3 d = get_z_direction();
	std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	std::cout << d.x << " " << d.y << " " << d.z << std::endl;
}

//void TransformComponent::look_at(glm::vec3 self_position, glm::vec3 target_position){
//	if ((target_position - self_position) == glm::vec3(0, 1, 0) || (target_position - self_position) == glm::vec3(0, -1, 0))
//		target_position += glm::vec3(0.01, 0, 0.01);
//
//	transform = glm::lookAt(self_position, target_position, glm::vec3(0, 1, 0));
//}

TransformComponent::Decomposition TransformComponent::decompose()
{
	TransformComponent::Decomposition decomposition;
	decomposition.scale;
	decomposition.rotation;
	decomposition.translation;
	decomposition.skew;
	decomposition.perspective;
	glm::decompose(
		transform, 
		decomposition.scale, 
		decomposition.rotation, 
		decomposition.translation, 
		decomposition.skew, 
		decomposition.perspective
	);
	return decomposition;
}


void TransformComponent::clear(){
	transform = glm::mat4(1);
}