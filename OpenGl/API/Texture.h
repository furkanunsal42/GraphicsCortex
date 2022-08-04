#pragma once
#include "Config.h"

#include "GL\glew.h"

#include <string>

class Texture {
public:
	static unsigned int CurrentBindedTexture[];
	unsigned int id = 0;
	int width = NULL, height = NULL, channels = NULL;
	unsigned char* image_data = NULL;

	// default paremeters
	bool vertical_flip = true;
	bool generate_mipmap = true;
	unsigned int min_filter = GL_LINEAR_MIPMAP_NEAREST;
	unsigned int mag_filter = GL_NEAREST;
	unsigned int wrap_s = GL_REPEAT;
	unsigned int wrap_t = GL_REPEAT;
	unsigned int internal_format = NULL;
	unsigned int format = NULL;
	unsigned int data_type = GL_UNSIGNED_BYTE;
	unsigned int texture_slot = 0;
	unsigned int target = GL_TEXTURE_2D;
	int multisample_amount = 0;
	bool compress_image = false;
	Texture();
	void load_image(std::string file_path, int desired_channels = 4, bool free_ram = false);
	void queue_image(std::string file_path, int desired_channels = 4, bool free_ram = false);
	void read_queue();
	void load_queue();
	void free_image();
	void initialize_blank_image();
	void bind();
	void unbind();

	void save();

	static enum info {
		WIDTH = GL_TEXTURE_WIDTH,
		HEIGHT = GL_TEXTURE_HEIGHT, 
		DEPTH = GL_TEXTURE_DEPTH, 
		INTERNAL_FORMAT = GL_TEXTURE_INTERNAL_FORMAT, 
		RED_SIZE = GL_TEXTURE_RED_SIZE, 
		GREEN_SIZE = GL_TEXTURE_GREEN_SIZE, 
		BLUE_SIZE = GL_TEXTURE_BLUE_SIZE, 
		ALPHA_SIZE = GL_TEXTURE_ALPHA_SIZE, 
		DEPTH_SIZE = GL_TEXTURE_DEPTH_SIZE, 
		IS_COMPRESSED = GL_TEXTURE_COMPRESSED,
		COMPESSED_SIZE = GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
		BUFFER_OFFEST = GL_TEXTURE_BUFFER_OFFSET,
	};
	void print_info(unsigned int opengl_code);
	
	std::string queued_image_path = "";
	int queued_desired_channels = 4;
	bool queued_free_ram = false;
	
	bool _read_image_check(std::string& file_path, int desired_channels = 4, bool print_errors = true);
	bool _load_image_check(bool free_ram = false, bool print_errors = true);
private:
	void _read_image(std::string& file_path, int desired_channels = 4);
	void _load_image(bool free_ram = false);
	std::string currently_stored_file_name = "";
};

class Material {
private:
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
