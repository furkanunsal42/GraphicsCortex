#pragma once
#include "Config.h"
#include "TextureBase.h"
#include "gl/glew.h"
#include <functional>
#include <thread>

class Framebuffer;

class Renderbuffer : public TextureBase2 {
	friend Framebuffer;
public:

	Renderbuffer() = delete;
	Renderbuffer(const Renderbuffer& other) = delete;

	Renderbuffer(int width, int height, ColorTextureFormat internal_format, int multisample = 0);
	Renderbuffer(int width, int height, DepthStencilTextureFormat internal_format, int multisample = 0);
	~Renderbuffer();
	void release();

	void bind();
	void unbind();

	int query_width();
	int query_height();
	int query_internal_format();
	int query_samples();
	int query_red_size();
	int query_green_size();
	int query_blue_size();
	int query_alpha_size();
	int query_depth_size();
	int query_stencil_size();

	glm::ivec2 get_size();
	void force_allocation();
	bool is_allocated();

	ColorTextureFormat get_internal_format_color();
	DepthStencilTextureFormat get_internal_format_depthstencil();

private:
	unsigned int target = GL_RENDERBUFFER;
	unsigned int multisample_amount = 0;

	int width;
	int height;

	union {
		ColorTextureFormat color_texture_format;
		DepthStencilTextureFormat depth_stencil_texture_format;
	};

	bool is_color_texture;

	bool _texture_generated = false;
	bool _texture_allocated = false;

	void _generate_texture();
	void _allocate_texture();
};

