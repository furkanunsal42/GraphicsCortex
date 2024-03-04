#include "Texture3D.h"
#include <iostream>
#include "Debuger.h"
#include <fstream>
#include <thread>
#include <memory>
#include <functional>

namespace {
	void read_image(const std::string& filename, Image** output_image, unsigned int texture_width, unsigned int texture_height, unsigned int texture_depth, int desired_channels) {
		ASSERT(texture_depth == 1);
		std::ifstream file;
		file.open(filename);
		if (file) {
			*output_image = new Image(filename, desired_channels);
		}
		else { // file doesn't exist
			std::cout << "[ERROR] Image path not found : " << filename << std::endl;
			*output_image = new Image("../GraphicsCortex/Images/missing_texture.png", desired_channels);
		}

		if ((*output_image)->get_width() == 0 || (*output_image)->get_height() == 0 || (*output_image)->get_channels() == 0) {
			std::cout << "[ERROR] Image couldn't be properly imported : " << filename << std::endl;
			if (*output_image != nullptr) delete* output_image;
			*output_image = new Image("../GraphicsCortex/Images/missing_texture.png", desired_channels);
		}
		if (texture_width != 0 && texture_height != 0)
			(*output_image)->resize(texture_width, texture_height);
	}
}

Texture3D::Texture3D(int width, int height, int depth, ColorTextureFormat internal_format, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), depth(depth), is_color_texture(true), color_texture_format(internal_format), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture3D::Texture3D(int width, int height, int depth, DepthStencilTextureFormat internal_format, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), depth(depth), is_color_texture(false), depth_stencil_texture_format(internal_format), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture3D::~Texture3D()
{
	release();
}

void Texture3D::release()
{
	if (_texture_generated) {
		GLCall(glDeleteTextures(1, &id));
	}
	_texture_generated = false;
	_texture_allocated = false;
}

void Texture3D::bind(int texture_slot)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to bind()" << std::endl;
		ASSERT(false);
	}

	if (!_texture_allocated) {
		std::cout << "[OpenGL Warning] Texture3D tried to bind(int) but no user data was loaded yet" << std::endl;
		_allocate_texture();
	}

	GLCall(glBindTextureUnit(texture_slot, id));
}

void Texture3D::bind()
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to bind()" << std::endl;
		ASSERT(false);
	}
	std::cout << "[OpenGL Warning] Bindless Texture3D tried to bind" << std::endl;
	GLCall(glBindTexture(target, id));
}

void Texture3D::unbind()
{
	std::cout << "[OpenGL Warning] Bindless Texture3D tried to unbind" << std::endl;

	GLCall(glBindTexture(target, 0));
}

