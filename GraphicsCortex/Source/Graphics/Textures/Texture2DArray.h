#pragma once
#include "Config.h"
#include "TextureBase.h"
#include "gl/glew.h"
#include <functional>
#include <thread>

class Program;
class ComputeProgram;
class BindlessMaterial;
class Framebuffer;
class AsyncBuffer;

class Texture2DArray : public TextureBase2 {
	friend Program;
	friend ComputeProgram;
	friend BindlessMaterial;
	friend Framebuffer;
public:
	int64_t texture_handle = 0;
	int mipmap_begin_level = 0;
	float mipmap_bias = 0.0f;

	WrapMode wrap_u = WrapMode::CLAMP;
	WrapMode wrap_v = WrapMode::CLAMP;
	SamplingFilter mipmap_min_filter = SamplingFilter::LINEAR;
	SamplingFilter min_filter = SamplingFilter::LINEAR;
	SamplingFilter mag_filter = SamplingFilter::LINEAR;
	bool is_bindless = false;

	Texture2DArray() = delete;
	Texture2DArray(const Texture2DArray& other) = delete;
	Texture2DArray(Texture2DArray&& other) = default;
	Texture2DArray& operator=(const Texture2DArray& other) = delete;
	Texture2DArray& operator=(Texture2DArray&& other) = default;

	Texture2DArray(int width, int height, int texture_count, ColorTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f, int multisample = 0);
	Texture2DArray(int width, int height, int texture_count, DepthStencilTextureFormat internal_format, int mipmap_levels = 1, float mipmap_bias = 0.0f, int multisample = 0);
	~Texture2DArray();
	void release();

	void bind(int texture_slot);
	void bind_as_image(int texture_slot, int mipmap_level);
	void bind_as_slice(int texture_slot, int mipmap_level, int layer_index);

	void bind();
	void unbind();

