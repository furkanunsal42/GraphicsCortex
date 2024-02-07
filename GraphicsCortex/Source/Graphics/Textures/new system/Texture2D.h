#pragma once
#include "Config.h"
#include "TextureBase.h"
#include "gl/glew.h"

class Texture2D : public TextureBase{
public:
	int width;
	int height;
	WrapMode wrap_x = WrapMode::CLAMP;
	WrapMode wrap_y = WrapMode::CLAMP;
	SamplingFilter min_filter = SamplingFilter::LINEAR;
	SamplingFilter mag_filter = SamplingFilter::LINEAR;
	
	
	union {
		ColorTextureFormat color_texture_format;
		DepthStencilTextureFormat depth_stencil_texture_format;
	};
	
	union {
		ColorFormat color_format;
		DepthStencilFormat depth_stencil_format;
	};
	
	Type type;

	Texture2D() = delete;
	//Texture2D(int width, int height, WellDefinedColorFormat texture_format, bool generate_mipmap = false, float mipmap_bias = 0.0f);
	Texture2D(int width, int height, ColorFormat format, ColorTextureFormat InternalFormat, Type type, int mipmap_levels = 0, float mipmap_bias = 0.0f);
	Texture2D(int width, int height, DepthStencilFormat format, DepthStencilTextureFormat InternalFormat, Type type, int mipmap_levels = 0, float mipmap_bias = 0.0f);
	~Texture2D();
	void release();
	void bind();
	void bind(int texture_slot);
	void unbind();

	void load_data(const void* image, int mipmap_target = 0);
	void load_data(const void* image, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(const Image& image, int mipmap_target = 0);
	void load_data(const Image& image, int x, int y, int width, int height, int mipmap_target = 0);

	void generate_mipmap(float bias);

	void copy_to_texture(Texture2D* target_texture, int self_mipmap, int target_mipmap);
	void copy_to_texture(Texture2D* target_texture, int self_mipmap, int target_mipmap, int self_x, int self_y, int width, int height, int target_x, int target_y);

	Image get_image(int mipmap_level);
	Image get_image(int mipmap_level, int x, int y, int width, int height);

	void clear(int clear_data);
	void clear(float clear_data);
	void clear(glm::vec2 clear_data);
	void clear(glm::vec3 clear_data);
	void clear(glm::vec4 clear_data);
	void clear(int clear_data,		 int x, int y, int width, int height);
	void clear(float clear_data,	 int x, int y, int width, int height);
	void clear(glm::vec2 clear_data, int x, int y, int width, int height);
	void clear(glm::vec3 clear_data, int x, int y, int width, int height);
	void clear(glm::vec4 clear_data, int x, int y, int width, int height);

	SamplingFilter query_mag_filter();
	SamplingFilter query_min_filter();
	WrapMode query_wrap_x();
	WrapMode query_wrap_y();
	WrapMode query_wrap_z();
	int query_base_level();
	float query_lod_bias();
	int query_max_level();
	int query_max_lod();
	int query_min_lod();
	int query_swizzle_r();
	int query_swizzle_g();
	int query_swizzle_b();
	int query_swizzle_a();
	void query_swizzle_rgba(int& r_out, int& g_out, int& b_out, int& a_out);

	int query_width(int mipmap_level);
	int query_height(int mipmap_level);
	int query_depth(int mipmap_level);
	int query_internal_format(int mipmap_level);
	int query_red_type(int mipmap_level);
	int query_green_type(int mipmap_level);
	int query_blue_type(int mipmap_level);
	int query_alpha_type(int mipmap_level);
	int query_depth_type(int mipmap_level);
	int query_red_size(int mipmap_level);
	int query_green_size(int mipmap_level);
	int query_blue_size(int mipmap_level);
	int query_alpha_size(int mipmap_level);
	int query_depth_size(int mipmap_level);
	bool query_is_compressed(int mipmap_level);
	int query_compressed_image_size(int mipmap_level);

private:
	int target = GL_TEXTURE_2D;
	int mipmap_levels = 0;
	float mipmap_bias = 0.0f;

	bool is_color_texture;

	bool _texture_generated = false;
	bool _texture_allocated = false;
	bool _user_data_loaded = false;
	bool _mipmap_generated = false;

	void _generate_texture();
	void _allocate_texture();

	int _get_gl_type();
	int _get_gl_format();
	int _get_gl_internal_format();
};

