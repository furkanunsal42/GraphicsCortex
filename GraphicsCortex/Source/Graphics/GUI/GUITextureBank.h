#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "Texture2D.h"

class GUITextureBank {
public:

	std::shared_ptr<Texture2D> get_texture(
		const std::filesystem::path& filepath,
		Texture2D::ColorTextureFormat internal_format = Texture2D::ColorTextureFormat::RGBA8,
		Texture2D::ColorFormat format = Texture2D::ColorFormat::RGBA,
		Texture2D::Type type = Texture2D::Type::UNSIGNED_BYTE,
		glm::ivec2 target_resolution = glm::ivec2(1, 1),
		int32_t mipmap = 1,
		bool vertical_flip = true
	);

private:

	std::unordered_map<std::filesystem::path, std::shared_ptr<Texture2D>> created_textures;

};