void Texture3D::load_data(const void* image, ColorFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage3D(id, mipmap_target, 0, 0, 0, this->width >> mipmap_target, this->height >> mipmap_target, this->depth >> mipmap_target, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture3D::load_data(const void* image, ColorFormat format, Type type, int x, int y, int z, int custom_width, int custom_height, int custom_depth, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage3D(id, mipmap_target, x, y, z, custom_width, custom_height, custom_depth, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture3D::load_data_async(const std::string& image_filepath, ColorFormat format, Type type, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (async_load_happening) {
		std::cout << "[Thread Error] Texture3D tried to load_data_async() while another async load wasn't terminated" << std::endl;
		ASSERT(false);
	}

	if (depth != 1) {
		std::cout << "[OpenGL Error] Texture3D tried to load_data_async() but depth wasn't 1, Image3D isn't supported yet" << std::endl;
		ASSERT(false);
	}

	async_load_happening = true;

	ASSERT(async_image == nullptr);
	ASSERT(async_loading_thread == nullptr);
	async_loading_thread = new std::thread(&read_image, image_filepath, &async_image, this->width, this->height, this->depth, ColorFormat_channels(format));

	post_async_load_function = [this, format, type, x, y, z, width, height, depth, mipmap_target]() {
		this->load_data(async_image->get_image_data(), format, type, x, y, z, width, height, depth, mipmap_target);
		};
}

void Texture3D::load_data(const void* image, DepthStencilFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage3D(id, mipmap_target, 0, 0, 0, this->width >> mipmap_target, this->height >> mipmap_target, this->depth >> mipmap_target, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture3D::load_data(const void* image, DepthStencilFormat format, Type type, int x, int y, int z, int custom_width, int custom_height, int custom_depth, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture3D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage3D(id, mipmap_target, x, y, z, custom_width, custom_height, custom_depth, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture3D::generate_mipmap()
{
	if (!_texture_generated) return;
	if (!_texture_allocated) return;
	if (!_user_data_loaded)  return;

	if (_mipmap_generated) return;

	GLCall(glGenerateTextureMipmap(id));

	_mipmap_generated = true;
}

void Texture3D::load_data_with_mipmaps(const void* image, ColorFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture3D::load_data_with_mipmaps(const void* image, DepthStencilFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture3D::_set_texture_parameters()
{
	if (_texture_handle_created) {
		std::cout << "[OpenGL Error] Texture3D tried to _set_texture_parameters() but texture handle was already created" << std::endl;
		ASSERT(false);
	}

	GLCall(glTextureParameteri(id, GL_TEXTURE_BASE_LEVEL, mipmap_begin_level));
	GLCall(glTextureParameterf(id, GL_TEXTURE_LOD_BIAS, mipmap_bias));

	int gl_min_filter = SamplingFilter_to_OpenGL(min_filter);
	int gl_mipmap_min_filter = SamplingFilter_to_OpenGL(mipmap_min_filter);
	int combined_min_filter = gl_min_filter;
	if (gl_min_filter == GL_NEAREST && gl_mipmap_min_filter == GL_NEAREST)	combined_min_filter = GL_NEAREST_MIPMAP_NEAREST;
	if (gl_min_filter == GL_NEAREST && gl_mipmap_min_filter == GL_LINEAR)	combined_min_filter = GL_NEAREST_MIPMAP_LINEAR;
	if (gl_min_filter == GL_LINEAR && gl_mipmap_min_filter == GL_NEAREST)	combined_min_filter = GL_LINEAR_MIPMAP_NEAREST;
	if (gl_min_filter == GL_LINEAR && gl_mipmap_min_filter == GL_LINEAR)	combined_min_filter = GL_LINEAR_MIPMAP_LINEAR;

	GLCall(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, combined_min_filter));
	GLCall(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, SamplingFilter_to_OpenGL(mag_filter)));
	GLCall(glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, mipmap_levels));
	GLCall(glTextureParameteri(id, GL_TEXTURE_WRAP_S, WrapMode_to_OpenGL(wrap_u)));
	GLCall(glTextureParameteri(id, GL_TEXTURE_WRAP_T, WrapMode_to_OpenGL(wrap_v)));
	GLCall(glTextureParameteri(id, GL_TEXTURE_WRAP_R, WrapMode_to_OpenGL(wrap_w)));
}

void Texture3D::_generate_texture()
{
	if (_texture_generated) return;
	GLCall(glCreateTextures(target, 1, &id));
	_texture_generated = true;
}

void Texture3D::_allocate_texture()
{
	if (!_texture_generated) return;
	if (_texture_allocated) return;

	if ((width >> mipmap_levels == 0 || height >> mipmap_levels == 0 || depth >> mipmap_levels == 0 || mipmap_levels >= sizeof(int) * 8) && mipmap_levels != 1) {
		int old_mipmap_levels = mipmap_levels;
		if (width >> mipmap_levels == 0 || mipmap_levels >= sizeof(int) * 8) mipmap_levels = std::log2(width);
		if (height >> mipmap_levels == 0 || mipmap_levels >= sizeof(int) * 8) mipmap_levels = std::log2(height);
		if (depth >> mipmap_levels == 0 || mipmap_levels >= sizeof(int) * 8) mipmap_levels = std::log2(depth);
		std::cout << "[OpenGL Warning] Texture3D with size (" << width << ", " << height << ", " << depth << ") tried to load " << old_mipmap_levels << " mipmap levels, mipmap levels reducing to " << mipmap_levels << std::endl;;
	}

	GLCall(glTextureStorage3D(id, mipmap_levels, _get_gl_internal_format(), width, height, depth));

	_texture_allocated = true;
	_create_handle();
}

void Texture3D::_create_handle()
{
	if (_texture_handle_created) return;
	if (!_texture_allocated) _allocate_texture();

	_set_texture_parameters();

	GLCall(texture_handle = glGetTextureHandleARB(id));
	GLCall(glMakeTextureHandleResidentARB(texture_handle));

	_texture_handle_created = true;
}

int Texture3D::_get_gl_internal_format()
{
	return is_color_texture ? ColorTextureFormat_to_OpenGL(color_texture_format) : DepthStencilTextureFormat_to_OpenGL(depth_stencil_texture_format);
}


Texture3D::SamplingFilter Texture3D::query_mag_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MAG_FILTER, &gl_filter));
	if (gl_filter == GL_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR) return SamplingFilter::LINEAR;

	ASSERT(false);
	return SamplingFilter();
}

Texture3D::SamplingFilter Texture3D::query_min_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MIN_FILTER, &gl_filter));
	if (gl_filter == GL_NEAREST || gl_filter == GL_NEAREST_MIPMAP_LINEAR || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR || gl_filter == GL_LINEAR_MIPMAP_LINEAR || gl_filter == GL_LINEAR_MIPMAP_NEAREST)	return SamplingFilter::LINEAR;

	ASSERT(false);
	return SamplingFilter();
}

Texture3D::SamplingFilter Texture3D::query_mipmap_min_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MIN_FILTER, &gl_filter));
	if (gl_filter == GL_LINEAR_MIPMAP_NEAREST || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR_MIPMAP_LINEAR || gl_filter == GL_NEAREST_MIPMAP_LINEAR)	return SamplingFilter::LINEAR;

	return SamplingFilter::NEAREST;
}

