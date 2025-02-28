#include "Asset.h"
#include <vector>
#include <functional>
#include <unordered_map>

#include "glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include <assimp/postprocess.h>

#include <mutex>

Asset::Asset(const std::filesystem::path& asset_path, const AssetImportDescription& properties)
{
    filepath = asset_path;

    this->importer = new Assimp::Importer();

    scene = ((Assimp::Importer*)this->importer)->ReadFile(asset_path.string(),
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (scene == nullptr ) {
        if (!std::filesystem::exists(filepath)) {
            std::wcout << "[AssetImporter Error] loading failed for asset because file does not exist at : " << std::filesystem::absolute(filepath) << std::endl;
            ASSERT(false);
        }
        
        std::wcout << "[AssetImporter Error] loading failed for asset at : " << std::filesystem::absolute(filepath) << std::endl;
        ASSERT(false);
    }

}

Asset::~Asset()
{
    ((Assimp::Importer*)this->importer)->FreeScene();
    delete (Assimp::Importer*)this->importer;
}

namespace {
    SingleModel assimp_load_singlemodel(const aiScene* scene, uint32_t submodel_index, const std::filesystem::path& path) {
        
        if (submodel_index < 0 || submodel_index >= scene->mNumMeshes) {
            std::cout << "[AssetImporter Error] submodel_index (" << submodel_index << ") is not present in asset at : " << path << std::endl;
            ASSERT(false);
        }

        
        SingleModel single_model;

        if (scene->mMeshes[submodel_index]->mVertices != nullptr)
            single_model.verticies = std::vector<glm::vec3>(
                (glm::vec3*)scene->mMeshes[submodel_index]->mVertices,
                (glm::vec3*)scene->mMeshes[submodel_index]->mVertices + scene->mMeshes[submodel_index]->mNumVertices
            );

        if (scene->mMeshes[submodel_index]->mNormals != nullptr)
            single_model.vertex_normals = std::vector<glm::vec3>(
                (glm::vec3*)scene->mMeshes[submodel_index]->mNormals,
                (glm::vec3*)scene->mMeshes[submodel_index]->mNormals + scene->mMeshes[submodel_index]->mNumVertices
            );

        if (scene->mMeshes[submodel_index]->mTangents != nullptr)
            single_model.vertex_tangents = std::vector<glm::vec3>(
                (glm::vec3*)scene->mMeshes[submodel_index]->mTangents,
                (glm::vec3*)scene->mMeshes[submodel_index]->mTangents + scene->mMeshes[submodel_index]->mNumVertices
            );

        if (scene->mMeshes[submodel_index]->mTextureCoords[0] != nullptr) {
            single_model.texture_coordinates_0.reserve(scene->mMeshes[submodel_index]->mNumVertices);
            for (size_t i = 0; i < scene->mMeshes[submodel_index]->mNumVertices; i++) {
                single_model.texture_coordinates_0.push_back(
                    glm::vec2(
                        scene->mMeshes[submodel_index]->mTextureCoords[0][i].x,
                        scene->mMeshes[submodel_index]->mTextureCoords[0][i].y
                    )
                );
            }
        }

        if (scene->mMeshes[submodel_index]->mTextureCoords[1] != nullptr) {
            single_model.texture_coordinates_1.reserve(scene->mMeshes[submodel_index]->mNumVertices);
            for (size_t i = 0; i < scene->mMeshes[submodel_index]->mNumVertices; i++) {
                single_model.texture_coordinates_1.push_back(
                    glm::vec2(
                        scene->mMeshes[submodel_index]->mTextureCoords[1][i].x,
                        scene->mMeshes[submodel_index]->mTextureCoords[1][i].y
                    )
                );
            }
        }

        if (scene->mMeshes[submodel_index]->mColors[0] != nullptr)
            single_model.vertex_colors = std::vector<glm::vec4>(
                (glm::vec4*)scene->mMeshes[submodel_index]->mColors[0],
                (glm::vec4*)scene->mMeshes[submodel_index]->mColors[0] + scene->mMeshes[submodel_index]->mNumVertices
            );

        //if (scene->mNumMeshes > 0) single_model.bone_indicies
        //if (scene->mNumMeshes > 0) single_model.bone_weights

        size_t primitive_count = scene->mMeshes[submodel_index]->mNumFaces;

        PrimitiveType primitive_type;
        switch (scene->mMeshes[submodel_index]->mPrimitiveTypes) {
        case aiPrimitiveType::aiPrimitiveType_POINT:    primitive_type = point; break;
        case aiPrimitiveType::aiPrimitiveType_LINE:     primitive_type = line; break;
        case aiPrimitiveType::aiPrimitiveType_TRIANGLE: primitive_type = triangle; break;
        default:
            std::wcout << "[AssetImporter Error] primitive type is not supported for mesh at: " << path << std::endl;
            ASSERT(false);
            break;
        }

        single_model.indicies.reserve(get_PrimitiveType_index_count(primitive_type)* primitive_count);

        for (size_t face = 0; face < scene->mMeshes[submodel_index]->mNumFaces; face++) {
            if (scene->mMeshes[submodel_index]->mFaces[face].mNumIndices != get_PrimitiveType_index_count(primitive_type)) {
                std::wcout << "[AssetImpoterter Error] multiple primitives per face are not supported for mesh at: " << path << std::endl;
                ASSERT(false);
            }

            for (size_t index = 0; index < scene->mMeshes[submodel_index]->mFaces[face].mNumIndices; index++) {
                single_model.indicies.push_back(scene->mMeshes[submodel_index]->mFaces[face].mIndices[index]);
            }
        }

        return single_model;
    }

    inline glm::mat4 mat4_assimp_to_glm(const aiMatrix4x4& from)
    {
        glm::mat4 to;


        to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
        to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
        to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
        to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;

        return to;
    }

    void assimp_traverse_nodes(const aiScene* scene, const std::function<void(aiNode*, glm::mat4&)>& lambda) {

        std::queue<glm::mat4> transforms;
        std::queue<aiNode*> scene_nodes;

        scene_nodes.push(scene->mRootNode);
        transforms.push(mat4_assimp_to_glm(scene_nodes.front()->mTransformation));

        while (scene_nodes.size() > 0) {
            aiNode* node = scene_nodes.front();
            glm::mat4& transform = transforms.front();

            lambda(node, transform);

            scene_nodes.pop();
            transforms.pop();

            uint32_t child_count = node->mNumChildren;
            for (uint32_t i = 0; i < child_count; i++) {
                aiNode* child = node->mChildren[i];

                if (child == nullptr) {
                    continue;
                }

                scene_nodes.push(child);
                transforms.push(mat4_assimp_to_glm(child->mTransformation));
            }
        }
    }

    //SingleMaterial assimp_load_single_material(const aiScene* scene, uint32_t submodel_index, const std::filesystem::path& path) {
    //
    //    if (submodel_index < 0 || submodel_index >= scene->mNumMeshes) {
    //        std::cout << "[AssetImporter Error] submodel_index (" << submodel_index << ") is not present in asset at : " << path << std::endl;
    //        ASSERT(false);
    //    }
    //    
    //    SingleMaterial single_material;
    //
    //    //scene->mMaterials.
    //
    //}

}

SingleModel Asset::load_single_model(uint32_t submodel_index)
{
    const aiScene* ai = (const aiScene*)scene;
    return assimp_load_singlemodel(ai, submodel_index, filepath);
}

Model Asset::load_model()
{
    Model model;
    
    const aiScene* assimp_scene = (const aiScene*)scene;
    uint32_t submodel_count = assimp_scene->mNumMeshes;
    
    for (uint32_t submodel = 0; submodel < submodel_count; submodel++) {
        model.add_model(assimp_load_singlemodel(assimp_scene, submodel, filepath));
    }

    if (assimp_scene->mRootNode == nullptr) {
        std::wcout << "[AssetImporter Error] Asset::load_model() is called with a mesh that doesn't contain a root node : " << std::filesystem::absolute(filepath) << std::endl;
        ASSERT(false);
    }

    std::unordered_map<aiNode*, node_t> assimp_to_model_node;

    assimp_traverse_nodes(assimp_scene, [&](aiNode* node, glm::mat4& transform) {

        aiNode* parent = node->mParent;
        size_t parent_native = Model::null_node_name;

        if (parent != nullptr) {
            auto iterator = assimp_to_model_node.find(parent);
            if (iterator != assimp_to_model_node.end()) {
                parent_native = iterator->second;
            }
        }

        node_t new_node = model.add_node(parent_native);
        assimp_to_model_node[node] = new_node;

        model[new_node].set_transform(mat4_assimp_to_glm(node->mTransformation));

        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            if (model.does_model_exist(node->mMeshes[i])) {
                model[new_node].add_submodel(node->mMeshes[i]);
            }
            else {
                std::cout << "[AssetImporter Error] Asset::load_model() is called but model but submodel index mismatch occured during node traversal at asset : " << std::filesystem::absolute(filepath) << std::endl;
                ASSERT(false);
            }
        }

        });

    return model;
}

