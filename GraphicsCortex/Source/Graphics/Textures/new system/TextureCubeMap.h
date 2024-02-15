#pragma once
#include "Config.h"
#include "TextureBase.h"
#include "gl/glew.h"
#include <functional>
#include <thread>
#include <array>

class Framebuffer2;

class TextureCubeMap : public TextureBase2 {
	friend Framebuffer2;
public:
	enum Face {
		UP		= 0,
		DOWN	= 1,
		RIGHT	= 2,
		LEFT	= 3,
		FORWARD	= 4,
		BACK	= 5,
	};

	int64_t texture_handle = 0;
	int mipmap_begin_level = 0;
	float mipmap_bias = 0.0f;

	WrapMode wrap_u = WrapMode::CLAMP;
	WrapMode wrap_v = WrapMode::CLAMP;
	SamplingFilter mipmap_min_filter = SamplingFilter::LINEAR;
	SamplingFilter min_filter = SamplingFilter::LINEAR;
	SamplingFilter mag_filter = SamplingFilter::LINEAR;

	TextureCubeMap() = delete;
	TextureCubeMap(const TextureCubeMap& other) = delete;

	TextureCubeMap(int width, int height, ColorTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f);
	TextureCubeMap(int width, int height, DepthStencilTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f);
	~TextureCubeMap();
	void release();

	void bind(int texture_slot);

	void bind();
	void unbind();

	void load_data(Face face, const void* image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(Face face, const void* image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(Face face, const Image& image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(Face face, const Image& image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data_async(Face face, const std::string& image_filepath, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data_async(Face face, const std::string& image_filepath, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);

	void load_data(Face face, const void* image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(Face face, const void* image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(Face face, const Image& image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(Face face, const Image& image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);

	void generate_mipmap();

	void load_data_with_mipmaps(Face face, const void* image, ColorFormat format, Type type);
	void load_data_with_mipmaps(Face face, const Image& image, ColorFormat format, Type type);
	void load_data_with_mipmaps(Face face, const void* image, DepthStencilFormat format, Type type);
	void load_data_with_mipmaps(Face face, const Image& image, DepthStencilFormat format, Type type);

	void load_data_width_mipmaps_async(Face face, const std::string& image_filepath, ColorFormat format, Type type);

	void wait_async_load();

	/*
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap);
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap, int self_x, int self_y, int width, int height, int target_x, int target_y);
	*/

	Image get_image(Face face, ColorFormat format, Type type, int mipmap_level);
	Image get_image(Face face, ColorFormat format, Type type, int mipmap_level, int x, int y, int width, int height);
	Image get_image(Face face, DepthStencilFormat format, Type type, int mipmap_level);
	Image get_image(Face face, DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int width, int height);

	void clear(Face face, unsigned char clear_data, int mipmap_target = 0);
	void clear(Face face, float clear_data, int mipmap_target = 0);
	void clear(Face face, glm::vec2 clear_data, int mipmap_target = 0);
	void clear(Face face, glm::vec3 clear_data, int mipmap_target = 0);
	void clear(Face face, glm::vec4 clear_data, int mipmap_target = 0);
	void clear(Face face, unsigned char clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(Face face, float clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(Face face, glm::vec2 clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(Face face, glm::vec3 clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(Face face, glm::vec4 clear_data, int x, int y, int width, int height, int mipmap_target = 0);

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
	unsigned int target = GL_TEXTURE_CUBE_MAP;

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
	bool _texture_handle_created = false;
	bool _user_data_loaded = false;
	bool _mipmap_generated = false;

	void _set_texture_parameters();

	void _generate_texture();
	void _allocate_texture();
	void _create_handle();

	bool async_load_happening = false;
	std::array<bool, 6> async_load_happening_per_face = {false, false, false, false, false, false};
	std::array<std::function<void()>, 6> post_async_load_functions = { [](){},[](){},[](){},[](){},[](){},[](){} };
	std::array<std::thread*, 6> async_loading_threads = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	std::array<Image*, 6> async_images = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };

	int static get_gl_face_index(Face face);
};

