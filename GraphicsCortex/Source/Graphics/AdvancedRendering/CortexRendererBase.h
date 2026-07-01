#pragma once

#include <cstdint>

class CortexRendererBase {

	static constexpr uint32_t attrib_vertex_slot = 0;
	static constexpr uint32_t attrib_normal_slot = 1;
	static constexpr uint32_t attrib_tangent_slot = 2;
	static constexpr uint32_t attrib_bitangent_slot = 3;
	static constexpr uint32_t attrib_uv0_slot = 4;
	static constexpr uint32_t attrib_uv1_slot = 5;
	static constexpr uint32_t attrib_vertex_color_slot = 6;
	static constexpr uint32_t attrib_bone_indicies_slot = 7;
	static constexpr uint32_t attrib_bone_weights_slot = 8;
	static constexpr uint32_t attrib_custom_slot0 = 9;

	static constexpr uint32_t pbr_mat_albedo = 0;	// pbr	metallic-roughness workflow
	static constexpr uint32_t pbr_mat_roughness = 1;
	static constexpr uint32_t pbr_mat_metallic = 2;

	static constexpr uint32_t pbr_mat_diffuse = 0;	// pbr  specular-glossiness workflow
	static constexpr uint32_t pbr_mat_specular = 1;
	static constexpr uint32_t pbr_mat_glossiness = 2;

	static constexpr uint32_t pbr_mat_normal = 3;	//	pbr shared textures
	static constexpr uint32_t pbr_mat_ambient_occlusion = 4;
	static constexpr uint32_t pbr_mat_height = 5;
	static constexpr uint32_t pbr_mat_emissive = 6;
	static constexpr uint32_t pbr_mat_opacity = 7;
	static constexpr uint32_t pbr_mat_refrection = 8;

	static constexpr uint32_t pbr_mat_custom0 = 9;


	virtual void init(glm::ivec2 resolution, Texture2D::ColorTextureFormat format) = 0;
	virtual void render() = 0;
};
