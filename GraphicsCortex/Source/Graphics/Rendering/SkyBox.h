#pragma once
#include "Graphic.h"
#include "Default_Assets.h"
#include "TextureCubeMap.h"
#include "Camera.h";

class SkyBox {
public:
	std::shared_ptr<TextureCubeMap> cubemap = std::make_shared<TextureCubeMap>(1024, TextureCubeMap::ColorTextureFormat::RGBA8, 1, 0);
	std::shared_ptr<Graphic> box = default_geometry::cube();

	SkyBox(const SkyBox& other) = delete;
	SkyBox();
	~SkyBox() = default;
	void render(Camera& camera);

	const std::string cubemap_sampler_name = "cubemap";
};