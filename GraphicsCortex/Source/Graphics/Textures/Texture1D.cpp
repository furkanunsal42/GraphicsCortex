#include "Texture1D.h"
#include <iostream>
#include "Debuger.h"
#include <fstream>
#include <thread>
#include <memory>
#include <functional>
#include "AsyncBuffer.h"

namespace {
	void read_image(const std::string& filename, Image** output_image, unsigned int texture_width, unsigned int texture_height, int desired_channels) {
		std::ifstream file;
		file.open(filename);
		if (file) {
			*output_image = new Image(filename, desired_channels);
		}
		else { // file doesn't exist
			std::cout << "[ERROR] Image path not found : " << filename << std::endl;
			*output_image = new Image("../GraphicsCortex/Images/missing_texture.png", desired_channels);
		}

		if ((*output_image)->get_width() == 0 || (*output_image)->get_height() == 0 || (*output_image)->get_channel_count() == 0) {
			std::cout << "[ERROR] Image couldn't be properly imported : " << filename << std::endl;
			if (*output_image != nullptr) delete* output_image;
			*output_image = new Image("../GraphicsCortex/Images/missing_texture.png", desired_channels);
		}
		if (texture_width != 0 && texture_height != 0)
			(*output_image)->resize(texture_width, texture_height);
	}
}

Texture1D::Texture1D(int width, ColorTextureFormat internal_format, int mipmap_levels, float mipmap_bias) : 
    width(width), color_texture_format(internal_format), is_color_texture(true), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
    _generate_texture();
}

Texture1D::Texture1D(int width, DepthStencilTextureFormat internal_format, int mipmap_levels, float mipmap_bias) : 
    width(width), depth_stencil_texture_format(internal_format), is_color_texture(false), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
    _generate_texture();
}

Texture1D::~Texture1D()
{
    release();
}

void Texture1D::release()
{
	if (_texture_generated) {
		GLCall(glDeleteTextures(1, &id));
	}
    _texture_generated = false;
    _texture_allocated = false;
}

void Texture1D::bind(int texture_slot)
{
    if (!_texture_generated) {
        std::cout << "[OpenGL Error] released Texture1D tried to bind()" << std::endl;
        ASSERT(false);
    }

    if (!_texture_allocated) {
        std::cout << "[OpenGL Warning] Texture1D tried to bind(int) but no user data was loaded yet" << std::endl;
        _allocate_texture();
    }

    GLCall(glBindTextureUnit(texture_slot, id));
}

void Texture1D::bind_as_image(int texture_slot, int mipmap_level)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to bind_as_image()" << std::endl;
		ASSERT(false);
	}

	if (!_texture_allocated) {
		std::cout << "[OpenGL Warning] Texture1D tried to bind_as_image() but no user data was loaded yet" << std::endl;
		_allocate_texture();
	}

	GLCall(glBindImageTexture(texture_slot, id, mipmap_level, GL_FALSE, 0, GL_READ_WRITE, _get_gl_internal_format()));
}

void Texture1D::bind()
{
    if (!_texture_generated) {
        std::cout << "[OpenGL Error] released Texture1D tried to bind()" << std::endl;
        ASSERT(false);
    }
    std::cout << "[OpenGL Warning] Bindless Texture1D tried to bind" << std::endl;
    GLCall(glBindTexture(target, id));
}

void Texture1D::unbind()
{
    std::cout << "[OpenGL Warning] Bindless Texture1D tried to unbind" << std::endl;

    GLCall(glBindTexture(target, 0));
}

