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
	unsigned int texture_slot = 0;
	Texture();
	void read_image(std::string file_path, int desired_channels = 4);
	void load_image(std::string file_path, int desired_channels = 4, bool free_ram = false);
	void queue_image(std::string file_path, int desired_channels = 4, bool free_ram = false);
	void read_queue();
	void load_queue();
	void free_image();
	void initialize_blank_image();
	void bind();
	void unbind();
	
private:
	std::string current_loaded_file_name = "";
	std::string queued_image_path = "";
	int queued_desired_channels = 4;
	bool queued_free_ram = false;
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
