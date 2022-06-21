#pragma once
#include "GL\glew.h"

#include <string>


class Texture {
public:
	unsigned int id = 0;
	int width = NULL, height = NULL, channels = NULL;
	unsigned char* image_data = NULL;

	// default paremeters
	bool vertical_flip = true;
	unsigned int min_filter = GL_NEAREST;
	unsigned int mag_filter = GL_NEAREST;
	unsigned int wrap_s = GL_REPEAT;
	unsigned int wrap_t = GL_REPEAT;
	unsigned int internal_format = NULL;
	unsigned int format = NULL;
	unsigned int data_type = GL_UNSIGNED_BYTE;

	Texture();
	void load_image(std::string file_path, int desired_channels = 4);
	void free_image();
	void initialize_blank_image();
	void bind(unsigned short texture_slot);
	void unbind();
};

class Material {
public:
	int color_map_slot = 0;
	int specular_map_slot = 1;
	int normal_map_slot = 2;

	Texture* color_map = nullptr;
	Texture* specular_map = nullptr;
	Texture* normal_map = nullptr;

	Material();
	Material(Texture& color, Texture& specular, Texture& normal);

	void bind();
	void unbind();
};

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
	unsigned int id = 0;
	int width = 0, height = 0;
	Texture color_texture;
	RenderBuffer depth_stencil_buffer;

	FrameBuffer(int width, int height);

	void bind();
	void unbind();
	void render();
};