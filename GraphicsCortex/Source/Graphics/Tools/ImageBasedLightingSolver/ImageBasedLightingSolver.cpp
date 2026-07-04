#include "ImageBasedLightingSolver.h"

std::filesystem::path image_based_lighting_solver_shader_path = "../GraphicsCortex/Source/GLSL/ImageBasedLightingSolver";

void ImageBasedLightingSolver::compile_shaders()
{
	if (are_shaders_compiled) return;

	cp_project_to_cubemap		= std::make_shared<ComputeProgram>(Shader(image_based_lighting_solver_shader_path / "project_to_cubemap.comp"));
	cp_project_to_equiangular	= std::make_shared<ComputeProgram>(Shader(image_based_lighting_solver_shader_path / "project_to_equiangular.comp"));
	cp_compute_irradiance		= std::make_shared<ComputeProgram>(Shader(image_based_lighting_solver_shader_path / "compute_irradiance.comp"));
	cp_compute_prefiltered		= std::make_shared<ComputeProgram>(Shader(image_based_lighting_solver_shader_path / "compute_prefiltered.comp"));
	cp_compute_brdf				= std::make_shared<ComputeProgram>(Shader(image_based_lighting_solver_shader_path / "compute_brdf.comp"));

	are_shaders_compiled = true;
}

void ImageBasedLightingSolver::project_to_cubemap(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture, uint32_t mipmap)
{
	compile_shaders();

	ComputeProgram& kernel = *cp_project_to_cubemap;

	
}
