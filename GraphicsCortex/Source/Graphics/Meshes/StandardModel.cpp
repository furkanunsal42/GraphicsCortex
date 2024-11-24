#include "StandardModel.h"

Model2::Model2(const std::string& filepath)
{
	load_model(filepath);
}

void Model2::clear()
{
	_sub_model_tree.clear();
}
