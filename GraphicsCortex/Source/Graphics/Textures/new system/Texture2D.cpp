#include "Texture2D.h"
#include <iostream>
#include "Debuger.h"

Texture2D::Texture2D(const Image& image, ColorTextureFormat internal_format, ColorFormat format, Type type, int mipmap_levels, float mipmap_bias) :
	width(image.get_width()), height(image.get_height()), is_color_texture(true), color_texture_format(internal_format), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
	load_data_with_mipmaps(image, format, type);
}

Texture2D::Texture2D(int width, int height, ColorTextureFormat internal_format, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), is_color_texture(true), color_texture_format(internal_format), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture2D::Texture2D(int width, int height, DepthStencilTextureFormat internal_format, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), is_color_texture(false), depth_stencil_texture_format(internal_format), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture2D::~Texture2D()
{
	release();
}

void Texture2D::release()
{
	if (_texture_generated)
		GLCall(glDeleteTextures(1, &id));
	_texture_generated = false;
	_texture_allocated = false;
}

void Texture2D::bind()
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to bind()" << std::endl;
		ASSERT(false);
	}
	GLCall(glBindTexture(target, id));
}

void Texture2D::bind(int texture_slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	bind();
	generate_mipmap();
	_set_texture_parameters();
}

void Texture2D::unbind()
{
	GLCall(glBindTexture(target, 0));
}

