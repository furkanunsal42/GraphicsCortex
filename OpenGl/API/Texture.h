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
	unsigned int internal_format = NULL;
	unsigned int format = NULL;

	Texture();
	void load_image(std::string file_path, int desired_channels = 4);
	void bind(unsigned short texture_slot);
	void unbind();
};

class Material {
public:
	unsigned short color_map_slot = 0;
	unsigned short specular_map_slot = 1;
	unsigned short normal_map_slot = 2;

	Texture color_map;
	Texture specular_map;
	Texture normal_map;

	Material();
	Material(Texture color, Texture specular, Texture normal);

	void bind();
	void unbind();
};
