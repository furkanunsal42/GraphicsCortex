#pragma once

#include <cstdint>

class CortexRendererBase {
public:
	static constexpr uint32_t attrib_position		= 0;
	static constexpr uint32_t attrib_normal			= 1;
	static constexpr uint32_t attrib_tangent		= 2;
	static constexpr uint32_t attrib_bitangent		= 3;
	static constexpr uint32_t attrib_uv0			= 4;
	static constexpr uint32_t attrib_uv1			= 5;
	static constexpr uint32_t attrib_vertex_color	= 6;
	static constexpr uint32_t attrib_bone_indicies	= 7;
	static constexpr uint32_t attrib_bone_weights	= 8;
	static constexpr uint32_t attrib_count			= 9;

	// pbr metallic-roughness workflow
	static constexpr uint32_t pbr_tex_albedo_opacity		= 0;
	static constexpr uint32_t pbr_tex_ao_roughness_metallic = 1;
	static constexpr uint32_t pbr_tex_normal				= 2;	//	pbr shared textures
	static constexpr uint32_t pbr_tex_height				= 3;
	static constexpr uint32_t pbr_tex_emissive				= 4;
	static constexpr uint32_t pbr_tex_refrection			= 5;
	static constexpr uint32_t pbr_tex_count					= 6;

	virtual void init(glm::ivec2 resolution) = 0;
	virtual void render(Camera& camera) = 0;
};
