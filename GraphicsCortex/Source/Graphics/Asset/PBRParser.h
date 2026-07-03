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
    static constexpr uint32_t MAX_TEX_SLOTS     = 9;
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

    bool exists() const;
    bool is_proper() const;
};

struct ParsedMaterial {
    PBRWorkflow workflow = PBRWorkflow::MetallicRoughness;
    std::array<ParsedTexture, PBR::MAX_TEX_SLOTS> maps;
};


std::ostream& operator<<(std::ostream& stream, const ParsedMaterial& material);

class MaterialParser {
public:

    static ParsedMaterial parse(aiMaterial* mat, const std::filesystem::path& root_path);

private:

    static ParsedTexture fetch(const std::filesystem::path& root_path, aiMaterial* mat, const std::vector<aiTextureType>& types);
};