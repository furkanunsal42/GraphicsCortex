#pragma once
#include "Component/Component.h"

#include "Tools/EquirectangularProjector/EquirectengularProjector.h"
#include "Texture2D.h"
#include "Camera.h"
#include "FrameBuffer.h"

class SkylightComponent : public Component {
public:

	SkylightComponent(std::shared_ptr<TextureCubeMap> sky_texutre = nullptr, std::shared_ptr<TextureCubeMap> sky_texture_convoluted = nullptr);

	void make_primary();
	bool is_primary();

	void set_sky_texture(Texture2D& equirectangular_texture, uint32_t resolution = 512, TextureCubeMap::ColorTextureFormat cubemap_texture_format = TextureCubeMap::ColorTextureFormat::RGB16F);
	void set_sky_texture(std::shared_ptr<TextureCubeMap> sky_texture);
	std::shared_ptr<TextureCubeMap> get_sky_texture();

	void set_sky_texture_convoluted(std::shared_ptr<TextureCubeMap> sky_texture_convoluted);
	std::shared_ptr<TextureCubeMap> get_sky_texture_convoluted();

	void calculate_sky_texture_convoluted(uint32_t convoluted_resolution = 32);
	
	void render(Framebuffer& framebuffer, Camera& camera);
	void render(Camera& camera);

	void on_added_to_scene(Scene& scene);

	Mesh unit_cube;
	std::shared_ptr<TextureCubeMap> sky_texture = nullptr;
	std::shared_ptr<TextureCubeMap> sky_texture_convoluted = nullptr;
	Framebuffer framebuffer;

	// owns Global Resource SkylightComponent_projector
	// owns Global Resource SkylightComponent_render
	// owns Global Resource SkylightComponent_convolver

private:
	bool is_primary_skylight = false;
	bool initialized = false;
	void init();
};