void Texture2D::load_data(const void* image, ColorFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	GLCall(glTexSubImage2D(target, mipmap_target, 0, 0, this->width >> mipmap_target, this->height >> mipmap_target, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture2D::load_data(const void* image, ColorFormat format, Type type, int x, int y, int custom_width, int custom_height, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	GLCall(glTexSubImage2D(target, mipmap_target, x, y, custom_width, custom_height, TextureBase2::ColorFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture2D::load_data(const Image& image, ColorFormat format, Type type, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = this->height >> mipmap_target;

	if (mipmap_width != image.get_width() || mipmap_height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, mipmap_target);
}

void Texture2D::load_data(const Image& image, ColorFormat format, Type type, int x, int y, int width, int height, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = this->height >> mipmap_target;

	if (width > image.get_width() || height > image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " load_data() size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	if (mipmap_width < width || mipmap_height < height) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " load_data() size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, x, y, width, height, mipmap_target);
}

void Texture2D::load_data(const void* image, DepthStencilFormat format, Type type, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	GLCall(glTexSubImage2D(target, mipmap_target, 0, 0, this->width >> mipmap_target, this->height >> mipmap_target, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture2D::load_data(const void* image, DepthStencilFormat format, Type type, int x, int y, int custom_width, int custom_height, int mipmap_target)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	GLCall(glTexSubImage2D(target, mipmap_target, x, y, custom_width, custom_height, TextureBase2::DepthStencilFormat_to_OpenGL(format), TextureBase2::Type_to_OpenGL(type), image));

	_user_data_loaded = true;
}

void Texture2D::load_data(const Image& image, DepthStencilFormat format, Type type, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = this->height >> mipmap_target;

	if (mipmap_width != image.get_width() || mipmap_height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, mipmap_target);
}

void Texture2D::load_data(const Image& image, DepthStencilFormat format, Type type, int x, int y, int width, int height, int mipmap_target)
{
	int mipmap_width = this->width >> mipmap_target;
	int mipmap_height = this->height >> mipmap_target;

	if (width > image.get_width() || height > image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " load_data() size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	if (mipmap_width < width || mipmap_height < height) {
		std::cout << "[OpenGL Error] Texture2D.load_data() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " load_data() size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image._image_data, format, type, x, y, width, height, mipmap_target);
}


void Texture2D::generate_mipmap()
{
	if (!_texture_generated) return;
	if (!_texture_allocated) return;
	if (!_user_data_loaded)  return;

	if (_mipmap_generated) return;

	bind();
	GLCall(glGenerateMipmap(target));

	_mipmap_generated = true;
}

void Texture2D::load_data_with_mipmaps(const void* image, ColorFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture2D::load_data_with_mipmaps(const Image& image, ColorFormat format, Type type)
{
	if (this->width != image.get_width() || this->height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data_with_mipmaps() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture2D::load_data_with_mipmaps(const void* image, DepthStencilFormat format, Type type)
{
	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture2D::load_data_with_mipmaps(const Image& image, DepthStencilFormat format, Type type)
{
	if (this->width != image.get_width() || this->height != image.get_height()) {
		std::cout << "[OpenGL Error] Texture2D.load_data_with_mipmaps() image size mismatch. " << " texture size : (" << width << ", " << height << "), " << " image size : (" << image.get_width() << ", " << image.get_height() << ")" << std::endl;
		ASSERT(false);
	}

	load_data(image, format, type, 0);
	generate_mipmap();
}

void Texture2D::_set_texture_parameters()
{
	GLCall(glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, mipmap_begin_level));
	GLCall(glTexParameterf(target, GL_TEXTURE_LOD_BIAS, mipmap_bias));
	
	int gl_min_filter = SamplingFilter_to_OpenGL(min_filter);
	int gl_mipmap_min_filter = SamplingFilter_to_OpenGL(mipmap_min_filter);
	int combined_min_filter = gl_min_filter;
	if (gl_min_filter == GL_NEAREST && gl_mipmap_min_filter == GL_NEAREST)	combined_min_filter = GL_NEAREST_MIPMAP_NEAREST;
	if (gl_min_filter == GL_NEAREST && gl_mipmap_min_filter == GL_LINEAR)	combined_min_filter = GL_NEAREST_MIPMAP_LINEAR;
	if (gl_min_filter == GL_LINEAR  && gl_mipmap_min_filter == GL_NEAREST)	combined_min_filter = GL_LINEAR_MIPMAP_NEAREST;
	if (gl_min_filter == GL_LINEAR && gl_mipmap_min_filter == GL_LINEAR)	combined_min_filter = GL_LINEAR_MIPMAP_LINEAR;

	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, combined_min_filter));	// add mipmap support
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, SamplingFilter_to_OpenGL(mag_filter)));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipmap_levels));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, WrapMode_to_OpenGL(wrap_u)));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, WrapMode_to_OpenGL(wrap_v)));
}

void Texture2D::_generate_texture()
{
	if (_texture_generated) return;
	GLCall(glGenTextures(1, &id));

	_texture_generated = true;
}

void Texture2D::_allocate_texture()
{
	if (!_texture_generated) return;
	if (_texture_allocated) return;

	bind();
	GLCall(glTexStorage2D(target, mipmap_levels, _get_gl_internal_format(), width, height));

	_texture_allocated = true;
}

int Texture2D::_get_gl_internal_format()
{
	return is_color_texture ? ColorTextureFormat_to_OpenGL(color_texture_format) : DepthStencilTextureFormat_to_OpenGL(depth_stencil_texture_format);
}

TextureBase2::SamplingFilter Texture2D::query_mag_filter()
{
	int gl_filter;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &gl_filter);
	if (gl_filter == GL_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR) return SamplingFilter::LINEAR;
	
	ASSERT(false);
	return SamplingFilter();
}

TextureBase2::SamplingFilter Texture2D::query_min_filter()
{
	int gl_filter;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &gl_filter);
	if (gl_filter == GL_NEAREST || gl_filter == GL_NEAREST_MIPMAP_LINEAR || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR  || gl_filter == GL_LINEAR_MIPMAP_LINEAR  || gl_filter == GL_LINEAR_MIPMAP_NEAREST)	return SamplingFilter::LINEAR;

	ASSERT(false);
	return SamplingFilter();
}

TextureBase2::SamplingFilter Texture2D::query_mipmap_min_filter()
{
	int gl_filter;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &gl_filter);
	if (gl_filter == GL_LINEAR_MIPMAP_NEAREST || gl_filter == GL_NEAREST_MIPMAP_NEAREST) return SamplingFilter::NEAREST;
	if (gl_filter == GL_LINEAR_MIPMAP_LINEAR  || gl_filter == GL_NEAREST_MIPMAP_LINEAR)	return SamplingFilter::LINEAR;

	return SamplingFilter::NEAREST;
}

TextureBase2::WrapMode Texture2D::query_wrap_u()
{
	int gl_wrap;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &gl_wrap);
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