void Texture1D::load_data(const void* image, ColorFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage1D(id, mipmap_target, 0, this->width >> mipmap_target, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture1D::load_data(const void* image, ColorFormat format, Type type, int x, int custom_width, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage1D(id, mipmap_target, x, custom_width, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture1D::load_data(const Image& image, ColorFormat format, Type type, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = 1;

	if (mipmap_width != image.get_width() || mipmap_height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, mipmap_target);
}

void Texture1D::load_data(const Image& image, ColorFormat format, Type type, int x, int width, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = 1;

	if (width > image.get_width() || 1 > image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " load_data() size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	if (mipmap_width < width || mipmap_height < 1) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " load_data() size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, x, width, mipmap_target);
}

void Texture1D::load_data(AsyncBuffer& async_buffer, ColorFormat format, Type type, int mipmap_target)
{
	load_data(async_buffer, format, type, 0, width >> mipmap_target, mipmap_target);
}

void Texture1D::load_data(AsyncBuffer& async_buffer, ColorFormat format, Type type, int x, int width, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	async_buffer.wait_to_sycronize_download();
	async_buffer.bind_upload();

	glTextureSubImage1D(id, mipmap_target, x, width, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), 0);

	async_buffer.unbind();
	async_buffer.set_fence_upload();

	_user_data_loaded = true;
}

void Texture1D::load_data_async(const std::string& image_filepath, ColorFormat format, Type type, int mipmap_target)
{
	load_data_async(image_filepath, format, type, 0, width, mipmap_target);
}

void Texture1D::load_data_async(const std::string& image_filepath, ColorFormat format, Type type, int x, int width, int mipmap_target)
{
	if (async_load_happening) {
		std::cout << "[Thread Error] Texture1D tried to load_data_async() while another async load wasn't terminated" << std::endl;
		ASSERT(false);
	}
	async_load_happening = true;

	ASSERT(async_image == nullptr);
	ASSERT(async_loading_thread == nullptr);
	async_loading_thread = new std::thread(&read_image, image_filepath, &async_image, this->width, 1, ColorFormat_channels(format));

	post_async_load_function = [this, format, type, x, width, mipmap_target]() {
		this->load_data(*async_image, format, type, x, width, mipmap_target);
		};
}

void Texture1D::load_data(const void* image, DepthStencilFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage1D(id, mipmap_target, 0, this->width >> mipmap_target, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture1D::load_data(const void* image, DepthStencilFormat format, Type type, int x, int custom_width, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	GLCall(glTextureSubImage1D(id, mipmap_target, x, custom_width, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture1D::load_data(const Image& image, DepthStencilFormat format, Type type, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = 1;

	if (mipmap_width != image.get_width() || mipmap_height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, mipmap_target);
}

void Texture1D::load_data(const Image& image, DepthStencilFormat format, Type type, int x, int width, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = 1;

	if (width > image.get_width() || 1 > image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " load_data() size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	if (mipmap_width < width || mipmap_height < 1) {
		std::cout << "[OpenGL Error] Texture1D.load_data() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " load_data() size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, x, width, mipmap_target);
}

void Texture1D::load_data(AsyncBuffer& async_buffer, DepthStencilFormat format, Type type, int mipmap_target) {
	load_data(async_buffer, format, type, 0, width >> mipmap_target, mipmap_target);
}

void Texture1D::load_data(AsyncBuffer& async_buffer, DepthStencilFormat format, Type type, int x, int width, int mipmap_target) {
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released Texture1D tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	async_buffer.wait_to_sycronize_download();
	async_buffer.bind_upload();

	GLCall(glTextureSubImage1D(id, mipmap_target, x, width, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), 0));

	async_buffer.unbind();
	async_buffer.set_fence_upload();

	_user_data_loaded = true;
}

void Texture1D::generate_mipmap()
{
	if (!_texture_generated) return;
	if (!_texture_allocated) return;
	if (!_user_data_loaded)  return;

	if (_mipmap_generated) return;

	GLCall(glGenerateTextureMipmap(id));

	_mipmap_generated = true;
}

void Texture1D::load_data_with_mipmaps(const void* image, ColorFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture1D::load_data_with_mipmaps(const Image& image, ColorFormat format, Type type)
{
	if (this->width != image.get_width() || 1 != image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data_with_mipmaps() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture1D::load_data_with_mipmaps(const void* image, DepthStencilFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture1D::load_data_with_mipmaps(const Image& image, DepthStencilFormat format, Type type)
{
	if (this->width != image.get_width() || 1 != image.get_height()) {
		std::cout << "[OpenGL Error] Texture1D.load_data_with_mipmaps() image size mismatch. " << " texture size : (" << width << ", " << 1 << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture1D::load_data_width_mipmaps_async(const std::string& image_filepath, ColorFormat format, Type type)
{
	if (async_load_happening) {
		std::cout << "[Thread Error] Texture1D tried to load_data_width_mipmaps_async() while another async load wasn't terminated" << std::endl;
		ASSERT(false);
	}
	async_load_happening = true;

	ASSERT(async_image == nullptr);
	ASSERT(async_loading_thread == nullptr);
	async_loading_thread = new std::thread(&read_image, image_filepath, &async_image, this->width, 1, ColorFormat_channels(format));

	post_async_load_function = [this, format, type]() {
		this->load_data_with_mipmaps(*async_image, format, type);
		};
}

void Texture1D::wait_async_load()
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

void Texture1D::_set_texture_parameters()
{
    if (_texture_handle_created) {
        std::cout << "[OpenGL Error] Texture1D tried to _set_texture_parameters() but texture handle was already created" << std::endl;
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
}

void Texture1D::_generate_texture()
{
    if (_texture_generated) return;
    GLCall(glCreateTextures(target, 1, &id));
    _texture_generated = true;
}

void Texture1D::_allocate_texture()
{
    if (!_texture_generated) return;
    if (_texture_allocated) return;

    if (width >> (mipmap_levels-1) == 0 || mipmap_levels >= sizeof(int) * 8) {
        int old_mipmap_levels = mipmap_levels;
        if (width >> mipmap_levels == 0 || mipmap_levels >= sizeof(int) * 8) mipmap_levels = std::log2(width);
        std::cout << "[OpenGL Warning] Texture1D with size (" << width << ", 1) tried to load " << old_mipmap_levels << " mipmap levels, mipmap levels reducing to " << mipmap_levels << std::endl;;
    }

    GLCall(glTextureStorage1D(id, mipmap_levels, _get_gl_internal_format(), width));

    _texture_allocated = true;
    _create_handle();
}

void Texture1D::_create_handle()
{
    if (_texture_handle_created) return;
    if (!_texture_allocated) _allocate_texture();

    _set_texture_parameters();

	if (is_bindless) {
		GLCall(texture_handle = glGetTextureHandleARB(id));
		GLCall(glMakeTextureHandleResidentARB(texture_handle));

		_texture_handle_created = true;
	}
}

int Texture1D::_get_gl_internal_format()
{
    return is_color_texture ? ColorTextureFormat_to_OpenGL(color_texture_format) : DepthStencilTextureFormat_to_OpenGL(depth_stencil_texture_format);
}


Texture1D::SamplingFilter Texture1D::query_mag_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MAG_FILTER, &gl_filter));
	if (gl_filter == GL_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR) return SamplingFilter::LINEAR;

	ASSERT(false);
	return SamplingFilter();
}

Texture1D::SamplingFilter Texture1D::query_min_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MIN_FILTER, &gl_filter));
	if (gl_filter == GL_NEAREST || gl_filter == GL_NEAREST_MIPMAP_LINEAR || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR || gl_filter == GL_LINEAR_MIPMAP_LINEAR || gl_filter == GL_LINEAR_MIPMAP_NEAREST)	return SamplingFilter::LINEAR;

	ASSERT(false);
	return SamplingFilter();
}

Texture1D::SamplingFilter Texture1D::query_mipmap_min_filter()
{
	int gl_filter;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MIN_FILTER, &gl_filter));
	if (gl_filter == GL_LINEAR_MIPMAP_NEAREST || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR_MIPMAP_LINEAR || gl_filter == GL_NEAREST_MIPMAP_LINEAR)	return SamplingFilter::LINEAR;

	return SamplingFilter::NEAREST;
}

Texture1D::WrapMode Texture1D::query_wrap_u()
{
	int gl_wrap;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_WRAP_S, &gl_wrap));
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

Texture1D::WrapMode Texture1D::query_wrap_v()
{
	int gl_wrap;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_WRAP_T, &gl_wrap));
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

Texture1D::WrapMode Texture1D::query_wrap_w()
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

int Texture1D::query_base_level()
{
	int base_level;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_BASE_LEVEL, &base_level));
	return base_level;
}

float Texture1D::query_lod_bias()
{
	float lod_bias;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_LOD_BIAS, &lod_bias));
	return lod_bias;
}

