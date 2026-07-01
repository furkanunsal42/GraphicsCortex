#pragma once

#include <iostream>
#include <string>
#include <array>
#include <filesystem>

#include <assimp/material.h>
#include <assimp/pbrmaterial.h>

#include <stb_image.h>

namespace PBR {
    // Shared constants reflecting your design
    static constexpr uint32_t albedo            = 0;    // (or diffuse)
    static constexpr uint32_t normal            = 1;
    static constexpr uint32_t opacity           = 2;
    static constexpr uint32_t roughness         = 3;    // (or specular)
    static constexpr uint32_t metallic          = 4;    // (or glossiness)
    static constexpr uint32_t ambient_occlusion = 5;
    static constexpr uint32_t emissive          = 6;
    static constexpr uint32_t height            = 7;
    static constexpr uint32_t refraction        = 8;
    static constexpr uint32_t MAX_SLOTS         = 9;
}

enum class PBRWorkflow : uint32_t {
    MetallicRoughness = 0,
    SpecularGlossiness = 1
};

enum class TextureChannel : uint32_t {
    None = 0,
    R = 1,
    G = 2,
    B = 3,
    A = 4,
    RGB = 5,
    RGBA = 6
};

struct ParsedTexture {
    std::filesystem::path absolute_path = "";
    int32_t               channel_count = 0;
    int32_t               texture_x = 0, texture_y = 0;
    TextureChannel        target_channel = TextureChannel::None;

    bool exists() const { return channel_count != 0; }
    bool is_proper() const { return exists() && target_channel != TextureChannel::None; }
};

struct ParsedMaterial {
    PBRWorkflow workflow = PBRWorkflow::MetallicRoughness;
    std::array<ParsedTexture, PBR::MAX_SLOTS> maps;
};

std::string TextureChannel_to_string(TextureChannel channel) {
    switch (channel) {
    case TextureChannel::None: return "None";
    case TextureChannel::R: return "R";
    case TextureChannel::G: return "G";
    case TextureChannel::B: return "B";
    case TextureChannel::A: return "A";
    case TextureChannel::RGB: return "RGB";
    case TextureChannel::RGBA: return "RGBA";
    }
    return "error";
}

std::string index_to_texture_name(int32_t index) {
    switch (index) {
    case 0: return "albedo";
    case 1: return "normal";
    case 2: return "opacity";
    case 3: return "roughness";
    case 4: return "metallic";
    case 5: return "ambient_occlusion";
    case 6: return "emissive";
    case 7: return "height";
    case 8: return "refraction";
    }
    return "error";
}

std::ostream& operator<<(std::ostream& stream, const ParsedMaterial& material) {
    stream << (material.workflow == PBRWorkflow::MetallicRoughness ? "MetallicRoughness" : "SpecularGlossiness") << std::endl;

    for (int32_t i = 0; i < PBR::MAX_SLOTS; i++) {
        stream << index_to_texture_name(i) << std::endl;
        stream << material.maps[i].absolute_path << ", format: " << TextureChannel_to_string(material.maps[i].target_channel) << std::endl;
    }

    std::cout << std::endl;
    return stream;
}

class MaterialParser {
public:

