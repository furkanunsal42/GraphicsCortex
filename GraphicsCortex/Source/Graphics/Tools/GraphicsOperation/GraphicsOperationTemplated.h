#pragma once
#include "GraphicsOperation.h"
#include "ShaderCompiler.h"
#include "Debuger.h"
#include "Buffer.h"
#include <string>

namespace {
	int32_t glsl_primitive_name_to_size_in_bytes(const std::string& glsl_primitive) {
		if (glsl_primitive == "int")	return sizeof(int32_t);
		if (glsl_primitive == "uint")	return sizeof(uint32_t);
		if (glsl_primitive == "float")	return sizeof(float);
		if (glsl_primitive == "vec2")	return sizeof(glm::vec2);
		if (glsl_primitive == "vec3")	return sizeof(glm::vec3);
		if (glsl_primitive == "vec4")	return sizeof(glm::vec4);
		if (glsl_primitive == "ivec2")	return sizeof(glm::ivec2);
		if (glsl_primitive == "ivec3")	return sizeof(glm::ivec3);
		if (glsl_primitive == "ivec4")	return sizeof(glm::ivec4);
		if (glsl_primitive == "uvec2")	return sizeof(glm::uvec2);
		if (glsl_primitive == "uvec3")	return sizeof(glm::uvec3);
		if (glsl_primitive == "uvec4")	return sizeof(glm::uvec4);
		std::cout << "[GraphicsOperation Error] glsl_primitive_name_to_size_in_bytes() is called but given type isn't supported" << std::endl;
		ASSERT(false);
		return 0;
	}

	template<typename T,
		std::enable_if_t<is_device_buffer_type(T), bool> = true>
	int32_t get_dimentionality() {
		return 1;
	}

	template<typename T,
		std::enable_if_t<is_device_texture_type(T), bool> = true>
	int32_t get_dimentionality() {
		return TextureBase2::get_texture_dimention<T>();
	}

	template<typename T,
		std::enable_if_t<is_device_buffer_type(T), bool> = true>
	std::string get_precise_type(T& value, const std::string& buffer_precise_type, bool interpolation) {
		return buffer_precise_type;
	}

	template<typename T,
		std::enable_if_t<is_device_texture_type(T), bool> = true>
	std::string get_precise_type(T& value, const std::string& buffer_precise_type, bool interpolation) {
		if (!interpolation)
			return TextureBase2::ColorTextureFormat_to_OpenGL_compute_Image_type<T>(value.get_internal_format_color());
		else
			return "sampler" + std::to_string(get_dimentionality<T>()) + "D";
	}

	template<typename T,
		std::enable_if_t<is_device_buffer_type(T), bool> = true>
	std::string get_image_internal_format(T& value) {
		return "";
	}

	template<typename T,
		std::enable_if_t<is_device_texture_type(T), bool> = true>
	std::string get_image_internal_format(T& value) {
		std::string result = TextureBase2::ColorTextureFormat_to_OpenGL_compute_Image_format(value.get_internal_format_color());
		if (result == "") {
			std::cout << "[OpenGL Error] GraphicsOperation::compute() is called with an image type but image format is not supported to be used in compute shaders." << std::endl;
			ASSERT(false);
		}
		return result;
	}

	template<typename T,
		std::enable_if_t<is_device_buffer_type(T), bool> = true>
	void  kernel_upndate_uniform_data(ComputeProgram& kernel, const std::string& uniform_name, T& value, bool is_sampler) {
		kernel.update_uniform_as_storage_buffer(uniform_name, value);
	}

	template<typename T,
		std::enable_if_t<is_device_texture_type(T), bool> = true>
	void  kernel_upndate_uniform_data(ComputeProgram& kernel, const std::string& uniform_name, T& value, bool is_sampler) {
		if (is_sampler)
			kernel.update_uniform(uniform_name, value);
		else
			kernel.update_uniform_as_image(uniform_name, value, 0);
	}

	glm::uvec3 get_resolution(Buffer& value, const std::string& buffer_primitive_name) {
		return glm::uvec3(value.get_buffer_size_in_bytes() / glsl_primitive_name_to_size_in_bytes(buffer_primitive_name), 1, 1);
	}

