#pragma once
#include <string>
#include <vector>
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

#include "ComputeProgram.h"
#include <type_traits>
#include <functional>
#include <variant>
#include <unordered_map>
#include <tuple>

class GraphicsOperation {
public:

	GraphicsOperation(GraphicsOperation& other) = delete;
	GraphicsOperation() = default;

	#define is_device_buffer_type(T)	std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, Buffer>

	#define is_device_texture_type(T)	std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, Texture1D>	  || \
										std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, Texture2D>	  || \
										std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, Texture3D>	  || \
										std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, Texture2DArray> || \
										std::is_same_v<std::remove_const_t<std::remove_reference_t<T>>, TextureCubeMap>

	#define is_device_data_type(T)		is_device_buffer_type(T) || is_device_texture_type(T)
	
	#define is_device_primitive_type(T)	std::is_same_v<T, int32_t>	|| \
										std::is_same_v<T, uint32_t>	|| \
										std::is_same_v<T, float>		|| \
										std::is_same_v<T, glm::vec2>	|| \
										std::is_same_v<T, glm::vec3>	|| \
										std::is_same_v<T, glm::vec4>	|| \
										std::is_same_v<T, glm::ivec2>	|| \
										std::is_same_v<T, glm::ivec3>	|| \
										std::is_same_v<T, glm::ivec4>	|| \
										std::is_same_v<T, glm::uvec2>	|| \
										std::is_same_v<T, glm::uvec3>	|| \
										std::is_same_v<T, glm::uvec4>