int Texture1D::query_max_level()
{
	int max_level;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_MAX_LEVEL, &max_level));
	return max_level;
}

int Texture1D::query_max_lod()
{
	float max_lod;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_MAX_LOD, &max_lod));
	return max_lod;
}

int Texture1D::query_min_lod()
{
	float min_lod;
	GLCall(glGetTextureParameterfv(id, GL_TEXTURE_MIN_LOD, &min_lod));
	return min_lod;
}

int Texture1D::query_swizzle_r()
{
	int swizzle_red;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_R, &swizzle_red));
	return swizzle_red;	// returns GL enum
}

int Texture1D::query_swizzle_g()
{
	int swizzle_green;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_G, &swizzle_green));
	return swizzle_green;	// returns GL enum
}

int Texture1D::query_swizzle_b()
{
	int swizzle_blue;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_B, &swizzle_blue));
	return swizzle_blue;	// returns GL enum
}

int Texture1D::query_swizzle_a()
{
	int swizzle_alpha;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_A, &swizzle_alpha));
	return swizzle_alpha;	// returns GL enum
}

int Texture1D::query_swizzle_rgba()
{
	int swizzle_rgba;
	GLCall(glGetTextureParameteriv(id, GL_TEXTURE_SWIZZLE_RGBA, &swizzle_rgba));
	return swizzle_rgba;	// returns GL enum
}