	template<typename T>
	glm::ivec3 ivec_n_to_ivec3(const T& vector);
	template<> glm::ivec3 ivec_n_to_ivec3<glm::ivec1>(const glm::ivec1& vector) { return glm::ivec3(vector.x, 1, 1); }
	template<> glm::ivec3 ivec_n_to_ivec3<glm::ivec2>(const glm::ivec2& vector) { return glm::ivec3(vector.x, vector.y, 1); }
	template<> glm::ivec3 ivec_n_to_ivec3<glm::ivec3>(const glm::ivec3& vector) { return glm::ivec3(vector.x, vector.y, vector.z); }
	template<> glm::ivec3 ivec_n_to_ivec3<glm::ivec4>(const glm::ivec4& vector) { return glm::ivec3(vector.x, vector.y, vector.z); }


	template<typename T,
		std::enable_if_t<is_device_texture_type(T), bool> = true>
	glm::uvec3 get_resolution(T& value, const std::string& buffer_primitive_name) {
		glm::uvec3 resolution(ivec_n_to_ivec3(value.get_size()));
		if (resolution.x == 0) resolution.x = 1;
		if (resolution.y == 0) resolution.y = 1;
		if (resolution.z == 0) resolution.z = 1;
		return resolution;
	}
}

template<typename target_t, typename source_t, typename operand_t,
		std::enable_if_t<is_device_data_type(target_t), bool>,
		std::enable_if_t<is_device_data_type(source_t), bool>,
		std::enable_if_t<is_device_data_type(operand_t), bool>>
void GraphicsOperation::_compute_general(
	target_t& target, const std::string& target_precise_type,
	source_t* source, const std::string& source_precise_type, bool source_interpolation,
	operand_t* operand, const std::string& operand_precise_type, bool operand_interpolation,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	std::vector<std::pair<std::string, std::string>> preprocessor_definitions;

	bool is_source_enabled = source != nullptr;
	bool is_operand_enabled = operand != nullptr;

	bool is_target_buffer = is_device_buffer_type(target_t);
	bool is_source_buffer = is_device_buffer_type(source_t);
	bool is_operand_buffer = is_device_buffer_type(operand_t);

	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("source_enabled", is_source_enabled ? "1" : "0"));
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("operand_enabled", is_operand_enabled ? "1" : "0"));

	if (is_source_enabled) {
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("source_type",
				is_source_buffer ? "buffer_t" :
				source_interpolation ? "sampler_t" :
				"image_t"));
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("source_precise_type", get_precise_type(*source, source_precise_type, source_interpolation)));
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("source_image_internal_format", get_image_internal_format<source_t>(*source)));
	}

	if (is_operand_enabled) {
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("operand_type", is_operand_buffer ? "buffer_t" :
				operand_interpolation ? "sampler_t" :
				"image_t"));
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("operand_precise_type", get_precise_type(*operand, operand_precise_type, operand_interpolation)));
		preprocessor_definitions.push_back(std::pair<std::string, std::string>
			("operand_image_internal_format", get_image_internal_format<operand_t>(*operand)));
	}

	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("target_type", is_target_buffer ? "buffer_t" : "image_t"));
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("target_precise_type", get_precise_type(target, target_precise_type, false)));
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("target_image_internal_format", get_image_internal_format<target_t>(target)));

	std::string final_preprocessing = setted_precomputation_statements + pushed_precomputation_statements;
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("preprocessing", final_preprocessing));
	pushed_precomputation_statements.clear();

	std::string constant_statements = _create_constant_definition();
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("constant_definitions", constant_statements));

	int dimentionality = 1;

	if (glm::all(glm::lessThanEqual(id_end, glm::ivec3(1, 1, 1)))) {
		int dimentionality = get_dimentionality<target_t>();
	}
	else if (id_end.y == 1 && id_end.z == 1) dimentionality = 1;
	else if (id_end.z == 1) dimentionality = 2;
	else dimentionality = 3;

	glm::ivec3 workgroup_size = dimentionality == 1 ? glm::ivec3(64, 1, 1) :
								dimentionality == 2 ? glm::ivec3(8, 8, 1) :
								dimentionality == 3 ? glm::ivec3(4, 4, 4) : glm::ivec3(4, 4, 4);
		
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("operation", operation));

	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("enable_strong_operation", (operation.find(';') != std::string::npos) ? "1" : "0"));

	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("workgroup_size_x", std::to_string(workgroup_size.x)));
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("workgroup_size_y", std::to_string(workgroup_size.y)));
	preprocessor_definitions.push_back(std::pair<std::string, std::string>
		("workgroup_size_z", std::to_string(workgroup_size.z)));

	ComputeProgram& kernel = *_compile_shaders(preprocessor_definitions);

	if (is_source_enabled) {
		kernel_upndate_uniform_data(kernel, "source_data", *source, source_interpolation);
		kernel.update_uniform("source_resolution", get_resolution(*source, source_precise_type));
	}

	if (is_operand_enabled) {
		kernel_upndate_uniform_data(kernel, "operand_data", *operand, operand_interpolation);
		kernel.update_uniform("operand_resolution", get_resolution(*operand, operand_precise_type));
	}

	kernel_upndate_uniform_data(kernel, "target_data", target, false);
	kernel.update_uniform("target_resolution", get_resolution(target, target_precise_type));

	if (id_end == glm::ivec3(0, 0, 0))
		id_end = get_resolution(target, target_precise_type);
	if (glm::any(glm::greaterThanEqual(id_begin, id_end))) {
		std::cout << "[OpenGL Error] GraphicsOperation.compute() is called but id_begin is greater than id_end." << std::endl;
		ASSERT(false);
	}

	kernel.update_uniform("id_begin", glm::uvec3(id_begin));
	kernel.update_uniform("id_end", glm::uvec3(id_end));

	_update_constant_uniforms(kernel);
	pushed_constants.clear();

	glm::ivec3 dispatch_thread_size = id_end - id_begin;
	kernel.dispatch_thread(dispatch_thread_size.x, dispatch_thread_size.y, dispatch_thread_size.z);

}

