#include "Asset.h"
#include "PBRParser.h"
#include "AdvancedRendering/UnifiedRenderer.h"
#include "ECS/Prefab.h"
#include "ECS_Systems/TransformHierarchySystem.h"
#include "ECS_Systems/RenderingSystem.h"

#include <assimp/scene.h>
#include <functional>

Prefab Asset::load_prefab()
{
    Prefab prefab;
    if (scene == nullptr) return prefab;

    const aiScene* ai_scene = static_cast<const aiScene*>(scene);
    UnifiedRenderer& renderer = UnifiedRenderer::get();

    std::unordered_map<std::string, std::shared_ptr<Image>> image_cache;
    std::unordered_map<std::string, uint32_t>               texture_cache;
    
    auto get_image = [&](const ParsedTexture& pt) -> Image* {
        if (!pt.is_proper() || pt.absolute_path.empty()) return nullptr;
        std::string path = pt.absolute_path.string();

        if (image_cache.find(path) == image_cache.end()) {
            image_cache[path] = std::make_shared<Image>(path, pt.channel_count, true);
        }
        return image_cache[path].get();
        };

    auto get_img_channel = [](TextureChannel tc) {
        if (tc == TextureChannel::G) return Image::green;
        if (tc == TextureChannel::B) return Image::blue;
        if (tc == TextureChannel::A) return Image::alpha;
        return Image::red; // Default for Grayscale/R maps
        };

    auto get_or_create_texture = [&](const std::string& key, std::shared_ptr<Image> img, Texture2D::ColorTextureFormat tex_format, Texture2D::ColorFormat col_format, Texture2D::Type type, int32_t mipmap) -> uint32_t {
        if (texture_cache.find(key) != texture_cache.end()) {
            return texture_cache[key];
        }
        auto texture = std::make_shared<Texture2D>(
            *img, tex_format, col_format, type, mipmap, 0, 0
        );
        texture->is_bindless = true;
        uint32_t tex_id = renderer.create_texture(texture);
        texture_cache[key] = tex_id;
        return tex_id;
        };

    std::vector<uint32_t> loaded_materials(ai_scene->mNumMaterials);

    for (uint32_t i = 0; i < ai_scene->mNumMaterials; i++) {
        aiMaterial* ai_mat = ai_scene->mMaterials[i];

        ParsedMaterial parsed_mat = MaterialParser::parse(ai_mat, filepath.parent_path());

        uint32_t mat_id = renderer.create_material();
        UnifiedRenderer::MaterialInfo& mat_info = renderer.get_material(mat_id);

        Image* albedo_img       = get_image(parsed_mat.maps[PBR::albedo]);
        Image* opacity_img      = get_image(parsed_mat.maps[PBR::opacity]);
        Image* normal_img       = get_image(parsed_mat.maps[PBR::normal]);
        Image* ao_img           = get_image(parsed_mat.maps[PBR::ambient_occlusion]);
        Image* rough_img        = get_image(parsed_mat.maps[PBR::roughness]);
        Image* metal_img        = get_image(parsed_mat.maps[PBR::metallic]);
        Image* emissive_img     = get_image(parsed_mat.maps[PBR::emissive]);
        Image* refraction_img   = get_image(parsed_mat.maps[PBR::refraction]);

        std::shared_ptr<Image> final_albedo;
        std::string albedo_final_key;
        std::string albedo_path = parsed_mat.maps[PBR::albedo].is_proper() ? parsed_mat.maps[PBR::albedo].absolute_path.string() : "";
        std::string opacity_path = parsed_mat.maps[PBR::opacity].is_proper() ? parsed_mat.maps[PBR::opacity].absolute_path.string() : "";

        if (albedo_img) {
            if (opacity_img && albedo_path != opacity_path) {
                albedo_final_key = "ALB_OPA|" + albedo_path + "|" + opacity_path;
                if (image_cache.find(albedo_final_key) == image_cache.end()) {
                    image_cache[albedo_final_key] = std::make_shared<Image>(Image::copy_channels_from_sources(
                        Image::ChannelSource{ .image = albedo_img,  .channel = Image::red },
                        Image::ChannelSource{ .image = albedo_img,  .channel = Image::green },
                        Image::ChannelSource{ .image = albedo_img,  .channel = Image::blue },
                        Image::ChannelSource{ .image = opacity_img, .channel = get_img_channel(parsed_mat.maps[PBR::opacity].target_channel) }
                    ));
                }
            }
            else {
                albedo_final_key = albedo_path;
            }
            final_albedo = image_cache[albedo_final_key];
        }

        std::shared_ptr<Image> final_arm;
        std::string arm_final_key;
        std::string ao_path = parsed_mat.maps[PBR::ambient_occlusion].is_proper() ? parsed_mat.maps[PBR::ambient_occlusion].absolute_path.string() : "";
        std::string rough_path = parsed_mat.maps[PBR::roughness].is_proper() ? parsed_mat.maps[PBR::roughness].absolute_path.string() : "";
        std::string metal_path = parsed_mat.maps[PBR::metallic].is_proper() ? parsed_mat.maps[PBR::metallic].absolute_path.string() : "";

        if (ao_img || rough_img || metal_img) {
            if (ao_img && ao_path == rough_path && rough_path == metal_path) {
                arm_final_key = ao_path;
            }
            else {
                arm_final_key = "ARM|" + ao_path + "|" + rough_path + "|" + metal_path;
                if (image_cache.find(arm_final_key) == image_cache.end()) {
                    image_cache[arm_final_key] = std::make_shared<Image>(Image::copy_channels_from_sources(
                        Image::ChannelSource{ .image = ao_img,    .channel = ao_img ? get_img_channel(parsed_mat.maps[PBR::ambient_occlusion].target_channel) : Image::red },
                        Image::ChannelSource{ .image = rough_img, .channel = rough_img ? get_img_channel(parsed_mat.maps[PBR::roughness].target_channel) : Image::red },
                        Image::ChannelSource{ .image = metal_img, .channel = metal_img ? get_img_channel(parsed_mat.maps[PBR::metallic].target_channel) : Image::red },
                        Image::ChannelSource{ .image = nullptr,   .channel = Image::red } // Capping at 3 channels
                    ));
                }
            }
            final_arm = image_cache[arm_final_key];
        }

        std::string normal_key = parsed_mat.maps[PBR::normal].is_proper() ? parsed_mat.maps[PBR::normal].absolute_path.string() : "";
        std::string emissive_key = parsed_mat.maps[PBR::emissive].is_proper() ? parsed_mat.maps[PBR::emissive].absolute_path.string() : "";
        std::string refraction_key = parsed_mat.maps[PBR::refraction].is_proper() ? parsed_mat.maps[PBR::refraction].absolute_path.string() : "";

        std::shared_ptr<Image> final_normal = normal_img ? image_cache[normal_key] : nullptr;
        std::shared_ptr<Image> final_emissive = emissive_img ? image_cache[emissive_key] : nullptr;
        std::shared_ptr<Image> final_refraction = refraction_img ? image_cache[refraction_key] : nullptr;

        if (final_albedo) {
            mat_info.push_back(get_or_create_texture(
                albedo_final_key, 
                final_albedo, 
                Texture2D::ColorTextureFormat::RGBA8, 
                Texture2D::ColorFormat::RGBA,
                Texture2D::Type::UNSIGNED_BYTE,
                32
            ));
        }

        if (final_arm) {
            mat_info.push_back(get_or_create_texture(
                arm_final_key, 
                final_arm, 
                Texture2D::ColorTextureFormat::RGB8, 
                Texture2D::ColorFormat::RGB,
                Texture2D::Type::UNSIGNED_BYTE,
                32
            ));
        }

        if (final_normal) {
            mat_info.push_back(get_or_create_texture(
                normal_key, 
                final_normal, 
                Texture2D::ColorTextureFormat::RGB8, 
                Texture2D::ColorFormat::RGB,
                Texture2D::Type::UNSIGNED_BYTE,
                32
            ));
        }

        if (final_emissive) {
            mat_info.push_back(get_or_create_texture(
                emissive_key, 
                final_emissive, 
                Texture2D::ColorTextureFormat::R8, 
                Texture2D::ColorFormat::RED,
                Texture2D::Type::UNSIGNED_BYTE,
                32
            ));
        }

        if (final_refraction) {
            mat_info.push_back(get_or_create_texture(
                refraction_key, 
                final_refraction, 
                Texture2D::ColorTextureFormat::R8, 
                Texture2D::ColorFormat::RED,
                Texture2D::Type::UNSIGNED_BYTE,
                32
            ));
        }

        loaded_materials[i] = mat_id;
    }

    // ====================================================================
    // 2. PROCESS ALL MESHES
    // ====================================================================
    std::vector<uint32_t> loaded_meshes(ai_scene->mNumMeshes, UnifiedRenderer::invalid_id);

    for (uint32_t i = 0; i < ai_scene->mNumMeshes; i++) {
        aiMesh* ai_mesh = ai_scene->mMeshes[i];

        uint32_t mesh_id = renderer.create_mesh(PrimitiveType::triangle);
        UnifiedRenderer::MeshInfo& mesh_info = renderer.get_mesh(mesh_id);

        // Configure Vertex Format (e.g., Pos: 12, Norm: 12, UV: 8 -> 32 bytes)
        mesh_info.set_attribute(0, UnifiedRenderer::AttributeType::f32_3, 0);  // Position
        mesh_info.set_attribute(1, UnifiedRenderer::AttributeType::f32_3, 12); // Normal
        mesh_info.set_attribute(2, UnifiedRenderer::AttributeType::f32_2, 24); // UV
        mesh_info.set_vertex_stride(32);

        std::vector<float> vertex_data;
        vertex_data.reserve(ai_mesh->mNumVertices * 8);

        for (uint32_t v = 0; v < ai_mesh->mNumVertices; v++) {
            vertex_data.push_back(ai_mesh->mVertices[v].x);
            vertex_data.push_back(ai_mesh->mVertices[v].y);
            vertex_data.push_back(ai_mesh->mVertices[v].z);

            if (ai_mesh->HasNormals()) {
                vertex_data.push_back(ai_mesh->mNormals[v].x);
                vertex_data.push_back(ai_mesh->mNormals[v].y);
                vertex_data.push_back(ai_mesh->mNormals[v].z);
            }
            else {
                vertex_data.push_back(0.0f); vertex_data.push_back(1.0f); vertex_data.push_back(0.0f);
            }

            if (ai_mesh->mTextureCoords[0]) {
                vertex_data.push_back(ai_mesh->mTextureCoords[0][v].x);
                vertex_data.push_back(ai_mesh->mTextureCoords[0][v].y);
            }
            else {
                vertex_data.push_back(0.0f); vertex_data.push_back(0.0f);
            }
        }

        std::vector<uint32_t> index_data;
        for (uint32_t f = 0; f < ai_mesh->mNumFaces; f++) {
            aiFace face = ai_mesh->mFaces[f];
            for (uint32_t ind = 0; ind < face.mNumIndices; ind++) {
                index_data.push_back(face.mIndices[ind]);
            }
        }

        // Upload to UnifiedRenderer (assuming DynamicBuffer logic handles upload natively)
        mesh_info.allocate_vertices(vertex_data.size() * sizeof(float));
        mesh_info.allocate_indices(index_data.size() * sizeof(uint32_t));

        // TODO: Copy vertex_data & index_data to UnifiedRenderer's mapped GPU memory

        loaded_meshes[i] = mesh_id;
    }

    // ====================================================================
    // 3. ASSEMBLE PREFAB HIERARCHY
    // ====================================================================

    // Recursive lambda to traverse the node tree
    std::function<void(aiNode*, int32_t)> process_node = [&](aiNode* node, int32_t parent_index) {

        // 1. Create a blueprint in the Prefab linked to the parent
        int32_t current_index = prefab.create_entity(parent_index);
        auto& blueprint = prefab.get_entity(current_index);

        // 2. Extract LOCAL Transform
        aiVector3D ai_scale, ai_pos;
        aiQuaternion ai_rot;
        node->mTransformation.Decompose(ai_scale, ai_rot, ai_pos);

        TransformComponent2 transform;
        transform.local_position = glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z);
        transform.local_rotation = glm::quat(ai_rot.w, ai_rot.x, ai_rot.y, ai_rot.z);
        transform.local_scale = glm::vec3(ai_scale.x, ai_scale.y, ai_scale.z);

        blueprint.add(transform);

        // 3. Attach Render Components
        if (node->mNumMeshes == 1) {
            // Standard case: 1 Node = 1 Mesh Entity
            uint32_t mesh_idx = node->mMeshes[0];

            MeshComponent2 mesh_comp;
            mesh_comp.mesh_id = loaded_meshes[mesh_idx];
            blueprint.add(mesh_comp);

            MaterialComponent2 mat_comp;
            mat_comp.material_id = loaded_materials[ai_scene->mMeshes[mesh_idx]->mMaterialIndex];
            blueprint.add(mat_comp);

            RendererComponent2 ren_comp;
            blueprint.add(ren_comp);

        }
        else if (node->mNumMeshes > 1) {
            // Complex case: A single Assimp Node contains multiple meshes (e.g. a multi-material object).
            // Solution: We create child entities under this node for each mesh piece.
            for (uint32_t m = 0; m < node->mNumMeshes; m++) {
                uint32_t mesh_idx = node->mMeshes[m];

                int32_t child_mesh_index = prefab.create_entity(current_index);
                auto& child_blueprint = prefab.get_entity(child_mesh_index);

                // Default Transform is Identity (0,0,0) - it arleady arleady inherited from the parent node!
                TransformComponent2 child_transform;
                child_blueprint.add(child_transform);

                MeshComponent2 mesh_comp;
                mesh_comp.mesh_id = loaded_meshes[mesh_idx];
                child_blueprint.add(mesh_comp);

                MaterialComponent2 mat_comp;
                mat_comp.material_id = loaded_materials[ai_scene->mMeshes[mesh_idx]->mMaterialIndex];
                child_blueprint.add(mat_comp);

                RendererComponent2 ren_comp;
                child_blueprint.add(ren_comp);
            }
        }

        // 4. Recurse through the tree
        for (uint32_t c = 0; c < node->mNumChildren; c++) {
            process_node(node->mChildren[c], current_index);
        }
        };

    // Start traversal from the root node (No parent, so -1)
    process_node(ai_scene->mRootNode, -1);

    return prefab;
}