TextureBase2::WrapMode Texture2D::query_wrap_v()
{
	int gl_wrap;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_WRAP_R, &gl_wrap);
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

TextureBase2::WrapMode Texture2D::query_wrap_w()
{
	int gl_wrap;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &gl_wrap);
	if (gl_wrap == GL_CLAMP)  return WrapMode::CLAMP;
	if (gl_wrap == GL_REPEAT) return WrapMode::REPEAT;
	if (gl_wrap == GL_MIRRORED_REPEAT) return WrapMode::MIRRORED_REPEAT;

	ASSERT(false);
	return WrapMode();
}

int Texture2D::query_base_level()
{
	int base_level;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_BASE_LEVEL, &base_level);
	return base_level;
}

float Texture2D::query_lod_bias()
{
	float lod_bias;
	bind();
	glGetTexParameterfv(target, GL_TEXTURE_LOD_BIAS, &lod_bias);
	return lod_bias;
}

int Texture2D::query_max_level()
{
	int max_level;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_MAX_LEVEL, &max_level);
	return max_level;
}

int Texture2D::query_max_lod()
{
	float max_lod;
	bind();
	glGetTexParameterfv(target, GL_TEXTURE_MAX_LOD, &max_lod);
	return max_lod;
}

int Texture2D::query_min_lod()
{
	float min_lod;
	bind();
	glGetTexParameterfv(target, GL_TEXTURE_MIN_LOD, &min_lod);
	return min_lod;
}

int Texture2D::query_swizzle_r()
{
	int swizzle_red;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_R, &swizzle_red);
	return swizzle_red;	// returns GL enum
}

int Texture2D::query_swizzle_g()
{
	int swizzle_green;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_G, &swizzle_green);
	return swizzle_green;	// returns GL enum
}

int Texture2D::query_swizzle_b()
{
	int swizzle_blue;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_B, &swizzle_blue);
	return swizzle_blue;	// returns GL enum
}

int Texture2D::query_swizzle_a()
{
	int swizzle_alpha;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_A, &swizzle_alpha);
	return swizzle_alpha;	// returns GL enum
}

int Texture2D::query_swizzle_rgba()
{
	int swizzle_rgba;
	bind();
	glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, &swizzle_rgba);
	return swizzle_rgba;	// returns GL enum
}

int Texture2D::query_width(int mipmap_level)
{
	int texture_width;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_WIDTH, &texture_width);
	return texture_width;
}

int Texture2D::query_height(int mipmap_level)
{
	int texture_height;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_HEIGHT, &texture_height);
	return texture_height;
}

int Texture2D::query_depth(int mipmap_level)
{
	int texture_depth;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_DEPTH, &texture_depth);
	return texture_depth;
}

int Texture2D::query_internal_format(int mipmap_level)
{
	int internal_format;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
	return internal_format;	// returns gl enum
}

int Texture2D::query_red_type(int mipmap_level)
{
	int type;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_RED_TYPE, &type);
	return type;	// returns gl enum
}

int Texture2D::query_green_type(int mipmap_level)
{
	int type;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_GREEN_TYPE, &type);
	return type;	// returns gl enum
}

int Texture2D::query_blue_type(int mipmap_level)
{
	int type;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_BLUE_TYPE, &type);
	return type;	// returns gl enum
}

int Texture2D::query_alpha_type(int mipmap_level)
{
	int type;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_ALPHA_TYPE, &type);
	return type;	// returns gl enum
}

int Texture2D::query_depth_type(int mipmap_level)
{
	int type;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_DEPTH_TYPE, &type);
	return type;	// returns gl enum
}

int Texture2D::query_red_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_RED_SIZE, &size);
	return size;
}

int Texture2D::query_green_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_GREEN_SIZE, &size);
	return size;
}

int Texture2D::query_blue_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_BLUE_SIZE, &size);
	return size;
}

int Texture2D::query_alpha_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_ALPHA_SIZE, &size);
	return size;
}

int Texture2D::query_depth_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_DEPTH_SIZE, &size);
	return size;
}

