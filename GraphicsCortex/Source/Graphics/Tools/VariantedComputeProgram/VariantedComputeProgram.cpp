#include "VariantedComputeProgram.h"

VariantedComputeProgram::VariantedComputeProgram(size_t maximum_variant_count) : 
	maximum_variant_count(maximum_variant_count)
{
		
}

VariantedComputeProgram::VariantedComputeProgram(Shader shader, size_t maximum_variant_count) : 
	maximum_variant_count(maximum_variant_count), shader(std::make_unique<Shader>(shader))
{
	
}

void VariantedComputeProgram::set_shader(Shader shader)
{
	release_all_variants();
	this->shader = std::make_unique<Shader>(shader);
}

void VariantedComputeProgram::begin_variant()
{
	current_macros.clear();
}

void VariantedComputeProgram::variant_define(const std::string& macro_name, const std::string& macro_value)
{
	current_macros[macro_name] = macro_value;
}

std::shared_ptr<ComputeProgram> VariantedComputeProgram::get_current_variant()
{
	bool shader_variant_exists		= programs.find(current_macros) != programs.end();
	size_t variant_count_overflow	= (maximum_variant_count == 0 || programs.size() <= maximum_variant_count) ? 
										0 : programs.size() - maximum_variant_count;

	if (!shader_variant_exists) {
		for (size_t i = 0; i < variant_count_overflow; i++)
			programs.erase(programs.begin());

		programs[current_macros] = std::make_shared<ComputeProgram>(*shader, current_macros.definitions);
	}

	return programs.at(current_macros);
}

void VariantedComputeProgram::release_current_variant()
{
	programs.erase(current_macros);
}

void VariantedComputeProgram::release_all_variants()
{
	programs.clear();
}

std::string& VariantDefinitions::operator[](const std::string& macro_name)
{
	const_iterator it = find(macro_name);
	
	if (it != end())
		return (std::string&)it->second;

	definitions.push_back(std::pair<std::string, std::string>(macro_name, std::string()));
	return definitions.back().second;
}

std::string& VariantDefinitions::at(const std::string& macro_name)
{
	const_iterator it = find(macro_name);

	if (it != end())
		return (std::string&)it->second;
	
	else {
		std::cout << "[VariantedComputeProgram Error] VariantDefinition::at() is called but macro doesn't exist" << std::endl;
		ASSERT(false);
	}
}

const std::string& VariantDefinitions::at(const std::string& macro_name) const
{
	return at(macro_name);
}

VariantDefinitions::const_iterator VariantDefinitions::find(const std::string& macro_name) const
{
	for (size_t i = 0; i < definitions.size(); i++)
		if (definitions[i].first == macro_name) 
			return (definitions.begin() + i);
 
	return definitions.end();
}

VariantDefinitions::const_iterator VariantDefinitions::begin() const{
	return definitions.begin();
}

VariantDefinitions::const_iterator VariantDefinitions::end() const {
	return definitions.end();
}

void VariantDefinitions::clear() {
	definitions.clear();
}

size_t VariantDefinitions::size() const {
	return definitions.size();
}

bool operator==(const VariantDefinitions& A, const VariantDefinitions& B)
{
	return A.definitions == B.definitions;
}