	// source
	template<typename target_t, typename source_t, 
		std::enable_if_t<is_device_texture_type(target_t), bool > = true,
		std::enable_if_t<is_device_texture_type(source_t), bool> = true>
	void compute(
		target_t& target,
		source_t& source, bool source_interpolation,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	template<typename target_t, typename source_t,
		std::enable_if_t<is_device_buffer_type(target_t), bool > = true,
		std::enable_if_t<is_device_texture_type(source_t), bool> = true>
	void compute(
		target_t& target, const std::string& target_precise_type,
		source_t& source, bool source_interpolation,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	template<typename target_t, typename source_t,
		std::enable_if_t<is_device_texture_type(target_t), bool > = true,
		std::enable_if_t<is_device_buffer_type(source_t), bool> = true>
	void compute(
		target_t& target,
		source_t& source, const std::string& source_precise_type,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));
	
	template<typename target_t, typename source_t,
		std::enable_if_t<is_device_buffer_type(target_t), bool > = true,
		std::enable_if_t<is_device_buffer_type(source_t), bool> = true>
	void compute(
		target_t& target, const std::string& target_precise_type,
		source_t& source, const std::string& source_precise_type,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	// only target
	template<typename target_t,
		std::enable_if_t<is_device_texture_type(target_t), bool > = true>
	void compute(
		target_t& target,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	template<typename target_t,
		std::enable_if_t<is_device_buffer_type(target_t), bool > = true>
	void compute(
		target_t& target, const std::string& target_precise_type,
		const std::string& operation,
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	void set_operand(std::shared_ptr<Texture1D> operand, bool operand_interpolation);
	void set_operand(std::shared_ptr<Texture2D> operand, bool operand_interpolation);
	void set_operand(std::shared_ptr<Texture3D> operand, bool operand_interpolation);
	void set_operand(std::shared_ptr<Texture2DArray> operand, bool operand_interpolation); 
	void set_operand(std::shared_ptr<TextureCubeMap> operand, bool operand_interpolation);
	void set_operand(std::shared_ptr<Buffer> operand, std::string operand_buffer_precise_type);
	void push_operand(std::shared_ptr<Texture1D> operand, bool operand_interpolation);
	void push_operand(std::shared_ptr<Texture2D> operand, bool operand_interpolation);
	void push_operand(std::shared_ptr<Texture3D> operand, bool operand_interpolation);
	void push_operand(std::shared_ptr<Texture2DArray> operand, bool operand_interpolation);
	void push_operand(std::shared_ptr<TextureCubeMap> operand, bool operand_interpolation);
	void push_operand(std::shared_ptr<Buffer> operand, std::string operand_buffer_precise_type);
	void clear_operands();

	void set_precomputation_statement(const std::string& precomputation_statements);
	void push_precomputation_statement(const std::string& precomputation_statement);
	void clear_precomputation_statements();

	template<typename T, std::enable_if_t<is_device_primitive_type(T), bool> = true>
	void set_constant(const std::string& name, const T& value);
	template<typename T, std::enable_if_t<is_device_primitive_type(T), bool> = true>
	void push_constant(const std::string& name, const T& value);
	void clear_constants();

	enum _ConstantType {
		c_int32,
		c_uint32,
		c_float,
		c_vec2,
		c_vec3,
		c_vec4,
		c_ivec2,
		c_ivec3,
		c_ivec4,
		c_uvec2,
		c_uvec3,
		c_uvec4,
	};

private:

	std::shared_ptr<ComputeProgram> _compile_shaders(const std::vector<std::pair<std::string, std::string>>& arithmatic_preprocessing_defines);

	std::unordered_map<std::string, std::shared_ptr<ComputeProgram>> _preprocessors_to_shaders;

	template<typename target_t, typename source_t, typename operand_t,
		std::enable_if_t<is_device_data_type(target_t), bool > = true,
		std::enable_if_t<is_device_data_type(source_t), bool> = true,
		std::enable_if_t<is_device_data_type(operand_t), bool> = true>
	void _compute_general(	
		target_t& target,	const std::string& target_precise_type,
		source_t* source,	const std::string& source_precise_type,		bool source_interpolation, 					
		operand_t* operand, const std::string& operand_precise_type,	bool operand_interpolation, 					
		const std::string& operation, 					
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	template<typename target_t, typename source_t,
		std::enable_if_t<is_device_data_type(target_t),			bool > = true,
		std::enable_if_t<is_device_data_type(source_t),			bool> = true>
	void _compute_basic(	
		target_t& target,	const std::string& target_precise_type,
		source_t* source,	const std::string& source_precise_type,		bool source_interpolation, 					
		const std::string& operation, 					
		glm::ivec3 id_begin = glm::ivec3(0, 0, 0), glm::ivec3 id_end = glm::ivec3(0, 0, 0));

	enum _OperandType {
		texture1d,
		texture2d,
		texture3d,
		texture2d_array,
		texture_cubmap,
		buffer,
		none,
	};
	
	std::shared_ptr<Texture1D> operand_1dtex = nullptr;
	std::shared_ptr<Texture2D> operand_2dtex = nullptr;
	std::shared_ptr<Texture3D> operand_3dtex = nullptr;
	std::shared_ptr<Texture2DArray> operand_2dtexarray = nullptr;
	std::shared_ptr<TextureCubeMap> operand_cubemap = nullptr;
	std::shared_ptr<Buffer> operand_buffer = nullptr;

	_OperandType operand_type = none;
	std::string operand_buffer_precise_type = "";
	bool operand_interpolation = false;
	bool operand_enabled = false;
	
	template<typename operand_t,
		std::enable_if_t<is_device_data_type(operand_t), bool> = true>
	std::shared_ptr<operand_t> _get_operand_ptr();

	template<> std::shared_ptr<Texture1D>		_get_operand_ptr<Texture1D>() { return operand_1dtex; }
	template<> std::shared_ptr<Texture2D>		_get_operand_ptr<Texture2D>() { return operand_2dtex; }
	template<> std::shared_ptr<Texture3D>		_get_operand_ptr<Texture3D>() { return operand_3dtex; }
	template<> std::shared_ptr<Texture2DArray>	_get_operand_ptr<Texture2DArray>() { return operand_2dtexarray; }
	template<> std::shared_ptr<TextureCubeMap>	_get_operand_ptr<TextureCubeMap>() { return operand_cubemap; }
	template<> std::shared_ptr<Buffer>			_get_operand_ptr<Buffer>() { return operand_buffer; }

	
	struct _constant_with_info {
		uint32_t value[4];
		std::string name;
		_ConstantType constant_type;
	};

	std::vector<_constant_with_info> setted_constants;
	std::vector<_constant_with_info> pushed_constants;

	std::string _create_constant_definition();
	void _update_constant_uniforms(ComputeProgram& kernel);

	std::string setted_precomputation_statements;
	std::string pushed_precomputation_statements;
};


#include "GraphicsOperationTemplated.h"