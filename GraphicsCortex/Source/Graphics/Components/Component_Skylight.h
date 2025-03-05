#pragma once
#include "Component/Component.h"

#include "Tools/EquirectangularProjector/EquirectengularProjector.h"
#include "Texture2D.h"
#include "Camera.h"
#include "FrameBuffer.h"

class SkylightComponent : public Component {
public:

	SkylightComponent(std::shared_ptr<TextureCubeMap> sky_texutre = nullptr);

	void make_primary();
	bool is_primary();

	void set_sky_texture(Texture2D& equirectangular_texture, uint32_t resolution = 512, TextureCubeMap::ColorTextureFormat cubemap_texture_format = TextureCubeMap::ColorTextureFormat::RGBA16F);
	void set_sky_texture(std::shared_ptr<TextureCubeMap> sky_texture);
	std::shared_ptr<TextureCubeMap> get_sky_texture();

	void set_sky_irradiance_texture(std::shared_ptr<TextureCubeMap> sky_irradiance_texture);
	std::shared_ptr<TextureCubeMap> get_sky_irradiance_texture();

	void set_sky_prefiltered_texture(std::shared_ptr<TextureCubeMap> prefiltered_texture);
	std::shared_ptr<TextureCubeMap> get_sky_prefiltered_texture();

	void set_sky_brdf_texture(std::shared_ptr<Texture2D> brdf_texture);
	std::shared_ptr<Texture2D> get_sky_brdf_texture();

	void calculate_sky_irradiance_texture(uint32_t irradiance_texture_resolution = 32);
	void calculate_sky_prefiltered_texture(uint32_t prefiltered_texture_resolution = 128, uint32_t mipmap_count = 6);
	void calculate_sky_brdf_texture(uint32_t view_direction_resolution = 512, uint32_t roughness_resolution = 512);

	void render(Framebuffer& framebuffer, Camera& camera);
	void render(Camera& camera);

	void on_added_to_scene(Scene& scene);

	Mesh unit_cube;
	std::shared_ptr<TextureCubeMap> sky_texture = nullptr;
	std::shared_ptr<TextureCubeMap> sky_irradiance_texture = nullptr;
	std::shared_ptr<TextureCubeMap> sky_prefiltered_texture = nullptr;
	std::shared_ptr<Texture2D> sky_brdf_texture = nullptr;

	Framebuffer framebuffer;

	// owns Global Resource SkylightComponent_projector
	// owns Global Resource SkylightComponent_render
	// owns Global Resource SkylightComponent_irradiance_convolver
	// owns Global Resource SkylightComponent_prefiltered_convolver
	// owns Global Resource SkylightComponent_operation

private:
	bool is_primary_skylight = false;
	bool initialized = false;
	void init();
};