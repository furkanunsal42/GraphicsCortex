#include "MeshMaterial.h"

#include <functional>
#include <iostream>

#include "glm.hpp"

#include "Debuger.h"
#include "Image.h"
#include "Texture2D.h"
#include "ShaderCompiler.h"

const std::string MeshMaterial::albedo_texture_uniform_name = "albedo_texture";
const std::string MeshMaterial::normal_texture_uniform_name = "normal_texture";
const std::string MeshMaterial::roughness_texture_uniform_name = "roughness_texture";
const std::string MeshMaterial::metallic_texture_uniform_name = "metallic_texture";

namespace {

    constexpr auto albedo_ctf = Texture2D::ColorTextureFormat::RGBA8;
    constexpr auto albedo_cf = Texture2D::ColorFormat::RGBA;
    constexpr auto albedo_t = Texture2D::Type::UNSIGNED_BYTE;

    constexpr auto normal_ctf = Texture2D::ColorTextureFormat::RGB8;
    constexpr auto normal_cf = Texture2D::ColorFormat::RGB;
    constexpr auto normal_t = Texture2D::Type::UNSIGNED_BYTE;

    constexpr auto roughness_ctf = Texture2D::ColorTextureFormat::R8;
    constexpr auto roughness_cf = Texture2D::ColorFormat::RED;
    constexpr auto roughness_t = Texture2D::Type::UNSIGNED_BYTE;

    constexpr auto metallic_ctf = Texture2D::ColorTextureFormat::R8;
    constexpr auto metallic_cf = Texture2D::ColorFormat::RED;
    constexpr auto metallic_t = Texture2D::Type::UNSIGNED_BYTE;

    std::shared_ptr<Texture2D> create_material_texture_without_clear(const std::shared_ptr<Image> image, Texture2D::ColorTextureFormat ctf, Texture2D::ColorFormat cf, Texture2D::Type t) {

        std::shared_ptr<Texture2D> texture;

        if (image != nullptr) {
            const Image& albedo_image = *image;
            glm::ivec2 resolution = glm::ivec2(albedo_image.get_width(), albedo_image.get_height());

            uint32_t mipmap_count = std::max(1.0, std::min(std::log2(resolution.x), std::log2(resolution.y)));
            texture = std::make_shared<Texture2D>(resolution.x, resolution.y, ctf, mipmap_count, 0, 0);
            texture->wrap_u = Texture2D::WrapMode::REPEAT;
            texture->wrap_v = Texture2D::WrapMode::REPEAT;
            texture->load_data_with_mipmaps(albedo_image, cf, t);
        }
        else {
            texture = std::make_shared<Texture2D>(1, 1, ctf, 1, 0, 0);
            texture->wrap_u = Texture2D::WrapMode::REPEAT;
            texture->wrap_v = Texture2D::WrapMode::REPEAT;
            //texture->clear(clear_value);
        }


        return texture;
    }

    template<typename T>
    std::shared_ptr<Texture2D> create_material_texture(const std::shared_ptr<Image> image, Texture2D::ColorTextureFormat ctf, Texture2D::ColorFormat cf, Texture2D::Type t, T clear_value) {

        std::shared_ptr<Texture2D> texture;

        if (image != nullptr) {
            const Image& albedo_image = *image;
            glm::ivec2 resolution = glm::ivec2(albedo_image.get_width(), albedo_image.get_height());

            uint32_t mipmap_count = std::max(1.0, std::min(std::log2(resolution.x), std::log2(resolution.y)));
            texture = std::make_shared<Texture2D>(resolution.x, resolution.y, ctf, mipmap_count, 0, 0);
            texture->wrap_u = Texture2D::WrapMode::REPEAT;
            texture->wrap_v = Texture2D::WrapMode::REPEAT;
            texture->load_data_with_mipmaps(albedo_image, cf, t);
        }
        else {
            texture = std::make_shared<Texture2D>(1, 1, ctf, 1, 0, 0);
            texture->wrap_u = Texture2D::WrapMode::REPEAT;
            texture->wrap_v = Texture2D::WrapMode::REPEAT;
            texture->clear(clear_value);
        }


        return texture;
    }