template<typename target_t, typename source_t,
	std::enable_if_t<is_device_data_type(target_t), bool>,
	std::enable_if_t<is_device_data_type(source_t), bool>>
	void GraphicsOperation::_compute_basic(
	target_t& target,	const std::string& target_precise_type,
	source_t* source,	const std::string& source_precise_type,		bool source_interpolation, 					
	const std::string& operation, 					
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	switch (operand_type) {
	case texture1d:
		_compute_general<target_t, source_t, Texture1D>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Texture1D>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	case texture2d:
		_compute_general<target_t, source_t, Texture2D>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Texture2D>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	case texture3d:
			_compute_general<target_t, source_t, Texture3D>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Texture3D>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	case texture2d_array:
			_compute_general<target_t, source_t, Texture2DArray>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Texture2DArray>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	case texture_cubmap:
		//	_compute_general<target_t, source_t, TextureCubeMap>(
		//	target, target_precise_type,
		//	source, source_precise_type, source_interpolation,
		//	_get_operand_ptr<TextureCubeMap>().get(), operand_buffer_precise_type, operand_interpolation,
		//	operation,
		//	id_begin, id_end
		//); 
		return;
	case buffer:
			_compute_general<target_t, source_t, Buffer>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Buffer>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	case none:
			_compute_general<target_t, source_t, Buffer>(
			target, target_precise_type,
			source, source_precise_type, source_interpolation,
			_get_operand_ptr<Buffer>().get(), operand_buffer_precise_type, operand_interpolation,
			operation,
			id_begin, id_end
		); return;
	}
}


// ----------- Public Functions -----------------

template<typename target_t, typename source_t,
	std::enable_if_t<is_device_texture_type(target_t), bool>,
	std::enable_if_t<is_device_texture_type(source_t), bool>>
void GraphicsOperation::compute(
	target_t& target,
	source_t& source, bool source_interpolation,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, "",
		&source, "", source_interpolation,
		operation,
		id_begin, id_end);
}

template<typename target_t, typename source_t,
	std::enable_if_t<is_device_buffer_type(target_t), bool>,
	std::enable_if_t<is_device_texture_type(source_t), bool>>
void GraphicsOperation::compute(
	target_t& target, const std::string& target_precise_type,
	source_t& source, bool source_interpolation,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, target_precise_type,
		&source, "", source_interpolation,
		operation,
		id_begin, id_end);
}

template<typename target_t, typename source_t,
	std::enable_if_t<is_device_texture_type(target_t), bool>,
	std::enable_if_t<is_device_buffer_type(source_t), bool>>
void GraphicsOperation::compute(
	target_t& target,
	source_t& source, const std::string& source_precise_type,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, "",
		&source, source_precise_type, false,
		operation,
		id_begin, id_end);
}

