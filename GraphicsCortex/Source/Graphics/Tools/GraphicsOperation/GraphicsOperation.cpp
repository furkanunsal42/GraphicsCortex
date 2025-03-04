#include "GraphicsOperation.h"

namespace {
	
	std::string concatinate_preprocessor_values(const std::vector<std::pair<std::string, std::string>>& preprocessor) {
		std::string concatinated_value;

		for (auto& pair : preprocessor)
			concatinated_value += pair.first + "!!->!!" + pair.second;
		
		return concatinated_value;
	}

	std::string constant_name_to_glsl_type(const GraphicsOperation::_ConstantType& type) {
		switch (type)
		{
		case GraphicsOperation::_ConstantType::c_int32	: return "int";
		case GraphicsOperation::_ConstantType::c_uint32	: return "uint";
		case GraphicsOperation::_ConstantType::c_float	: return "float";
		case GraphicsOperation::_ConstantType::c_vec2	: return "vec2";
		case GraphicsOperation::_ConstantType::c_vec3	: return "vec3";
		case GraphicsOperation::_ConstantType::c_vec4	: return "vec4";
		case GraphicsOperation::_ConstantType::c_ivec2	: return "ivec2";
		case GraphicsOperation::_ConstantType::c_ivec3	: return "ivec3";
		case GraphicsOperation::_ConstantType::c_ivec4	: return "ivec4";
		case GraphicsOperation::_ConstantType::c_uvec2	: return "uvec2";
		case GraphicsOperation::_ConstantType::c_uvec3	: return "uvec3";
		case GraphicsOperation::_ConstantType::c_uvec4	: return "uvec4";
		}

		ASSERT(false);
	}

	void update_uniform_by_constant_type(ComputeProgram& kernel, const std::string& uniform_name, uint32_t* value, const GraphicsOperation::_ConstantType& type) {
		switch (type)
		{
		case GraphicsOperation::_ConstantType::c_int32:		kernel.update_uniform(uniform_name, *(int32_t*)value); return;
		case GraphicsOperation::_ConstantType::c_uint32:	kernel.update_uniform(uniform_name, *(uint32_t*)value); return;
		case GraphicsOperation::_ConstantType::c_float:		kernel.update_uniform(uniform_name, *(float*)value); return;
		case GraphicsOperation::_ConstantType::c_vec2:		kernel.update_uniform(uniform_name, *(glm::vec2*)value); return;
		case GraphicsOperation::_ConstantType::c_vec3:		kernel.update_uniform(uniform_name, *(glm::vec3*)value); return;
		case GraphicsOperation::_ConstantType::c_vec4:		kernel.update_uniform(uniform_name, *(glm::vec4*)value); return;
		case GraphicsOperation::_ConstantType::c_ivec2:		kernel.update_uniform(uniform_name, *(glm::ivec2*)value); return;
		case GraphicsOperation::_ConstantType::c_ivec3:		kernel.update_uniform(uniform_name, *(glm::ivec3*)value); return;
		case GraphicsOperation::_ConstantType::c_ivec4:		kernel.update_uniform(uniform_name, *(glm::ivec4*)value); return;
		case GraphicsOperation::_ConstantType::c_uvec2:		kernel.update_uniform(uniform_name, *(glm::uvec2*)value); return;
		case GraphicsOperation::_ConstantType::c_uvec3:		kernel.update_uniform(uniform_name, *(glm::uvec3*)value); return;
		case GraphicsOperation::_ConstantType::c_uvec4:		kernel.update_uniform(uniform_name, *(glm::uvec4*)value); return;
		}

		ASSERT(false);
	}
}

void GraphicsOperation::clear_constants()
{
	setted_constants.clear();
	pushed_constants.clear();
}

void GraphicsOperation::clear_all_state()
{
	clear_operands();
	clear_constants();
	clear_precomputation_statements();
}

void GraphicsOperation::clear()
{
	clear_all_state();
	_preprocessors_to_shaders.clear();
}