    MeshMaterial::SingleMaterial create_single_mesh_material(ModelMaterial::SingleMaterial& model_mat) {
        MeshMaterial::SingleMaterial material;

        material.albedo_texture = create_material_texture(model_mat.albedo_image, albedo_ctf, albedo_cf, albedo_t, model_mat.albedo);

        material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0, 0, 0.5));

        material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 1.0f);

        material.metallic_texture = create_material_texture(model_mat.metallic_image, metallic_ctf, metallic_cf, metallic_t, glm::vec3(0));

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

    struct _TextureLoadParams {
        Texture2D::ColorTextureFormat ctf   = Texture2D::ColorTextureFormat::RGB8;
        Texture2D::ColorFormat cf           = Texture2D::ColorFormat::RGB;
        Texture2D::Type t                   = Texture2D::Type::UNSIGNED_BYTE;
        std::shared_ptr<Texture2D> texture  = nullptr;
    };

    std::unordered_map<std::shared_ptr<Image>, _TextureLoadParams> image_to_texture;

    std::span<ModelMaterial::SingleMaterial> models = model_material.get_materials();
    for (auto& model : models) {
        if (model.albedo_image != nullptr)              
            image_to_texture[model.albedo_image]            = { albedo_ctf, albedo_cf, albedo_t, nullptr};
        if (model.normal_image != nullptr)              
            image_to_texture[model.normal_image]            = { normal_ctf, normal_cf, normal_t, nullptr };
        if (model.roughness_image != nullptr)           
            image_to_texture[model.roughness_image]         = { roughness_ctf, roughness_cf, roughness_t, nullptr};
        if (model.metallic_image != nullptr)            
            image_to_texture[model.metallic_image]          = { metallic_ctf, metallic_cf, metallic_t, nullptr };
        //if (model.height_image != nullptr)              
        //    image_to_texture[model.height_image]            = { albedo_ctf, albedo_cf, albedo_t, nullptr };
        //if (model.ambient_occlusion_image != nullptr)   
        //    image_to_texture[model.ambient_occlusion_image] = { albedo_ctf, albedo_cf, albedo_t, nullptr };
        //if (model.emissive_image != nullptr)            
        //    image_to_texture[model.emissive_image]          = { albedo_ctf, albedo_cf, albedo_t, nullptr };
    }

    for (auto& image_texture : image_to_texture) {
        const std::shared_ptr<Image> image = image_texture.first;
        _TextureLoadParams& texture_params = image_texture.second;
        texture_params.texture = create_material_texture_without_clear(image, texture_params.ctf, texture_params.cf, texture_params.t);
    }

    for (auto& model_mat : model_material.get_materials()) {
        MeshMaterial::SingleMaterial material;
        
        if (material.albedo_texture != nullptr)
            material.albedo_texture = image_to_texture[model_mat.albedo_image].texture;
        else
            material.albedo_texture = create_material_texture(model_mat.albedo_image, albedo_ctf, albedo_cf, albedo_t, glm::vec4(model_mat.albedo));

        if (material.normal_texture != nullptr)
            material.normal_texture = image_to_texture[model_mat.normal_image].texture;
        else
            material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0, 0, 0.5));

        if (material.roughness_texture != nullptr)
            material.roughness_texture = image_to_texture[model_mat.roughness_image].texture;
        else
            material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 1.0f);

        if (material.metallic_texture != nullptr)
            material.metallic_texture = image_to_texture[model_mat.metallic_image].texture;
        else
            material.metallic_texture = create_material_texture(model_mat.metallic_image, metallic_ctf, metallic_cf, metallic_t, 0.0f);

        add_material(std::move(material));
    }
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

void MeshMaterial::update_uniforms(Program& program, material_t material_index)
{
    if (!does_material_exist(material_index)) {
        std::cout << "[OpenGL Error] MeshMaterial::update_uniforms() is called with material_index=" << material_index << " but given material doesn't exist" << std::endl;
        ASSERT(false);
    }

    SingleMaterial* single_material = get_material(material_index);
    
    program.update_uniform(albedo_texture_uniform_name, *single_material->albedo_texture);
    program.update_uniform(normal_texture_uniform_name, *single_material->normal_texture);
    program.update_uniform(roughness_texture_uniform_name, *single_material->roughness_texture);
    program.update_uniform(metallic_texture_uniform_name, *single_material->metallic_texture);
    //...
}
