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
const std::string MeshMaterial::ambient_occlusion_texture_uniform_name = "ambient_occlusion_texture";

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

    constexpr auto ambient_occlusion_ctf = Texture2D::ColorTextureFormat::R8;
    constexpr auto ambient_occlusion_cf = Texture2D::ColorFormat::RED;
    constexpr auto ambient_occlusion_t = Texture2D::Type::UNSIGNED_BYTE;

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

        material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0.5, 0.5, 1));

        material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 0.9f);

        material.metallic_texture = create_material_texture(model_mat.metallic_image, metallic_ctf, metallic_cf, metallic_t, 0.0f);

        material.ambient_occlusion_texture = create_material_texture(model_mat.ambient_occlusion_image, ambient_occlusion_ctf, ambient_occlusion_cf, ambient_occlusion_t, 1.0f);

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

    std::span<ModelMaterial::SingleMaterial> materials = model_material.get_materials();
    for (auto& material : materials) {
        if (material.albedo_image != nullptr)              
            image_to_texture[material.albedo_image]            = { albedo_ctf, albedo_cf, albedo_t, nullptr};
        if (material.normal_image != nullptr)              
            image_to_texture[material.normal_image]            = { normal_ctf, normal_cf, normal_t, nullptr };
        if (material.roughness_image != nullptr)           
            image_to_texture[material.roughness_image]         = { roughness_ctf, roughness_cf, roughness_t, nullptr};
        if (material.metallic_image != nullptr)            
            image_to_texture[material.metallic_image]          = { metallic_ctf, metallic_cf, metallic_t, nullptr };
        //if (material.height_image != nullptr)              
        //    image_to_texture[material.height_image]            = { albedo_ctf, albedo_cf, albedo_t, nullptr };
        if (material.ambient_occlusion_image != nullptr)   
            image_to_texture[material.ambient_occlusion_image] = { ambient_occlusion_ctf, ambient_occlusion_cf, ambient_occlusion_t, nullptr };
        //if (model.emissive_image != nullptr)            
        //    image_to_texture[model.emissive_image]          = { albedo_ctf, albedo_cf, albedo_t, nullptr };
    }

    for (auto& image_texture : image_to_texture) {
        _TextureLoadParams& texture_params = image_texture.second;
        texture_params.texture = create_material_texture_without_clear(image_texture.first, texture_params.ctf, texture_params.cf, texture_params.t);
    }

    for (auto& model_mat : model_material.get_materials()) {
        MeshMaterial::SingleMaterial material;
        
        if (model_mat.albedo_image != nullptr)
            material.albedo_texture = image_to_texture[model_mat.albedo_image].texture;
        else
            material.albedo_texture = create_material_texture(model_mat.albedo_image, albedo_ctf, albedo_cf, albedo_t, glm::vec4(model_mat.albedo));

        if (model_mat.normal_image != nullptr)
            material.normal_texture = image_to_texture[model_mat.normal_image].texture;
        else
            material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0.5, 0.5, 1));

        if (model_mat.roughness_image != nullptr)
            material.roughness_texture = image_to_texture[model_mat.roughness_image].texture;
        else
            material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 0.9f);

        if (model_mat.metallic_image != nullptr)
            material.metallic_texture = image_to_texture[model_mat.metallic_image].texture;
        else
            material.metallic_texture = create_material_texture(model_mat.metallic_image, metallic_ctf, metallic_cf, metallic_t, 0.0f);
        
        if (model_mat.ambient_occlusion_image != nullptr)
            material.ambient_occlusion_texture = image_to_texture[model_mat.ambient_occlusion_image].texture;
        else
            material.ambient_occlusion_texture = create_material_texture(model_mat.ambient_occlusion_image, ambient_occlusion_ctf, ambient_occlusion_cf, ambient_occlusion_t, 1.0f);

        add_material(material);
    }
}

