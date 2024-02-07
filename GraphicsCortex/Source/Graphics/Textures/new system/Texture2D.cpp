#include "Texture2D.h"

Texture2D::Texture2D(int width, int height, ColorFormat format, ColorTextureFormat internal_format, Type type, bool generate_mipmap, float mipmap_bias) :
	width(width), height(height), is_color_texture(true), color_format(format), color_texture_format(internal_format), type(type), generate_mipmap(generate_mipmap), mipmap_bias(mipmap_bias)
{

}

Texture2D::Texture2D(int width, int height, DepthStencilFormat format, DepthStencilTextureFormats internal_format, Type type, bool generate_mipmap, float mipmap_bias) :
	width(width), height(height), is_color_texture(false), depth_stencil_format(format), depth_stencil_texture_format(internal_format), type(type), generate_mipmap(generate_mipmap), mipmap_bias(mipmap_bias)
{

}

Texture2D::~Texture2D()
{

}
