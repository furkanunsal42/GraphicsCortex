#pragma once
#include "GL\glew.h"

#include <string>
#include "Texture.h"
#include "ShaderCompiler.h"
#include "Graphic.h"

class RenderBuffer {
public:
	int multisample = 0;
	unsigned int id = 0;
	int width = 0, height = 0;
	unsigned int internal_format = GL_DEPTH24_STENCIL8;
	RenderBuffer();
	void bind();
	void unbind();
};

class FrameBuffer {
public:
	unsigned int id = 0;
	int width = 0, height = 0;
	int texture_slot = 9;
	Texture color_texture;
	RenderBuffer depth_stencil_buffer;
	Graphic screen;
	Program* program = nullptr;

	FrameBuffer(int width, int height, int anti_alliasing = 0);

	void bind();
	void unbind();
	void render();
private:
	int multisample = 0;
	bool screen_initialized = false;
};