#pragma once

template<typename T>
void UniformBuffer::push_variable()
{
	static_assert(
		std::is_same_v<T, bool> ||
		std::is_same_v<T, int> ||
		std::is_same_v<T, float> ||
		std::is_same_v<T, glm::vec2> ||
		std::is_same_v<T, glm::vec3> ||
		std::is_same_v<T, glm::vec4> ||
		std::is_same_v<T, glm::mat2> ||
		std::is_same_v<T, glm::mat3> ||
		std::is_same_v<T, glm::mat4> ||
		std::is_same_v<T, Texture2D>
		);
	_push_varible_size(_get_gl_std140_size<T>());
}

template<typename T>
void UniformBuffer::set_data(int managed_buffer_offset_in_bytes, int uploading_data_offset_in_bytes, int size_in_bytes, const std::vector<T>& data) {
	if (size_in_bytes == 0) return;
	if (managed_buffer_offset_in_bytes < 0 || uploading_data_offset_in_bytes < 0 || size_in_bytes < 0) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but offset and size was out of bound" << std::endl;
		ASSERT(false)
	}
	if (uploading_data_offset_in_bytes + size_in_bytes > data.size() * sizeof(T)) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but given data's size is lower than given offset + size" << std::endl;
		ASSERT(false);
	}
	if (!_buffer_generated) {
		std::cout << "[OpenGL Error] UniformBuffer tried to set_data() but resource was released" << std::endl;
		ASSERT(false);
	}
	_allocate_buffer();

	set_data(managed_buffer_offset_in_bytes, uploading_data_offset_in_bytes, size_in_bytes, (void*)((char*)data.data /* + custom_data_offset_in_bytes*/));
}

template<typename T>
int	UniformBuffer::_get_gl_std140_size()
{
	static_assert(
		std::is_same_v<T, bool>			||
		std::is_same_v<T, int>			||
		std::is_same_v<T, float>		||
		std::is_same_v<T, glm::vec2>	||
		std::is_same_v<T, glm::vec3>	||
		std::is_same_v<T, glm::vec4>	||
		std::is_same_v<T, glm::mat2>	||
		std::is_same_v<T, glm::mat3>	||
		std::is_same_v<T, glm::mat4>	||
		std::is_same_v<T, Texture2D>
		);

	const int N = 4; // base size

	if (typeid(T) == typeid(bool))			return N;
	if (typeid(T) == typeid(int))			return N;
	if (typeid(T) == typeid(float))			return N;
	if (typeid(T) == typeid(glm::vec2))		return 2 * N;
	if (typeid(T) == typeid(glm::vec3))		return 4 * N;
	if (typeid(T) == typeid(glm::vec4))		return 4 * N;
	if (typeid(T) == typeid(glm::mat2))		return 2 * 4 * N;
	if (typeid(T) == typeid(glm::mat3))		return 3 * 4 * N;
	if (typeid(T) == typeid(glm::mat4))		return 4 * 4 * N;
	if (typeid(T) == typeid(Texture2D))		return 2 * N;
	ASSERT(false);
}
