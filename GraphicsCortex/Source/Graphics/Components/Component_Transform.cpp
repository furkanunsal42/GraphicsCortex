#include "Component_Transform.h"
#include "glm.hpp"
#include "gtx/transform.hpp"
#include "gtx/matrix_decompose.hpp"

glm::mat4 TransformComponent::get_matrix()
{
	glm::mat4 matrix = glm::scale(glm::mat4(1), _scale);
	matrix = glm::mat4_cast(_rotation) * matrix;
	matrix = glm::translate(matrix, _position);
	return matrix;
}

glm::vec3 TransformComponent::multiply_point(glm::vec3 point)
{
	glm::vec4 point4 = glm::vec4(point, 1);
	point4 = get_matrix() * point4;
	point4 = point4 / point4.a;
	return glm::vec3(point4);
}

glm::vec3 TransformComponent::multiply_vector(glm::vec3 vector)
{
	return glm::mat3(get_matrix()) * vector;
}

void TransformComponent::set_position(glm::vec3 position){
	_position = position;
}

glm::vec3 TransformComponent::get_position(){
	return _position;
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

void TransformComponent::set_scale(glm::vec3 scale){
	_scale = scale;
}

glm::vec3 TransformComponent::get_scale(){
	return _scale;
}

void TransformComponent::set_rotation(glm::quat rotation){
	_rotation = rotation;
}

void TransformComponent::set_rotation(glm::vec3 euler_angles_radian)
{
	set_rotation(glm::quat(euler_angles_radian));
}

glm::quat TransformComponent::get_rotation(){
	return _rotation;
}

glm::vec3 TransformComponent::get_rotation_euler(){
	return glm::eulerAngles(_rotation);
}

void TransformComponent::set_x_direction(glm::vec3 direction)
{
}

void TransformComponent::set_y_direction(glm::vec3 direction)
{
	direction = glm::normalize(direction);

}

void TransformComponent::set_z_direction(glm::vec3 direction)
{
	direction = glm::normalize(direction);

	glm::vec3 rotation_vector;
	float angle;
	if (direction == glm::vec3(0, 0, 1)) {
		rotation_vector = glm::vec3(0, 1, 0);
		angle = 0;
	}
	else if (direction == glm::vec3(0, 0, -1)) {
		rotation_vector = glm::vec3(0, 1, 0);
		angle = glm::pi<float>();
	}
	else {
		rotation_vector = glm::cross(glm::vec3(0, 0, 1), direction);
		angle = glm::acos(glm::dot(glm::vec3(0, 0, 1), direction));
	}

	std::cout << "rotation_vector : " << rotation_vector.x << " " << rotation_vector.y << " " << rotation_vector.z << std::endl;
	std::cout << "angle : " << angle * 180 / 3.14 << std::endl;

	set_rotation(glm::rotate(angle, rotation_vector));
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
		get_matrix(),
		decomposition.scale, 
		decomposition.rotation, 
		decomposition.translation, 
		decomposition.skew, 
		decomposition.perspective
	);
	return decomposition;
}


void TransformComponent::clear(){
	_position = glm::vec3(0);
	_rotation = glm::quat();
	_scale = glm::vec3(1);
}