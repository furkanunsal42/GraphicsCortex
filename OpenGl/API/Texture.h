#pragma once
#include "GL\glew.h"

#include <string>

class Texture {
public:
	unsigned int id;
	int width, height, channels;
	unsigned char* image_data;

	// default paremeters
	bool vertical_flip = true;
	unsigned int min_filter = GL_NEAREST;
	unsigned int mag_filter = GL_NEAREST;
	unsigned int wrap_s = GL_REPEAT;
	unsigned int wrap_t = GL_REPEAT;
	unsigned int texture_slot = 0;
	unsigned int internal_format = GL_RGBA8;
	unsigned int format = GL_RGBA;

	Texture();
	void load_image(std::string file_path, int desired_channels = 4);
	void bind();
	void unbind();
};