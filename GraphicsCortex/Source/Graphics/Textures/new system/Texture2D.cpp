#include "Texture2D.h"
#include <iostream>
#include "Debuger.h"

Texture2D::Texture2D(int width, int height, ColorFormat format, ColorTextureFormat internal_format, Type type, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), is_color_texture(true), color_format(format), color_texture_format(internal_format), type(type), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture2D::Texture2D(int width, int height, DepthStencilFormat format, DepthStencilTextureFormat internal_format, Type type, int mipmap_levels, float mipmap_bias) :
	width(width), height(height), is_color_texture(false), depth_stencil_format(format), depth_stencil_texture_format(internal_format), type(type), mipmap_levels(mipmap_levels), mipmap_bias(mipmap_bias)
{
	_generate_texture();
}

Texture2D::~Texture2D()
{
	release();
}

void Texture2D::release()
{
	if (_texture_generated) glDeleteTextures(1, &id);
	_texture_generated = false;
	_texture_allocated = false;
}

void Texture2D::bind()
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to bind()" << std::endl;
		ASSERT(false);
	}
	glBindTexture(target, id);
}

void Texture2D::bind(int texture_slot)
{
	glActiveTexture(GL_TEXTURE0 + texture_slot);
	bind();
}

void Texture2D::unbind()
{
	glBindTexture(target, 0);
}

void Texture2D::load_data(const void* image, int mipmap_target)
{
	if (!_generate_texture) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	glTexSubImage2D(target, mipmap_target, 0, 0, this->width, this->height, _get_gl_format(), _get_gl_type(), image);

	_user_data_loaded = true;
}

void Texture2D::load_data(const void* image, int x, int y, int write_width, int write_height, int mipmap_target)
{
	if (!_generate_texture) {
		std::cout << "[OpenGL Error] released texture tried to load_data()" << std::endl;
		ASSERT(false);
	}
	_allocate_texture();

	bind();
	glTexSubImage2D(target, mipmap_target, x, y, write_width, write_height, _get_gl_format(), _get_gl_type(), image);

	_user_data_loaded = true;
}

void Texture2D::load_data(const Image& image, int mipmap_target)
{
	load_data(image._image_data, mipmap_target);
}

void Texture2D::load_data(const Image& image, int x, int y, int width, int height, int mipmap_target)
{
	load_data(image._image_data, x, y, width, height, mipmap_target);
}

void Texture2D::generate_mipmap(float bias)
{
	if (!_texture_generated) {
		std::cout << "[OpenGL Error] released texture tried to generate_mipmap()" << std::endl;
		ASSERT(false);
	}
	if (!_texture_allocated || !_user_data_loaded) {
		std::cout << "[OpenGL Warning] Texture2D::generate_mipmap() was called but user data not loaded yet" << std::endl;
		return;
	}

	bind();
	glGenerateMipmap(target);
	
	_mipmap_generated = true;
}



void Texture2D::_generate_texture()
{
	if (_texture_generated) return;
	glGenTextures(1, &id);

	_texture_generated = true;
}

void Texture2D::_allocate_texture()
{
	if (!_texture_generated) return;
	if (_texture_allocated) return;

	bind();
	glTexStorage2D(target, mipmap_levels, _get_gl_internal_format(), width, height);

	_texture_allocated = true;
}

int Texture2D::_get_gl_type()
{
	return Type_to_OpenGL(type);
}

int Texture2D::_get_gl_format()
{
	return is_color_texture ? ColorFormat_to_OpenGL(color_format) : DepthStencilFormat_to_OpenGL(depth_stencil_format);
}

int Texture2D::_get_gl_internal_format()
{
	return is_color_texture ? ColorTextureFormat_to_OpenGL(color_texture_format) : DepthStencilTextureFormat_to_OpenGL(depth_stencil_texture_format);
}
