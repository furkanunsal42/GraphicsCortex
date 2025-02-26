#include "MeshMaterial.h"

#include <functional>

#include "glm.hpp"

#include "Image.h"
#include "Texture2D.h"

namespace {

    template<typename T>
    std::shared_ptr<Texture2D> create_material_texture(const std::optional<Image>& image, Texture2D::ColorTextureFormat ctf, Texture2D::ColorFormat cf, Texture2D::Type t, T clear_value) {

        std::shared_ptr<Texture2D> texture;

        if (image.has_value()) {
            const Image& albedo_image = image.value();
            glm::ivec2 resolution = glm::ivec2(albedo_image.get_width(), albedo_image.get_height());

            texture = std::make_shared<Texture2D>(resolution.x, resolution.y, ctf, 1, 0, 0);
            texture->load_data(albedo_image, cf, t, 0);
        }
        else {
            texture = std::make_shared<Texture2D>(1, 1, ctf, 1, 0, 0);
            texture->clear(clear_value);
        }
        return texture;
    }

    MeshMaterial::SingleMaterial create_single_mesh_material(ModelMaterial::SingleMaterial& model_mat) {
        MeshMaterial::SingleMaterial material;

        auto albedo_ctf = Texture2D::ColorTextureFormat::RGBA8;
        auto albedo_cf = Texture2D::ColorFormat::RGBA;
        auto albedo_t = Texture2D::Type::UNSIGNED_BYTE;
        material.albedo_texture = create_material_texture(model_mat.albedo_image, albedo_ctf, albedo_cf, albedo_t, model_mat.albedo);


        auto normal_ctf = Texture2D::ColorTextureFormat::RGB8;
        auto normal_cf = Texture2D::ColorFormat::RGB;
        auto normal_t = Texture2D::Type::UNSIGNED_BYTE;
        material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0, 0, 0.5));


        auto roughness_ctf = Texture2D::ColorTextureFormat::R8;
        auto roughness_cf = Texture2D::ColorFormat::RED;
        auto roughness_t = Texture2D::Type::UNSIGNED_BYTE;
        material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 1.0f);


        auto metalness_ctf = Texture2D::ColorTextureFormat::R8;
        auto metalness_cf = Texture2D::ColorFormat::RED;
        auto metalness_t = Texture2D::Type::UNSIGNED_BYTE;
        material.metalness_texture = create_material_texture(model_mat.metalness_image, metalness_ctf, metalness_cf, metalness_t, glm::vec3(0));


        //auto specular_ctf   = Texture2D::ColorTextureFormat::R8;
        //auto specular_cf    = Texture2D::ColorFormat::RED;
        //auto specular_t     = Texture2D::Type::UNSIGNED_BYTE;
        //material.specular_texture = create_material_texture(model_mat.specular_image, specular_ctf, specular_cf, specular_t, glm::vec3(0));
        //
        //
        //auto height_ctf     = Texture2D::ColorTextureFormat::R8;
        //auto height_cf      = Texture2D::ColorFormat::RED;
        //auto height_t       = Texture2D::Type::UNSIGNED_BYTE;
        //material.height_texture = create_material_texture(model_mat.height_image, height_ctf, height_cf, height_t, glm::vec3(0));
        //
        //
        //auto opacity_ctf    = Texture2D::ColorTextureFormat::R8;
        //auto opacity_cf     = Texture2D::ColorFormat::RED;
        //auto opacity_t      = Texture2D::Type::UNSIGNED_BYTE;
        //material.opacity_texture = create_material_texture(model_mat.opacity_image, opacity_ctf, opacity_cf, opacity_t, glm::vec3(0));
        //
        //
        //auto ambiant_occlusion_ctf  = Texture2D::ColorTextureFormat::R8;
        //auto ambiant_occlusion_cf   = Texture2D::ColorFormat::RED;
        //auto ambiant_occlusion_t    = Texture2D::Type::UNSIGNED_BYTE;
        //material.ambiant_occlusion_texture = create_material_texture(model_mat.ambiant_occlusion_image, ambiant_occlusion_ctf, ambiant_occlusion_cf, ambiant_occlusion_t, glm::vec3(0));
        //
        //
        //auto refrection_ctf     = Texture2D::ColorTextureFormat::R8;
        //auto refrection_cf      = Texture2D::ColorFormat::RED;
        //auto refrection_t       = Texture2D::Type::UNSIGNED_BYTE;
        //material.refrection_texture = create_material_texture(model_mat.refrection_image, refrection_ctf, refrection_cf, refrection_t, glm::vec3(0));
        //
        //
        //auto emissive_ctf   = Texture2D::ColorTextureFormat::R8;
        //auto emissive_cf    = Texture2D::ColorFormat::RED;
        //auto emissive_t     = Texture2D::Type::UNSIGNED_BYTE;
        //material.emissive_texture = create_material_texture(model_mat.emissive_image, emissive_ctf, emissive_cf, emissive_t, glm::vec3(0));

        return material;
    }

    MeshMaterial::SingleMaterial create_single_mesh_material(ModelMaterial::SingleMaterial&& model_mat) {
        return create_single_mesh_material((ModelMaterial::SingleMaterial&)model_mat);
    }

}

void MeshMaterial::load_material(ModelMaterial& model_material)
{
    for (auto& single_model_material : model_material.get_materials())
        add_material(create_single_mesh_material(single_model_material));
}

void MeshMaterial::load_material(ModelMaterial&& model_material)
{
    load_material((ModelMaterial&)model_material);
}

void MeshMaterial::load_material(ModelMaterial::SingleMaterial& model_mat)
{
    clear();

    SingleMaterial single_material = create_single_mesh_material(model_mat);
    add_material(single_material);
}

void MeshMaterial::load_material(ModelMaterial::SingleMaterial&& single_model_material)
{
    load_material((ModelMaterial::SingleMaterial&)single_model_material);
}

bool MeshMaterial::does_material_exist(material_t material)
{
    return material >= 0 && material < materials.size();
}

size_t MeshMaterial::get_material_count()
{
    return materials.size();
}

material_t MeshMaterial::add_material(MeshMaterial::SingleMaterial& single_material)
{
    materials.push_back(single_material);
    return materials.size() - 1;
}

material_t MeshMaterial::add_material(MeshMaterial::SingleMaterial&& single_material)
{
    materials.emplace_back(single_material);
    return materials.size() - 1;
}

MeshMaterial::SingleMaterial* MeshMaterial::get_material(material_t material)
{
    if (!does_material_exist(material))
        return nullptr;

    return &materials[material];
}

std::span<MeshMaterial::SingleMaterial> MeshMaterial::get_materials()
{
    return std::span<MeshMaterial::SingleMaterial>(materials);
}

void MeshMaterial::clear()
{
    materials.clear();
}