Mesh Asset::load_mesh() {
    return Mesh(load_model());
}


ModelMaterial::SingleMaterial Asset::load_single_model_material(uint32_t submodel_index)
{
    const aiScene* assimp_scene = (const aiScene*)scene;
    uint32_t submodel_count = assimp_scene->mNumMeshes;

    ModelMaterial::SingleMaterial single_material;

    if (submodel_index < 0 || submodel_index >= assimp_scene->mNumMeshes) {
        std::cout << "[AssetImporter Error] submodel_index (" << submodel_index << ") is not present in asset at : " << filepath << std::endl;
        ASSERT(false);
    }

    if (!assimp_scene->HasMaterials()) {
        std::cout << "[AssetImporter Error] materials doesn't exist : " << filepath << std::endl;
        ASSERT(false);
    }
    uint32_t material_index = assimp_scene->mMeshes[submodel_index]->mMaterialIndex;
    
    aiString albedo_path;
    aiString normal_path;
    aiString roughness_path;
    aiString metallic_path;
    aiString ambient_occlusion_path;
    aiString height_path;
    aiString emmisive_path;

    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE, 0, &albedo_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= albedo_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.albedo_image = std::make_shared<Image>(image_path.string(), 3, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_NORMALS, 0, &normal_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= normal_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.normal_image = std::make_shared<Image>(image_path.string(), 3, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughness_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= roughness_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.roughness_image = std::make_shared<Image>(image_path.string(), 1, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_METALNESS, 0, &metallic_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= metallic_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.metallic_image = std::make_shared<Image>(image_path.string(), 1, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ambient_occlusion_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= ambient_occlusion_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.ambient_occlusion_image = std::make_shared<Image>(image_path.string(), 1, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_HEIGHT, 0, &height_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= height_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.height_image = std::make_shared<Image>(image_path.string(), 1, false);
    }
    if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_EMISSIVE, 0, &emmisive_path) == aiReturn_SUCCESS) {
        std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= emmisive_path.C_Str()).string();
        if (!std::filesystem::exists(image_path))
            std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
        else
            single_material.emissive_image = std::make_shared<Image>(image_path.string(), 3, false);
    }
    
    return single_material;
}

ModelMaterial Asset::load_model_material()
{
    ModelMaterial material;

    const aiScene* assimp_scene = (const aiScene*)scene;
    uint32_t submodel_count = assimp_scene->mNumMeshes;


    struct _ImageParam {
        std::string path = "";
        uint32_t channel_count = 3;
        bool flip = false;
    };

    std::vector<std::shared_ptr<Image>> images;
    std::vector<_ImageParam> image_params;
    std::unordered_map<std::filesystem::path, std::pair<uint32_t, _ImageParam>> path_to_index_param;

    for (uint32_t submodel_index = 0; submodel_index < submodel_count; submodel_index++) {

        uint32_t material_index = assimp_scene->mMeshes[submodel_index]->mMaterialIndex;

        aiString albedo_path;
        aiString normal_path;
        aiString roughness_path;
        aiString metallic_path;
        aiString ambient_occlusion_path;
        aiString height_path;
        aiString emmisive_path;

        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE, 0, &albedo_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= albedo_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 4;
                path_to_index_param[image_path] = std::pair(0u, params);
            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_NORMALS, 0, &normal_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= normal_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 3;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughness_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= roughness_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 1;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_METALNESS, 0, &metallic_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= metallic_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 1;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ambient_occlusion_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= ambient_occlusion_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 1;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_HEIGHT, 0, &height_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= height_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 1;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_EMISSIVE, 0, &emmisive_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= emmisive_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                _ImageParam params;
                params.path = image_path.string();
                params.channel_count = 3;
                path_to_index_param[image_path] = std::pair(0u, params);

            }
        }
    }

    int i = 0;
    for (auto& pair : path_to_index_param) {
        auto& index_param = pair.second;
        index_param.first = i;
        images.push_back(nullptr);
        image_params.push_back(index_param.second);
        i++;
    }

    std::cout << "---------" << std::endl;
    std::mutex m;

    uint32_t texture_count = images.size();
    //#pragma omp parallel for
    for (int i = 0; i < texture_count; i++) {
        std::cout << i << std::endl;
        images[i] = std::make_shared<Image>(image_params[i].path, image_params[i].channel_count, image_params[i].flip);

        m.lock();
        std::cout << "loaded : " << image_params[i].path << "   " << images[i]->get_width() << ", " << images[i]->get_height() << std::endl;
        m.unlock();

    }

    for (uint32_t submodel_index = 0; submodel_index < submodel_count; submodel_index++) {
        std::cout << submodel_index << std::endl;

        ModelMaterial::SingleMaterial single_material;

        uint32_t material_index = assimp_scene->mMeshes[submodel_index]->mMaterialIndex;

        aiString albedo_path;
        aiString normal_path;
        aiString roughness_path;
        aiString metallic_path;
        aiString ambient_occlusion_path;
        aiString height_path;
        aiString emmisive_path;

        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE, 0, &albedo_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= albedo_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(albedo_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.albedo_image = images[index];
            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_NORMALS, 0, &normal_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= normal_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(normal_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.normal_image = images[index];

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &roughness_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= roughness_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(roughness_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.roughness_image = images[index];

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_METALNESS, 0, &metallic_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= metallic_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(metallic_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.metallic_image = images[index];

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &ambient_occlusion_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= ambient_occlusion_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(ambient_occlusion_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.ambient_occlusion_image = images[index];

            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_HEIGHT, 0, &height_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= height_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(height_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.height_image = images[index];
            }
        }
        if (assimp_scene->mMaterials[material_index]->GetTexture(aiTextureType_EMISSIVE, 0, &emmisive_path) == aiReturn_SUCCESS) {
            std::filesystem::path image_path = std::filesystem::absolute(filepath.parent_path() /= emmisive_path.C_Str()).string();
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("textures/") /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path("../textures/") /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }
            if (!std::filesystem::exists(image_path)) {
                image_path = std::filesystem::absolute(filepath.parent_path() /= std::filesystem::path(emmisive_path.C_Str()).filename()).string();
            }

            if (!std::filesystem::exists(image_path))
                std::cout << "[AssetImporter Error] Image not found : " << image_path << std::endl;
            else {
                uint32_t index = path_to_index_param[image_path].first;
                single_material.emissive_image = images[index];
            }
        }

        material.add_material(std::move(single_material));
    }

    return material;
}

MeshMaterial Asset::load_mesh_material()
{
    MeshMaterial material;
    material.load_material(load_model_material());
    return material;
}
