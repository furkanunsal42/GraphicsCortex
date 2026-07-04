#pragma once

#include <memory>
#include <filesystem>

#include "Texture2D.h"
#include "TextureCubeMap.h"
#include "ComputeProgram.h"


extern std::filesystem::path image_based_lighting_solver_shader_path;

class ImageBasedLightingSolver {
public:

	void project_to_cubemap					(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture, uint32_t mipmap = 0);
	void project_to_equiangular				(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture, uint32_t mipmap = 0);
	void calculate_sky_irradiance_texture	(TextureCubeMap& in_sky_texture, TextureCubeMap&	out_irradiance_texture);
	void calculate_sky_prefiltered_texture	(TextureCubeMap& in_sky_texture, TextureCubeMap&	out_prefiltered_texture);
	void calculate_sky_brdf_texture			(TextureCubeMap& in_sky_texture, Texture2D&			out_brdf_texture);

private:

	void compile_shaders();
	bool are_shaders_compiled = false;

	std::shared_ptr<ComputeProgram> cp_project_to_cubemap;
	std::shared_ptr<ComputeProgram> cp_project_to_equiangular;
	std::shared_ptr<ComputeProgram> cp_compute_irradiance;
	std::shared_ptr<ComputeProgram> cp_compute_prefiltered;
	std::shared_ptr<ComputeProgram> cp_compute_brdf;
};