	void load_data(int texture_index, const void* image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(int texture_index, const void* image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(int texture_index, const Image& image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(int texture_index, const Image& image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	
	void load_data_async(int texture_index, const std::string& image_filepath, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data_async(int texture_index, const std::string& image_filepath, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);

	void load_data(int texture_index, const void* image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(int texture_index, const void* image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	void load_data(int texture_index, const Image& image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(int texture_index, const Image& image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target = 0);
	
	void load_data(const void* image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(const void* image, ColorFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void load_data(const Image& image, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(const Image& image, ColorFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void load_data(AsyncBuffer& async_buffer, ColorFormat format, Type type, int mipmap_target = 0);
	void load_data(AsyncBuffer& async_buffer, ColorFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);

	void load_data(const void* image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(const void* image, DepthStencilFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void load_data(const Image& image, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(const Image& image, DepthStencilFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void load_data(AsyncBuffer& async_buffer, DepthStencilFormat format, Type type, int mipmap_target = 0);
	void load_data(AsyncBuffer& async_buffer, DepthStencilFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);

	void generate_mipmap();

	void load_data_with_mipmaps(int texture_index, const void* image, ColorFormat format, Type type);
	void load_data_with_mipmaps(int texture_index, const Image& image, ColorFormat format, Type type);
	void load_data_with_mipmaps(int texture_index, const void* image, DepthStencilFormat format, Type type);
	void load_data_with_mipmaps(int texture_index, const Image& image, DepthStencilFormat format, Type type);

	void load_data_with_mipmaps(const void* image, ColorFormat format, Type type);
	void load_data_with_mipmaps(const Image& image, ColorFormat format, Type type);
	void load_data_with_mipmaps(const void* image, DepthStencilFormat format, Type type);
	void load_data_with_mipmaps(const Image& image, DepthStencilFormat format, Type type);

	void load_data_width_mipmaps_async(int texture_index, const std::string& image_filepath, ColorFormat format, Type type);

	void wait_async_load();

	/*
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap);
	void copy_to_texture(Texture2D& target_texture, int self_mipmap, int target_mipmap, int self_x, int self_y, int width, int height, int target_x, int target_y);
	*/

	std::shared_ptr<Image> get_image(int texture_index, ColorFormat format, Type type, int mipmap_level);
	std::shared_ptr<Image> get_image(int texture_index, ColorFormat format, Type type, int mipmap_level, int x, int y, int width, int height);
	std::shared_ptr<Image> get_image(int texture_index, DepthStencilFormat format, Type type, int mipmap_level);
	std::shared_ptr<Image> get_image(int texture_index, DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int width, int height);

	std::shared_ptr<Image> get_image(ColorFormat format, Type type, int mipmap_level);
	std::shared_ptr<Image> get_image(ColorFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth);
	std::shared_ptr<Image> get_image(DepthStencilFormat format, Type type, int mipmap_level);
	std::shared_ptr<Image> get_image(DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth);

	std::shared_ptr<AsyncBuffer> get_image_async(ColorFormat format, Type type, int mipmap_level);
	std::shared_ptr<AsyncBuffer> get_image_async(ColorFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth);
	std::shared_ptr<AsyncBuffer> get_image_async(DepthStencilFormat format, Type type, int mipmap_level);
	std::shared_ptr<AsyncBuffer> get_image_async(DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth);

	void clear(int texture_index, unsigned char clear_data, int mipmap_target = 0);
	void clear(int texture_index, float clear_data, int mipmap_target = 0);
	void clear(int texture_index, glm::vec2 clear_data, int mipmap_target = 0);
	void clear(int texture_index, glm::vec3 clear_data, int mipmap_target = 0);
	void clear(int texture_index, glm::vec4 clear_data, int mipmap_target = 0);
	void clear(int texture_index, unsigned char clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(int texture_index, float clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(int texture_index, glm::vec2 clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(int texture_index, glm::vec3 clear_data, int x, int y, int width, int height, int mipmap_target = 0);
	void clear(int texture_index, glm::vec4 clear_data, int x, int y, int width, int height, int mipmap_target = 0);

	void clear(unsigned char clear_data, int mipmap_target = 0);
	void clear(float clear_data, int mipmap_target = 0);
	void clear(glm::vec2 clear_data, int mipmap_target = 0);
	void clear(glm::vec3 clear_data, int mipmap_target = 0);
	void clear(glm::vec4 clear_data, int mipmap_target = 0);
	void clear(unsigned char clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void clear(float clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void clear(glm::vec2 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void clear(glm::vec3 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);
	void clear(glm::vec4 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target = 0);


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

	glm::ivec3 get_size();
	void force_allocation();
	bool is_allocated();

	uint32_t get_mipmap_count();

	ColorTextureFormat get_internal_format_color();
	DepthStencilTextureFormat get_internal_format_depthstencil();

private:
	unsigned int target = GL_TEXTURE_2D_ARRAY;
	unsigned int multisample_amount = 0;

	int width;
	int height;
	int texture_count;

	union {
		ColorTextureFormat color_texture_format;
		DepthStencilTextureFormat depth_stencil_texture_format;
	};

	int mipmap_levels = 0;

	bool is_color_texture;

	bool _texture_generated = false;
	bool _texture_allocated = false;
	bool _texture_handle_created = false;

	void _set_texture_parameters();

	void _generate_texture();
	void _allocate_texture();
	void _create_handle();

	bool async_load_happening = false;
	std::unordered_map<int, bool> async_load_happening_per_face;					// key: texture_index  |  value: is_asnyc_load_happening
	std::unordered_map<int, std::function<void()>> post_async_load_functions;		// key: texture_index  |  value: post_asynch_loac_function
	std::unordered_map<int, std::thread*> async_loading_threads;					// key: texture_index  |  value: asyn_loading_thread
	std::unordered_map<int, Image*> async_images;									// key: texture_index  |  value: async_image

	int _get_gl_internal_format();
};