template<typename target_t, typename source_t,
	std::enable_if_t<is_device_buffer_type(target_t), bool>,
	std::enable_if_t<is_device_buffer_type(source_t), bool>>
void GraphicsOperation::compute(
	target_t& target, const std::string& target_precise_type,
	source_t& source, const std::string& source_precise_type,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, target_precise_type,
		&source, source_precise_type, false,
		operation,
		id_begin, id_end);
}

// -----

template<typename target_t,
	std::enable_if_t<is_device_texture_type(target_t), bool>>
void GraphicsOperation::compute(
	target_t& target,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, "",
		(Texture2D*)(int*)nullptr, "", false,
		operation,
		id_begin, id_end);
}

template<typename target_t,
	std::enable_if_t<is_device_buffer_type(target_t), bool>>
void GraphicsOperation::compute(
	target_t& target, const std::string& target_precise_type,
	const std::string& operation,
	glm::ivec3 id_begin, glm::ivec3 id_end)
{
	_compute_basic(
		target, target_precise_type,
		(Texture2D*)nullptr, "", false,
		operation,
		id_begin, id_end);
}

namespace {
	template<typename T, std::enable_if_t<is_device_primitive_type(T), bool> = true>
	GraphicsOperation::_ConstantType get_constant_type();

	template<> GraphicsOperation::_ConstantType get_constant_type<int32_t>() { return GraphicsOperation::_ConstantType::c_int32; };
	template<> GraphicsOperation::_ConstantType get_constant_type<uint32_t>() { return GraphicsOperation::_ConstantType::c_uint32; };
	template<> GraphicsOperation::_ConstantType get_constant_type<float>() { return GraphicsOperation::_ConstantType::c_float; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::vec2>() { return GraphicsOperation::_ConstantType::c_vec2; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::vec3>() { return GraphicsOperation::_ConstantType::c_vec3; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::vec4>() { return GraphicsOperation::_ConstantType::c_vec4; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::ivec2>() { return GraphicsOperation::_ConstantType::c_ivec2; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::ivec3>() { return GraphicsOperation::_ConstantType::c_ivec3; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::ivec4>() { return GraphicsOperation::_ConstantType::c_ivec4; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::uvec2>() { return GraphicsOperation::_ConstantType::c_uvec2; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::uvec3>() { return GraphicsOperation::_ConstantType::c_uvec3; };
	template<> GraphicsOperation::_ConstantType get_constant_type<glm::uvec4>() { return GraphicsOperation::_ConstantType::c_uvec4; };
}

template<typename T, std::enable_if_t<is_device_primitive_type(T), bool>>
void GraphicsOperation::set_constant(const std::string& name, const T& value) {
	for (int i = 0; i < setted_constants.size(); i++) {
		if (setted_constants[i].name == name) {
			std::memcpy(&setted_constants[i].value, &value, sizeof(value));
			setted_constants[i].constant_type = get_constant_type<T>();
			return;
		}
	}
	for (int i = 0; i < pushed_constants.size(); i++) {
		if (pushed_constants[i].name == name) {
			pushed_constants.erase(pushed_constants.begin() + i);
			break;
		}
	}

	setted_constants.emplace_back(_constant_with_info{.value=0, .name = name,.constant_type = get_constant_type<T>()});
	std::memcpy(&setted_constants.back().value, &value, sizeof(value));
}

template<typename T, std::enable_if_t<is_device_primitive_type(T), bool>>
void GraphicsOperation::push_constant(const std::string& name, const T& value) {
	for (int i = 0; i < pushed_constants.size(); i++) {
		if (pushed_constants[i].name == name) {
			std::memcpy(&pushed_constants[i].value, &value, sizeof(value));
			pushed_constants[i].constant_type = get_constant_type<T>();
			return;
		}
	}
	for (int i = 0; i < setted_constants.size(); i++) {
		if (setted_constants[i].name == name) {
			setted_constants.erase(setted_constants.begin() + i);
			setted_constants[i].constant_type = get_constant_type<T>();
			break;
		}
	}
	pushed_constants.emplace_back(_constant_with_info{ .value = 0, .name = name, .constant_type = get_constant_type<T>() });
	std::memcpy(&pushed_constants.back().value, &value, sizeof(value));
}
