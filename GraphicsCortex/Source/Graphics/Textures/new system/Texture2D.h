#pragma once
#include "Config.h"
#include "TextureBase.h"
#include "gl/glew.h"

class Texture2D : public TextureBase2{
public:
	int mipmap_begin_level = 0;
	float mipmap_bias = 0.0f;

	WrapMode wrap_u = WrapMode::CLAMP;
	WrapMode wrap_v = WrapMode::CLAMP;
	SamplingFilter mipmap_min_filter = SamplingFilter::LINEAR;
	SamplingFilter min_filter = SamplingFilter::LINEAR;
	SamplingFilter mag_filter = SamplingFilter::LINEAR;

	Texture2D() = delete;
	//Texture2D(int width, int height, WellDefinedColorFormat texture_format, bool generate_mipmap = false, float mipmap_bias = 0.0f);
	Texture2D(const Image& image, ColorTextureFormat internal_format, ColorFormat format, Type type, int mipmap_levels = 1, float mipmap_bias = 0.0f);
	Texture2D(int width, int height, ColorTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f);
	Texture2D(int width, int height, DepthStencilTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f);
	~Texture2D();
	void release();
	void bind();
	void bind(int texture_slot);
	void unbind();

	void load_data(const void* image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(const void* image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(const Image& image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(const Image& image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);

	void load_data(const void* image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(const void* image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(const Image& image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(const Image& image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	
	void generate_mipmap();

	void load_data_with_mipmaps(const void* image, ColorFormat format, Type type);
	void load_data_with_mipmaps(const Image& image, ColorFormat format, Type type);
	void load_data_with_mipmaps(const void* image, DepthStencilFormat format, Type type);
	void load_data_with_mipmaps(const Image& image, DepthStencilFormat format, Type type);
	/*
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap);
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap, int self_x, int self_y, int width, int height, int target_x, int target_y);

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
	*/

	SamplingFilter query_mag_filter();
	SamplingFilter query_min_filter();
	SamplingFilter query_mipmap_min_filter();
	WrapMode query_wrap_u();
	WrapMode query_wrap_v();
	WrapMode query_wrap_w();
	int query_base_level();
	float query_lod_bias();
	int query_max_level();
	int query_max_lod();
	int query_min_lod();
	int query_swizzle_r();
	int query_swizzle_g();
	int query_swizzle_b();
	int query_swizzle_a();
	int query_swizzle_rgba();

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
	
	int width;
	int height;

	union {
		ColorTextureFormat color_texture_format;
		DepthStencilTextureFormat depth_stencil_texture_format;
	};
	
	int mipmap_levels = 0;

	bool is_color_texture;

	bool _texture_generated = false;
	bool _texture_allocated = false;
	bool _user_data_loaded = false;
	bool _mipmap_generated = false;

	void _set_texture_parameters();

	void _generate_texture();
	void _allocate_texture();

	int _get_gl_internal_format();
};