    static ParsedMaterial parse(aiMaterial* mat, const std::filesystem::path& root_path) {

        ParsedMaterial result;
        result.workflow = PBRWorkflow::MetallicRoughness;

        //aiString dummy;
        //if (mat->GetTexture(AI_MATKEY_GLTF_PBRSPECULARGLOSSINESS_SPECULARGLOSSINESS_TEXTURE, &dummy) == AI_SUCCESS ||
        //    mat->GetTexture(AI_MATKEY_GLTF_PBRSPECULARGLOSSINESS_DIFFUSE_TEXTURE, &dummy) == AI_SUCCESS) {
        //    result.workflow = PBRWorkflow::SpecularGlossiness;
        //}
        //else if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0 && mat->GetTextureCount(aiTextureType_BASE_COLOR) == 0) {
        //    // Heuristic fallback for older .obj formats
        //    result.workflow = PBRWorkflow::SpecularGlossiness;
        //}

        // 1. Fetch File Data (Agnostic to Channels)
        result.maps[PBR::normal] = fetch(root_path, mat, { aiTextureType_NORMALS,         aiTextureType_HEIGHT });
        result.maps[PBR::height] = fetch(root_path, mat, { aiTextureType_DISPLACEMENT });
        result.maps[PBR::emissive] = fetch(root_path, mat, { aiTextureType_EMISSION_COLOR,  aiTextureType_EMISSIVE });
        result.maps[PBR::opacity] = fetch(root_path, mat, { aiTextureType_OPACITY });
        result.maps[PBR::refraction] = fetch(root_path, mat, { aiTextureType_TRANSMISSION });
        result.maps[PBR::ambient_occlusion] = fetch(root_path, mat, { aiTextureType_AMBIENT_OCCLUSION, aiTextureType_LIGHTMAP });


        if (result.workflow == PBRWorkflow::MetallicRoughness) {

            result.maps[PBR::albedo] = fetch(root_path, mat, { aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE });

            ParsedTexture orm = fetch(root_path, mat, { aiTextureType_UNKNOWN });

            if (orm.exists()) {
                result.maps[PBR::roughness] = orm;
                result.maps[PBR::metallic] = orm;
                if (!result.maps[PBR::ambient_occlusion].exists())
                    result.maps[PBR::ambient_occlusion] = orm;
            }
            else {
                result.maps[PBR::roughness] = fetch(root_path, mat, { aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_SHININESS });
                result.maps[PBR::metallic] = fetch(root_path, mat, { aiTextureType_METALNESS });
            }
        }
        else {
            result.maps[PBR::albedo] = fetch(root_path, mat, { aiTextureType_DIFFUSE, aiTextureType_BASE_COLOR });
            result.maps[PBR::roughness] = fetch(root_path, mat, { aiTextureType_SPECULAR }); // Specular
            result.maps[PBR::metallic] = fetch(root_path, mat, { aiTextureType_SHININESS }); // Glossiness
        }

        ParsedTexture& albedo = result.maps[PBR::albedo];
        ParsedTexture& opacity = result.maps[PBR::opacity];
        ParsedTexture& normal = result.maps[PBR::normal];
        ParsedTexture& emissive = result.maps[PBR::emissive];
        ParsedTexture& height = result.maps[PBR::height];
        ParsedTexture& refraction = result.maps[PBR::refraction];
        ParsedTexture& metallic = result.maps[PBR::metallic];
        ParsedTexture& roughness = result.maps[PBR::roughness];
        ParsedTexture& ambient_occlusion = result.maps[PBR::ambient_occlusion];

        // Albedo / Opacity
        if (albedo.exists()) {
            if (albedo.channel_count >= 3) {
                albedo.target_channel = TextureChannel::RGB;
            }
            else {
                std::cout << "[PBRParser Warning] Albedo is grayscale (<3 channels). Targetting R channel." << std::endl;
                albedo.target_channel = TextureChannel::R;
            }
        }

        if (albedo.exists() && albedo.channel_count == 4) {
            if (!opacity.exists()) {
                opacity = albedo; // Auto-extract opacity from albedo
                opacity.target_channel = TextureChannel::A;
            }
            else if (opacity.absolute_path == albedo.absolute_path) {
                opacity.target_channel = TextureChannel::A;
            }
            else {
                opacity.target_channel = TextureChannel::R; // Separate file
            }
        }
        else if (opacity.exists()) {
            opacity.target_channel = TextureChannel::R;
        }

        // Normal Map
        if (normal.exists()) {
            if (normal.channel_count >= 3) {
                normal.target_channel = TextureChannel::RGB;
            }
            else {
                std::cout << "[PBRParser Error] Normal map expects RGB, but found " << normal.channel_count << " channels at: " << normal.absolute_path << std::endl;
                normal.target_channel = TextureChannel::None; // Invalid
            }
        }

        // Emissive Map
        if (emissive.exists()) {
            if (emissive.channel_count >= 3) {
                emissive.target_channel = TextureChannel::RGB;
            }
            else {
                std::cout << "[PBRParser Warning] Emissive is grayscale. Targetting R channel." << std::endl;
                emissive.target_channel = TextureChannel::R;
            }
        }

        // Single-Channel Data Maps
        if (height.exists()) height.target_channel = TextureChannel::R;
        if (refraction.exists()) refraction.target_channel = TextureChannel::R;


        // --- Workflow-Specific Resolution ---

        if (result.workflow == PBRWorkflow::MetallicRoughness) {

            // Heuristic for packed ORM map:
            // If metallic and roughness exist, share a path, and have >= 3 channels, it's packed.
            // Also handles cases where an exporter incorrectly only assigned the texture to one slot.
            bool is_packed_orm = false;
            if (metallic.exists() && roughness.exists() && metallic.absolute_path == roughness.absolute_path && metallic.channel_count >= 3) {
                is_packed_orm = true;
            }
            else if (metallic.exists() && !roughness.exists() && metallic.channel_count >= 3) {
                is_packed_orm = true;
                roughness = metallic; // Assume packed into metallic
            }
            else if (roughness.exists() && !metallic.exists() && roughness.channel_count >= 3) {
                is_packed_orm = true;
                metallic = roughness; // Assume packed into roughness
            }
            else if (ambient_occlusion.exists() && ambient_occlusion.channel_count >= 3 && !metallic.exists() && !roughness.exists()) {
                is_packed_orm = true;
                metallic = ambient_occlusion;
                roughness = ambient_occlusion;
            }

            if (is_packed_orm) {
                // glTF Standard: AO = R, Roughness = G, Metallic = B
                ambient_occlusion.target_channel = TextureChannel::R;
                roughness.target_channel = TextureChannel::G;
                metallic.target_channel = TextureChannel::B;

                // If AO points to a completely different file, respect the explicit override
                if (ambient_occlusion.exists() && ambient_occlusion.absolute_path != roughness.absolute_path) {
                    ambient_occlusion.target_channel = TextureChannel::R;
                }
                else if (!ambient_occlusion.exists()) {
                    ambient_occlusion = roughness;
                    ambient_occlusion.target_channel = TextureChannel::R;
                }
            }
            else {
                // Individual Grayscale Maps
                if (metallic.exists()) {
                    metallic.target_channel = TextureChannel::R;
                    if (metallic.channel_count > 1) std::cout << "[PBRParser Warning] Metallic map is not grayscale, but treating as R channel." << std::endl;
                }
                if (roughness.exists()) {
                    roughness.target_channel = TextureChannel::R;
                    if (roughness.channel_count > 1) std::cout << "[PBRParser Warning] Roughness map is not grayscale, but treating as R channel." << std::endl;
                }
                if (ambient_occlusion.exists()) {
                    ambient_occlusion.target_channel = TextureChannel::R;
                }
            }
        }
        else { // PBRWorkflow::SpecularGlossiness

            // In this workflow:
            // "roughness" stores the Specular map (RGB)
            // "metallic" stores the Glossiness map (Grayscale or Alpha)

            bool is_packed_spec_gloss = (roughness.exists() && metallic.exists() &&
                roughness.absolute_path == metallic.absolute_path &&
                roughness.channel_count == 4);

            if (is_packed_spec_gloss) {
                roughness.target_channel = TextureChannel::RGB;
                metallic.target_channel = TextureChannel::A;
            }
            else {
                // Separate Files
                if (roughness.exists()) {
                    if (roughness.channel_count >= 3) {
                        roughness.target_channel = TextureChannel::RGB;
                    }
                    else {
                        std::cout << "[PBRParser Warning] Specular map expects RGB, but found < 3 channels. Using R." << std::endl;
                        roughness.target_channel = TextureChannel::R;
                    }
                }
                if (metallic.exists()) {
                    metallic.target_channel = TextureChannel::R;
                }
            }

            if (ambient_occlusion.exists()) {
                ambient_occlusion.target_channel = TextureChannel::R;
            }
        }

        return result;
    }

private:

    static ParsedTexture fetch(const std::filesystem::path& root_path, aiMaterial* mat, const std::vector<aiTextureType>& types) {

        aiString    path;
        bool        found = false;

        for (int32_t i = 0; i < types.size(); i++) {
            if (mat->GetTextureCount(types[i]) > 0) {
                mat->GetTexture(types[i], 0, &path);
                found = true;
                break;
            }
        }

        if (found == false)
            return { "" };

        std::filesystem::path written_path(path.C_Str());
        if (written_path.is_relative())
            written_path = root_path / written_path;

        int32_t x, y;
        int32_t channels;
        // Important: stbi_info only reads the header, so it is extremely fast and safe here!
        if (!stbi_info(written_path.string().c_str(), &x, &y, &channels)) {
            std::cout << "[PBRParser Error] Missing or unreadable image file: " << written_path << std::endl;
            return { "" }; // Prevent resolving channels on an invalid file
        }

        ParsedTexture result;
        result.absolute_path = written_path;
        result.channel_count = channels;
        result.texture_x = x;
        result.texture_y = y;
        result.target_channel = TextureChannel::None;

        return result;
    }
};