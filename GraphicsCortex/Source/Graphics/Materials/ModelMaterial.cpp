#include "ModelMaterial.h"

bool ModelMaterial::does_material_exist(material_t material)
{
    return material >= 0 && material < materials.size();
}

size_t ModelMaterial::get_material_count()
{
    return materials.size();
}

material_t ModelMaterial::add_material(SingleMaterial& single_material)
{
    materials.push_back(single_material);
    return materials.size() - 1;
}

material_t ModelMaterial::add_material(SingleMaterial&& single_material)
{
    materials.emplace_back(single_material);
    return materials.size() - 1;
}

ModelMaterial::SingleMaterial* ModelMaterial::get_material(material_t material)
{
    if (!does_material_exist(material))
        return nullptr;

    return &materials[material];
}

std::span<ModelMaterial::SingleMaterial> ModelMaterial::get_materials()
{
    return std::span<ModelMaterial::SingleMaterial>(materials);
}

void ModelMaterial::clear()
{
    materials.clear();
}
