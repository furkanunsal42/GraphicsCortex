#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "Texture2D.h"

class GUITextureBank {
public:

	std::shared_ptr<Texture2D> get_texture(
		const std::filesystem::path& filepath,
		Texture2D::ColorTextureFormat internal_format,
		Texture2D::ColorFormat format,
		Texture2D::Type type,
		glm::ivec2 target_resolution = glm::ivec2(-1),
		int32_t mipmap = 32,
		bool vertical_flip = true
	);

private:

	std::unordered_map<std::filesystem::path, std::weak_ptr<Texture2D>> created_textures;

};