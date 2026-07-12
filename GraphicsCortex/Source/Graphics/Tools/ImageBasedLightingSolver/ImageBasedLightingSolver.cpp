#include "ImageBasedLightingSolver.h"

std::filesystem::path image_based_lighting_solver_shader_path = "../GraphicsCortex/Source/GLSL/ImageBasedLightingSolver";

void ImageBasedLightingSolver::compile_shaders()
{
	if (are_shaders_compiled) return;

	cp_project_to_cubemap			= VariantedComputeProgram(Shader(image_based_lighting_solver_shader_path / "project_to_cubemap.comp"));
	cp_project_to_equirectangular	= VariantedComputeProgram(Shader(image_based_lighting_solver_shader_path / "project_to_equirectangular.comp"));
	cp_compute_irradiance			= VariantedComputeProgram(Shader(image_based_lighting_solver_shader_path / "compute_irradiance.comp"));
	cp_compute_prefiltered			= VariantedComputeProgram(Shader(image_based_lighting_solver_shader_path / "compute_prefiltered.comp"));
	cp_compute_brdf					= VariantedComputeProgram(Shader(image_based_lighting_solver_shader_path / "compute_brdf.comp"));

	are_shaders_compiled = true;
}

void ImageBasedLightingSolver::project_to_cubemap(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture)
{
	compile_shaders();

	cp_project_to_cubemap.begin_variant();
	cp_project_to_cubemap.variant_define("target_texture_format", Texture2D::ColorTextureFormat_to_OpenGL_compute_Image_format(equirectangular_texture.get_internal_format_color()));

	ComputeProgram& kernel = *cp_project_to_cubemap.get_current_variant();

	kernel.update_uniform_as_image("target_equirectangular_texture", equirectangular_texture, 0);
	kernel.update_uniform("source_cubemap", cubemap_texture);
	kernel.update_uniform("target_resolution", equirectangular_texture.get_size());

	kernel.dispatch_thread(glm::ivec3(equirectangular_texture.get_size(), 1));
}

void ImageBasedLightingSolver::project_to_equirectangular(Texture2D& equirectangular_texture, TextureCubeMap& cubemap_texture)
{
	compile_shaders();

	cp_project_to_equirectangular.begin_variant();
	cp_project_to_equirectangular.variant_define("target_texture_format", TextureCubeMap::ColorTextureFormat_to_OpenGL_compute_Image_format(cubemap_texture.get_internal_format_color()));

	ComputeProgram& kernel = *cp_project_to_equirectangular.get_current_variant();

	kernel.update_uniform("source_equiangular_texture", equirectangular_texture);
	kernel.update_uniform_as_image("target_cubemap", cubemap_texture, 0);
	kernel.update_uniform("target_resolution", cubemap_texture.get_size().x);

	kernel.dispatch_thread(equirectangular_texture.get_size().x, equirectangular_texture.get_size().x, 6);
}

void ImageBasedLightingSolver::calculate_sky_irradiance_texture(TextureCubeMap& in_sky_texture, TextureCubeMap& out_irradiance_texture)
{
}

void ImageBasedLightingSolver::calculate_sky_prefiltered_texture(TextureCubeMap& in_sky_texture, TextureCubeMap& out_prefiltered_texture)
{
}

void ImageBasedLightingSolver::calculate_sky_brdf_texture(TextureCubeMap& in_sky_texture, Texture2D& out_brdf_texture)
{
}