Texture3D::WrapMode Texture3D::query_wrap_u()
{
	int gl_wrap;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_WRAP_S, &gl_wrap));
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

Texture3D::WrapMode Texture3D::query_wrap_v()
{
	int gl_wrap;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_WRAP_T, &gl_wrap));
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

Texture3D::WrapMode Texture3D::query_wrap_w()
{
	int gl_wrap;
	bind();
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_WRAP_R, &gl_wrap));
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

int Texture3D::query_base_level()
{
	int base_level;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_BASE_LEVEL, &base_level));
	return base_level;
}

float Texture3D::query_lod_bias()
{
	float lod_bias;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_LOD_BIAS, &lod_bias));
	return lod_bias;
}

int Texture3D::query_max_level()
{
	int max_level;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MAX_LEVEL, &max_level));
	return max_level;
}

int Texture3D::query_max_lod()
{
	float max_lod;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_MAX_LOD, &max_lod));
	return max_lod;
}

int Texture3D::query_min_lod()
{
	float min_lod;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_MIN_LOD, &min_lod));
	return min_lod;
}

int Texture3D::query_swizzle_r()
{
	int swizzle_red;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &swizzle_red));
	return swizzle_red;	// returns GL enum
}

int Texture3D::query_swizzle_g()
{
	int swizzle_green;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &swizzle_green));
	return swizzle_green;	// returns GL enum
}

int Texture3D::query_swizzle_b()
{
	int swizzle_blue;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_B, &swizzle_blue));
	return swizzle_blue;	// returns GL enum
}

int Texture3D::query_swizzle_a()
{
	int swizzle_alpha;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_A, &swizzle_alpha));
	return swizzle_alpha;	// returns GL enum
}

int Texture3D::query_swizzle_rgba()
{
	int swizzle_rgba;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_RGBA, &swizzle_rgba));
	return swizzle_rgba;	// returns GL enum
}

int Texture3D::query_width(int mipmap_level)
{
	int texture_width;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_WIDTH, &texture_width));
	return texture_width;
}

int Texture3D::query_height(int mipmap_level)
{
	int texture_height;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_HEIGHT, &texture_height));
	return texture_height;
}

int Texture3D::query_depth(int mipmap_level)
{
	int texture_depth;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH, &texture_depth));
	return texture_depth;
}

int Texture3D::query_internal_format(int mipmap_level)
{
	int internal_format;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_INTERNAL_FORMAT, &internal_format));
	return internal_format;	// returns gl enum
}

int Texture3D::query_red_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_RED_TYPE, &type));
	return type;	// returns gl enum
}

int Texture3D::query_green_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_GREEN_TYPE, &type));
	return type;	// returns gl enum
}

int Texture3D::query_blue_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_BLUE_TYPE, &type));
	return type;	// returns gl enum
}

int Texture3D::query_alpha_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_ALPHA_TYPE, &type));
	return type;	// returns gl enum
}

int Texture3D::query_depth_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH_TYPE, &type));
	return type;	// returns gl enum
}

int Texture3D::query_red_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_RED_SIZE, &size));
	return size;
}

int Texture3D::query_green_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_GREEN_SIZE, &size));
	return size;
}

int Texture3D::query_blue_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_BLUE_SIZE, &size));
	return size;
}

int Texture3D::query_alpha_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_ALPHA_SIZE, &size));
	return size;
}

int Texture3D::query_depth_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH_SIZE, &size));
	return size;
}

bool Texture3D::query_is_compressed(int mipmap_level)
{
	int is_compressed;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_COMPRESSED, &is_compressed));
	if (is_compressed == GL_FALSE) return false;
	if (is_compressed == GL_TRUE) return true;

	ASSERT(false);
	return false;
}

int Texture3D::query_compressed_image_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size));
	return size;
}