void MeshMaterial::load_material(ModelMaterial&& model_material)
{
    struct _TextureLoadParams {
        Texture2D::ColorTextureFormat ctf = Texture2D::ColorTextureFormat::RGB8;
        Texture2D::ColorFormat cf = Texture2D::ColorFormat::RGB;
        Texture2D::Type t = Texture2D::Type::UNSIGNED_BYTE;
        std::shared_ptr<Texture2D> texture = nullptr;
    };

    std::unordered_map<std::shared_ptr<Image>, _TextureLoadParams> image_to_texture;

    std::span<ModelMaterial::SingleMaterial> materials = model_material.get_materials();
    for (auto& material : materials) {
        if (material.albedo_image != nullptr)
            image_to_texture[material.albedo_image] = { albedo_ctf, albedo_cf, albedo_t, nullptr };
        if (material.normal_image != nullptr)
            image_to_texture[material.normal_image] = { normal_ctf, normal_cf, normal_t, nullptr };
        if (material.roughness_image != nullptr)
            image_to_texture[material.roughness_image] = { roughness_ctf, roughness_cf, roughness_t, nullptr };
        if (material.metallic_image != nullptr)
            image_to_texture[material.metallic_image] = { metallic_ctf, metallic_cf, metallic_t, nullptr };
        //if (material.height_image != nullptr)              
        //    image_to_texture[material.height_image]            = { albedo_ctf, albedo_cf, albedo_t, nullptr };
        if (material.ambient_occlusion_image != nullptr)
            image_to_texture[material.ambient_occlusion_image] = { ambient_occlusion_ctf, ambient_occlusion_cf, ambient_occlusion_t, nullptr };
        //if (model.emissive_image != nullptr)            
        //    image_to_texture[model.emissive_image]          = { albedo_ctf, albedo_cf, albedo_t, nullptr };
    }

    for (auto& image_texture : image_to_texture) {
        _TextureLoadParams& texture_params = image_texture.second;
        texture_params.texture = create_material_texture_without_clear(image_texture.first, texture_params.ctf, texture_params.cf, texture_params.t);
    }

    for (auto& model_mat : model_material.get_materials()) {
        MeshMaterial::SingleMaterial material;

        if (model_mat.albedo_image != nullptr)
            material.albedo_texture = image_to_texture[model_mat.albedo_image].texture;
        else
            material.albedo_texture = create_material_texture(model_mat.albedo_image, albedo_ctf, albedo_cf, albedo_t, glm::vec4(model_mat.albedo));

        if (model_mat.normal_image != nullptr)
            material.normal_texture = image_to_texture[model_mat.normal_image].texture;
        else
            material.normal_texture = create_material_texture(model_mat.normal_image, normal_ctf, normal_cf, normal_t, glm::vec3(0.5, 0.5, 1));

        if (model_mat.roughness_image != nullptr)
            material.roughness_texture = image_to_texture[model_mat.roughness_image].texture;
        else
            material.roughness_texture = create_material_texture(model_mat.roughness_image, roughness_ctf, roughness_cf, roughness_t, 0.9f);

        if (model_mat.metallic_image != nullptr)
            material.metallic_texture = image_to_texture[model_mat.metallic_image].texture;
        else
            material.metallic_texture = create_material_texture(model_mat.metallic_image, metallic_ctf, metallic_cf, metallic_t, 0.0f);

        if (model_mat.ambient_occlusion_image != nullptr)
            material.ambient_occlusion_texture = image_to_texture[model_mat.ambient_occlusion_image].texture;
        else
            material.ambient_occlusion_texture = create_material_texture(model_mat.ambient_occlusion_image, ambient_occlusion_ctf, ambient_occlusion_cf, ambient_occlusion_t, 1.0f);

        model_mat.albedo_image = nullptr;
        model_mat.normal_image = nullptr;
        model_mat.roughness_image = nullptr;
        model_mat.metallic_image = nullptr;
        model_mat.height_image = nullptr;
        model_mat.ambient_occlusion_image = nullptr;
        model_mat.emissive_image = nullptr;

        add_material(material);
    }
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
    program.update_uniform(ambient_occlusion_texture_uniform_name, *single_material->ambient_occlusion_texture);
    //...
}