std::shared_ptr<ComputeProgram> GraphicsOperation::_compile_shaders(const std::vector<std::pair<std::string, std::string>>& arithmatic_preprocessing_defines) {
	std::string concatinated_value = concatinate_preprocessor_values(arithmatic_preprocessing_defines);

	auto iterator = _preprocessors_to_shaders.find(concatinated_value);
	if (iterator == _preprocessors_to_shaders.end()) {
		std::shared_ptr<ComputeProgram>cp_data_arithmatic = 
			std::make_shared<ComputeProgram>
			(Shader("../GraphicsCortex/Source/GLSL/TextureArithmatic/graphics_operation.comp"), arithmatic_preprocessing_defines);
		
		_preprocessors_to_shaders[concatinated_value] = cp_data_arithmatic;
		return cp_data_arithmatic;
	}

	return iterator->second;
}

std::string GraphicsOperation::_create_constant_definition()
{
	std::string definition;
	for (_constant_with_info& constant : setted_constants)
		definition += "uniform " + constant_name_to_glsl_type(constant.constant_type) + " " + constant.name + ";";
	for (_constant_with_info& constant : pushed_constants)
		definition += "uniform " + constant_name_to_glsl_type(constant.constant_type) + " " + constant.name + ";";
	return definition;
}

void GraphicsOperation::_update_constant_uniforms(ComputeProgram& kernel)
{
	for (_constant_with_info& constant : setted_constants)
		update_uniform_by_constant_type(kernel, constant.name, constant.value, constant.constant_type);
	for (_constant_with_info& constant : pushed_constants)
		update_uniform_by_constant_type(kernel, constant.name, constant.value, constant.constant_type);
}

void GraphicsOperation::set_operand(std::shared_ptr<Texture1D> operand, bool operand_interpolation) {
	clear_operands();
	operand_type = texture1d;
	operand_1dtex = operand;
	operand_buffer_precise_type = "";
	this->operand_interpolation = operand_interpolation;
	operand_enabled = true;
}
void GraphicsOperation::set_operand(std::shared_ptr<Texture2D> operand, bool operand_interpolation) {
	clear_operands();
	operand_type = texture2d;
	operand_2dtex = operand;
	operand_buffer_precise_type = "";
	this->operand_interpolation = operand_interpolation;
	operand_enabled = true;
}
void GraphicsOperation::set_operand(std::shared_ptr<Texture3D> operand, bool operand_interpolation) {
	clear_operands();
	operand_type = texture3d;
	operand_3dtex = operand;
	operand_buffer_precise_type = "";
	this->operand_interpolation = operand_interpolation;
	operand_enabled = true;
}
void GraphicsOperation::set_operand(std::shared_ptr<Texture2DArray> operand, bool operand_interpolation) {
	clear_operands();
	operand_type = texture2d_array;
	operand_2dtexarray = operand;
	operand_buffer_precise_type = "";
	this->operand_interpolation = operand_interpolation;
	operand_enabled = true;
}
void GraphicsOperation::set_operand(std::shared_ptr<TextureCubeMap> operand, bool operand_interpolation) {
	clear_operands();
	operand_type = texture_cubmap;
	operand_cubemap = operand;
	operand_buffer_precise_type = "";
	this->operand_interpolation = operand_interpolation;
	operand_enabled = true;
}
void GraphicsOperation::set_operand(std::shared_ptr<Buffer> operand, std::string operand_buffer_precise_type) {
	clear_operands();
	operand_type = buffer;
	operand_buffer = operand;
	this->operand_buffer_precise_type = operand_buffer_precise_type;
	this->operand_interpolation = false;
	operand_enabled = true;
}

void GraphicsOperation::clear_operands() {
	operand_1dtex = nullptr;
	operand_2dtex = nullptr;
	operand_3dtex = nullptr;
	operand_2dtexarray = nullptr;
	operand_cubemap = nullptr;
	operand_buffer = nullptr;
	operand_buffer_precise_type = "";
	this->operand_interpolation = false;
	operand_enabled = false;
	operand_type = none;
}

void GraphicsOperation::set_precomputation_statement(const std::string& precomputation_statement) {
	this->setted_precomputation_statements = precomputation_statement;
}

void GraphicsOperation::push_precomputation_statement(const std::string& precomputation_statement) {
	this->pushed_precomputation_statements = precomputation_statement;
}

void GraphicsOperation::clear_precomputation_statements() {
	this->setted_precomputation_statements.clear();
	this->pushed_precomputation_statements.clear();
}