glm::ivec3 Texture3D::get_size() {
	return glm::ivec3(width, height, depth);
}

void Texture3D::force_allocation() {
	wait_async_load();
	_allocate_texture();
}


void Texture3D::wait_async_load()
{
	if (async_load_happening == false) return;
	ASSERT(async_loading_thread != nullptr);
	ASSERT(post_async_load_function != nullptr);

	async_loading_thread->join();
	post_async_load_function();
	post_async_load_function = nullptr;
	delete async_loading_thread;
	delete async_image;
	async_image = nullptr;
	async_load_happening = false;
}

Image Texture3D::get_image(ColorFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to get_image() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	int r_channel = query_red_size(mipmap_level);
	int g_channel = query_green_size(mipmap_level);
	int b_channel = query_blue_size(mipmap_level);
	int a_channel = query_alpha_size(mipmap_level);

	int is_r = r_channel != 0;
	int is_g = g_channel != 0;
	int is_b = b_channel != 0;
	int is_a = a_channel != 0;

	int mipmap_channels = is_r + is_g + is_b + is_a;
	int mipmap_pixel_size = r_channel + g_channel + b_channel + a_channel;

	int mipmap_width = query_width(mipmap_level);
	int mipmap_height = query_height(mipmap_level);
	int mipmap_depth = query_depth(mipmap_level);

	if (depth != 1) {
		std::cout << "[OpenGL Error] Texture3D tried to get_image() with depth != 1, 3D Images aren't supported yet" << std::endl;
		ASSERT(false);
	}

	int format_channels = ColorFormat_channels(format);
	int image_size = width * height * depth * mipmap_pixel_size;
	unsigned char* image = new unsigned char[width * height * mipmap_pixel_size];

	GLCall(glGetTextureSubImage(id, mipmap_level, x, y, z, width, height, depth, ColorFormat_to_OpenGL(format), Type_to_OpenGL(type), image_size, image));

	return Image(image, width, height, format_channels, true);
}

Image Texture3D::get_image(DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int z, int width, int height, int depth)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to get_image() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	int d_channel = query_depth_size(mipmap_level);

	int mipmap_channels = 1;
	int mipmap_pixel_size = d_channel;

	int mipmap_width = query_width(mipmap_level);
	int mipmap_height = query_height(mipmap_level);
	int mipmap_depth = query_depth(mipmap_level);

	if (depth != 1) {
		std::cout << "[OpenGL Error] Texture3D tried to get_image() with depth != 1, 3D Images aren't supported yet" << std::endl;
		ASSERT(false);
	}

	int format_channels = 1;
	int image_size = width * height * depth * mipmap_pixel_size;
	unsigned char* image = new unsigned char[width * height * depth * mipmap_pixel_size];

	int gl_format;
	if (format == DepthStencilFormat::DEPTH) gl_format = GL_DEPTH_COMPONENT;
	else if (format == DepthStencilFormat::STENCIL) gl_format = GL_STENCIL_INDEX;
	else {
		std::cout << "[OpenGL Error] Texture3D tried to get_image() with unsuppoerted format" << std::endl;
		ASSERT(false);
	}

	GLCall(glGetTextureSubImage(id, mipmap_level, x, y, z, width, height, depth, gl_format, Type_to_OpenGL(type), image_size, image));

	return Image(image, width, height, format_channels, true);
}

void Texture3D::clear(unsigned char clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, 0, width, height, depth, mipmap_target);
}

void Texture3D::clear(float clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, 0, width, height, depth, mipmap_target);
}

void Texture3D::clear(glm::vec2 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, 0, width, height, depth, mipmap_target);
}

void Texture3D::clear(glm::vec3 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, 0, width, height, depth, mipmap_target);
}

void Texture3D::clear(glm::vec4 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, 0, width, height, depth, mipmap_target);
}

void Texture3D::clear(unsigned char clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, z, width, height, depth, GL_RED, GL_UNSIGNED_BYTE, &clear_data));
}

void Texture3D::clear(float clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, z, width, height, depth, GL_RED, GL_FLOAT, &clear_data));
}

void Texture3D::clear(glm::vec2 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, z, width, height, depth, GL_RG, GL_FLOAT, &clear_data));
}

void Texture3D::clear(glm::vec3 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, z, width, height, depth, GL_RGB, GL_FLOAT, &clear_data));
}

void Texture3D::clear(glm::vec4 clear_data, int x, int y, int z, int width, int height, int depth, int mipmap_target)
{
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture3D tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, z, width, height, depth, GL_RGBA, GL_FLOAT, &clear_data));
}