int Texture1D::query_width(int mipmap_level)
{
	int texture_width;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_WIDTH, &texture_width));
	return texture_width;
}

int Texture1D::query_height(int mipmap_level)
{
	int texture_height;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_HEIGHT, &texture_height));
	return texture_height;
}

int Texture1D::query_depth(int mipmap_level)
{
	int texture_depth;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH, &texture_depth));
	return texture_depth;
}

int Texture1D::query_internal_format(int mipmap_level)
{
	int internal_format;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_INTERNAL_FORMAT, &internal_format));
	return internal_format;	// returns gl enum
}

int Texture1D::query_red_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_RED_TYPE, &type));
	return type;	// returns gl enum
}

int Texture1D::query_green_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_GREEN_TYPE, &type));
	return type;	// returns gl enum
}

int Texture1D::query_blue_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_BLUE_TYPE, &type));
	return type;	// returns gl enum
}

int Texture1D::query_alpha_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_ALPHA_TYPE, &type));
	return type;	// returns gl enum
}

int Texture1D::query_depth_type(int mipmap_level)
{
	int type;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH_TYPE, &type));
	return type;	// returns gl enum
}

int Texture1D::query_red_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_RED_SIZE, &size));
	return size;
}

int Texture1D::query_green_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_GREEN_SIZE, &size));
	return size;
}

int Texture1D::query_blue_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_BLUE_SIZE, &size));
	return size;
}

int Texture1D::query_alpha_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_ALPHA_SIZE, &size));
	return size;
}

int Texture1D::query_depth_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_DEPTH_SIZE, &size));
	return size;
}

bool Texture1D::query_is_compressed(int mipmap_level)
{
	int is_compressed;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_COMPRESSED, &is_compressed));
	if (is_compressed == GL_FALSE) return false;
	if (is_compressed == GL_TRUE) return true;

	ASSERT(false);
	return false;
}

int Texture1D::query_compressed_image_size(int mipmap_level)
{
	int size;
	GLCall(glGetTextureLevelParameteriv(id, mipmap_level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size));
	return size;
}

std::shared_ptr<Image> Texture1D::get_image(ColorFormat format, Type type, int mipmap_level)
{
	return get_image(format, type, mipmap_level, 0, query_width(mipmap_level));
}

std::shared_ptr<Image> Texture1D::get_image(ColorFormat format, Type type, int mipmap_level, int x, int width)
{
	if (!_texture_allocated) 
		_allocate_texture();

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

	int format_channels = ColorFormat_channels(format);
	int image_size = width * mipmap_pixel_size;
	unsigned char* image = new unsigned char[image_size];
	GLCall(glGetTextureSubImage(id, mipmap_level, x, 0, 0, width, 1, 1, ColorFormat_to_OpenGL(format), Type_to_OpenGL(type), image_size, image));

	return std::make_shared<Image>(image, width, 1, 1, format_channels, 1, true);
}