bool Texture2D::query_is_compressed(int mipmap_level)
{
	int is_compressed;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_COMPRESSED, &is_compressed);
	if (is_compressed == GL_FALSE) return false;
	if (is_compressed == GL_TRUE) return true;
	
	ASSERT(false);
	return false;
}

int Texture2D::query_compressed_image_size(int mipmap_level)
{
	int size;
	bind();
	glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &size);
	return size;
}

Image Texture2D::get_image(ColorFormat format, Type type, int mipmap_level)
{
	return get_image(format, type, mipmap_level, 0, 0, query_width(mipmap_level), query_height(mipmap_level));
}

Image Texture2D::get_image(ColorFormat format, Type type, int mipmap_level, int x, int y, int width, int height)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to get_image() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
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

	int format_channels = ColorFormat_channels(format);
	int image_size = width * height * mipmap_pixel_size;
	unsigned char* image = new unsigned char[width * height * mipmap_pixel_size];

	GLCall(glGetTextureSubImage(id, mipmap_level, x, y, 0, width, height, 1, ColorFormat_to_OpenGL(format), Type_to_OpenGL(type), image_size, image));

	return Image(image, width, height, format_channels, true);
}

Image Texture2D::get_image(DepthStencilFormat format, Type type, int mipmap_level)
{
	return get_image(format, type, mipmap_level, 0, 0, query_width(mipmap_level), query_height(mipmap_level));
}

Image Texture2D::get_image(DepthStencilFormat format, Type type, int mipmap_level, int x, int y, int width, int height)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to get_image() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	int d_channel = query_depth_size(mipmap_level);

	int mipmap_channels = 1;
	int mipmap_pixel_size = d_channel;

	int mipmap_width = query_width(mipmap_level);
	int mipmap_height = query_height(mipmap_level);

	int format_channels = 1;
	int image_size = width * height * mipmap_pixel_size;
	unsigned char* image = new unsigned char[width * height * mipmap_pixel_size];

	int gl_format;
	if (format == DepthStencilFormat::DEPTH) gl_format = GL_DEPTH_COMPONENT;
	else if (format == DepthStencilFormat::STENCIL) gl_format = GL_STENCIL_INDEX;
	else {
		std::cout << "[OpenGL Error] Texture tried to get_image() with unsuppoerted format" << std::endl;
		ASSERT(false);
	}

	GLCall(glGetTextureSubImage(id, mipmap_level, x, y, 0, width, height, 1, gl_format, Type_to_OpenGL(type), image_size, image));

	return Image(image, width, height, format_channels, true);
}

void Texture2D::clear(unsigned char clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, width, height, mipmap_target);
}

void Texture2D::clear(float clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, width, height, mipmap_target);
}

void Texture2D::clear(glm::vec2 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, width, height, mipmap_target);
}

void Texture2D::clear(glm::vec3 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, width, height, mipmap_target);
}

void Texture2D::clear(glm::vec4 clear_data, int mipmap_target)
{
	clear(clear_data, 0, 0, width, height, mipmap_target);
}

void Texture2D::clear(unsigned char clear_data, int x, int y, int width, int height, int mipmap_target)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, 0, width, height, 1, GL_RED, GL_UNSIGNED_BYTE, &clear_data));
}

void Texture2D::clear(float clear_data, int x, int y, int width, int height, int mipmap_target)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, 0, width, height, 1, GL_RED, GL_FLOAT, &clear_data));
}

void Texture2D::clear(glm::vec2 clear_data, int x, int y, int width, int height, int mipmap_target)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, 0, width, height, 1, GL_RG, GL_FLOAT, &clear_data));
}

void Texture2D::clear(glm::vec3 clear_data, int x, int y, int width, int height, int mipmap_target)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, 0, width, height, 1, GL_RGB, GL_FLOAT, &clear_data));
}

void Texture2D::clear(glm::vec4 clear_data, int x, int y, int width, int height, int mipmap_target)
{
	bind();
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Error] Texture tried to clear() but either not allocated any ram or didn't loaded any user data yet" << std::endl;
		ASSERT(false);
	}

	GLCall(glClearTexSubImage(id, mipmap_target, x, y, 0, width, height, 1, GL_RGBA, GL_FLOAT, &clear_data));
}