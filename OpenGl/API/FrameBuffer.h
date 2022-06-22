#pragma once
#include "GL\glew.h"

#include <string>
#include "Texture.h"
#include "ShaderCompiler.h"


class RenderBuffer {
public:
	unsigned int id = 0;
	int width = 0, height = 0;
	unsigned int internal_format = GL_DEPTH24_STENCIL8;
	RenderBuffer();
	void bind();
	void unbind();
};

class FrameBuffer {
public:
	unsigned int screen_vao_id = 0;
	unsigned int id = 0;
	int width = 0, height = 0;
	Texture color_texture;
	RenderBuffer depth_stencil_buffer;
	Program* program = nullptr;

	FrameBuffer(int width, int height);

	void bind();
	void unbind();
	void render();
};