std::shared_ptr<Image> Texture1D::get_image(DepthStencilFormat format, Type type, int mipmap_level)
{
	return get_image(format, type, mipmap_level, 0, query_width(mipmap_level));
}

std::shared_ptr<Image> Texture1D::get_image(DepthStencilFormat format, Type type, int mipmap_level, int x, int width)
{
	if (!_texture_allocated) 
		_allocate_texture();

	int d_channel = query_depth_size(mipmap_level);

	int mipmap_channels = 1;
	int mipmap_pixel_size = d_channel;

	int mipmap_width = query_width(mipmap_level);
	int mipmap_height = query_height(mipmap_level);

	int format_channels = 1;
	int image_size = width * mipmap_pixel_size;
	unsigned char* image = new unsigned char[width * mipmap_pixel_size];

	int gl_format;
	if (format == DepthStencilFormat::DEPTH) gl_format = GL_DEPTH_COMPONENT;
	else if (format == DepthStencilFormat::STENCIL) gl_format = GL_STENCIL_INDEX;
	else {
		std::cout << "[OpenGL Error] Texture1D tried to get_image() with unsuppoerted format" << std::endl;
		ASSERT(false);
	}

	GLCall(glGetTextureSubImage(id, mipmap_level, x, 0, 0, width, 1, 1, gl_format, Type_to_OpenGL(type), image_size, image));

	return std::make_shared<Image>(image, width, 1, 1, format_channels, 1, true);
}

std::shared_ptr<AsyncBuffer> Texture1D::get_image_async(ColorFormat format, Type type, int mipmap_level) {
	return get_image_async(format, type, mipmap_level, 0, width >> mipmap_level);
}

std::shared_ptr<AsyncBuffer> Texture1D::get_image_async(ColorFormat format, Type type, int mipmap_level, int x, int width) {
	int pixel_size = query_red_size(mipmap_level) + query_green_size(mipmap_level) + query_blue_size(mipmap_level) + query_alpha_size(mipmap_level);
	pixel_size = pixel_size / 8;
	std::shared_ptr<AsyncBuffer> readback_buffer = std::make_shared<AsyncBuffer>((size_t)width * pixel_size);

	readback_buffer->wait_to_sycronize_upload();
	readback_buffer->bind_download();

	if (!_texture_allocated)
		_allocate_texture();

	int format_channels = ColorFormat_channels(format);
	size_t image_size = width * pixel_size;

	GLCall(glGetTextureSubImage(id, mipmap_level, x, 0, 0, width, 1, 1, ColorFormat_to_OpenGL(format), Type_to_OpenGL(type), image_size, 0));
	GLCall(glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT));

	readback_buffer->unbind();
	readback_buffer->set_fence_download();

	Image::ImageParameters parameters;
	parameters.width = width;
	parameters.height = 1;
	parameters.depth = 1;
	parameters.vertical_flip = true;
	parameters.bytes_per_channel = pixel_size / format_channels;
	parameters.channel_count = format_channels;

	readback_buffer->set_image_parameters(parameters);

	return readback_buffer;
}

std::shared_ptr<AsyncBuffer> Texture1D::get_image_async(DepthStencilFormat format, Type type, int mipmap_level) {
	return get_image_async(format, type, mipmap_level, 0, width >> mipmap_level);
}

std::shared_ptr<AsyncBuffer> Texture1D::get_image_async(DepthStencilFormat format, Type type, int mipmap_level, int x, int width) {
	int pixel_size = query_depth_size(mipmap_level);
	pixel_size = pixel_size / 8;
	std::shared_ptr readback_buffer = std::make_shared<AsyncBuffer>((size_t)width * pixel_size);

	readback_buffer->bind_download();
	readback_buffer->map();


	if (!_texture_allocated)
		_allocate_texture();

	int mipmap_width = query_width(mipmap_level);

	int format_channels = 1;
	size_t image_size = width * pixel_size;

	int gl_format;
	if (format == DepthStencilFormat::DEPTH) gl_format = GL_DEPTH_COMPONENT;
	else if (format == DepthStencilFormat::STENCIL) gl_format = GL_STENCIL_INDEX;
	else {
		std::cout << "[OpenGL Error] Texture1D tried to get_image_async() with unsuppoerted format" << std::endl;
		ASSERT(false);
	}

	GLCall(glGetTextureSubImage(id, mipmap_level, x, 0, 0, width, 1, 1, gl_format, Type_to_OpenGL(type), image_size, 0));

	readback_buffer->unbind();

	return readback_buffer;
}

void Texture1D::clear(unsigned char clear_data, int mipmap_target)
{
	clear(clear_data, 0, width, mipmap_target);
}

void Texture1D::clear(float clear_data, int mipmap_target)
{
	clear(clear_data, 0, width, mipmap_target);
}

void Texture1D::clear(glm::vec2 clear_data, int mipmap_target)
{
	clear(clear_data, 0, width, mipmap_target);
}

void Texture1D::clear(glm::vec3 clear_data, int mipmap_target)
{
	clear(clear_data, 0, width, mipmap_target);
}

void Texture1D::clear(glm::vec4 clear_data, int mipmap_target)
{
	clear(clear_data, 0, width, mipmap_target);
}

void Texture1D::clear(unsigned char clear_data, int x, int width, int mipmap_target)
{
	if (!_texture_allocated)
		_allocate_texture();

	uint32_t type = GL_RED;
	if (is_intager_ColorTextureFormat(get_internal_format_color()) || is_unsigned_intager_ColorTextureFormat(get_internal_format_color()))
		type = GL_RED_INTEGER;

	GLCall(glClearTexSubImage(id, mipmap_target, x, 0, 0, width, 1, 1, type, GL_UNSIGNED_BYTE, &clear_data));
}

void Texture1D::clear(float clear_data, int x, int width, int mipmap_target)
{
	if (!_texture_allocated)
		_allocate_texture();

	uint32_t type = GL_RED;
	if (is_intager_ColorTextureFormat(get_internal_format_color()) || is_unsigned_intager_ColorTextureFormat(get_internal_format_color()))
		type = GL_RED_INTEGER;

	GLCall(glClearTexSubImage(id, mipmap_target, x, 0, 0, width, 1, 1, type, GL_FLOAT, &clear_data));
}

void Texture1D::clear(glm::vec2 clear_data, int x, int width, int mipmap_target)
{
	if (!_texture_allocated)
		_allocate_texture();

	uint32_t type = GL_RG;
	if (is_intager_ColorTextureFormat(get_internal_format_color()) || is_unsigned_intager_ColorTextureFormat(get_internal_format_color()))
		type = GL_RG_INTEGER;


	GLCall(glClearTexSubImage(id, mipmap_target, x, 0, 0, width, 1, 1, type, GL_FLOAT, &clear_data));
}

void Texture1D::clear(glm::vec3 clear_data, int x, int width, int mipmap_target)
{
	if (!_texture_allocated)
		_allocate_texture();

	uint32_t type = GL_RGB;
	if (is_intager_ColorTextureFormat(get_internal_format_color()) || is_unsigned_intager_ColorTextureFormat(get_internal_format_color()))
		type = GL_RGB_INTEGER;

	GLCall(glClearTexSubImage(id, mipmap_target, x, 0, 0, width, 1, 1, type, GL_FLOAT, &clear_data));
}

void Texture1D::clear(glm::vec4 clear_data, int x, int width, int mipmap_target)
{
	if (!_texture_allocated)
		_allocate_texture();

	uint32_t type = GL_RGBA;
	if (is_intager_ColorTextureFormat(get_internal_format_color()) || is_unsigned_intager_ColorTextureFormat(get_internal_format_color()))
		type = GL_RGBA_INTEGER;

	GLCall(glClearTexSubImage(id, mipmap_target, x, 0, 0, width, 1, 1, type, GL_FLOAT, &clear_data));
}

glm::ivec1 Texture1D::get_size() {
	return glm::ivec1(width);
}

void Texture1D::force_allocation() {
	wait_async_load();
	_allocate_texture();
}

bool Texture1D::is_allocated()
{
	return _texture_allocated;
}

Texture1D::ColorTextureFormat Texture1D::get_internal_format_color()
{
	return color_texture_format;
}

Texture1D::DepthStencilTextureFormat Texture1D::get_internal_format_depthstencil()
{
	return depth_stencil_texture_format;
}
