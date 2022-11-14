#pragma once
#include "Config.h"
#include "Image.h"

#include "GL\glew.h"

#include <string>
#include <vector>

class Material;
class FrameBuffer;

class TextureBase {
public:
	unsigned int id = 0;

	// default paremeters
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

	TextureBase(int multisample = 0);
	~TextureBase();

	virtual void release();

	virtual void bind();
	virtual void unbind();

	enum info {
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
	
	virtual void print_info(unsigned int opengl_code);

	virtual bool is_loaded();

	friend Material;
	friend FrameBuffer;

protected:
	int width = NULL, height = NULL, channels = NULL;
	bool _loaded_on_gpu = false;
};

class Texture : public TextureBase {
public:
	unsigned int target = GL_TEXTURE_2D;

	Texture(int multisample = 0);
	~Texture();
	void release();

	void load_image(Image& image);
	void initialize_blank_image(int width, int height);
	
	void bind();
	void unbind();

	Image save(bool vertical_flip = true);

	void print_info(unsigned int opengl_code);
	
	bool is_loaded();

	friend Material;
	friend FrameBuffer;

private:
	bool _load_image_check(bool print_errors = true);
};

class RenderBuffer : public TextureBase {
public:
	unsigned int target = GL_RENDERBUFFER;
	
	RenderBuffer(int multisample = 0);
	~RenderBuffer();
	void release();

	void initialize_blank_image(int width, int height);

	void bind();
	void unbind();

	Image save(bool vertical_flip = true);

	void print_info(unsigned int opengl_code);

	bool is_loaded();

	friend Material;
	friend FrameBuffer;
};

class TextureArray : public TextureBase {
public:
	unsigned int target = GL_TEXTURE_2D_ARRAY;

	TextureArray(int multisample = 0);
	~TextureArray();

	void release();

	void load_images(std::vector<Image>& images);
	void initialize_blank_images(int width, int height, int depth);

	void load_single_image(Image& image, int index);

	void bind();
	void unbind();

	Image save(int index, bool vertical_flip = true);

	void print_info(unsigned int opengl_code);

	bool is_loaded();
private:
	int depth = NULL;
	bool _load_image_check(bool print_errors = true);
};


class Material {
public:
	int color_map_slot = 0;
	int specular_map_slot = 1;
	int normal_map_slot = 2;

	Texture color_map;
	Texture specular_map;
	Texture normal_map;

	Material();
	~Material();

	void set_color_texture(const std::string& filename, int desired_channels);
	void set_specular_texture(const std::string& filename, int desired_channels);
	void set_normal_texture(const std::string& filename, int desired_channels);

	void bind();
	void unbind();

	bool _enable_color_map = false;
	bool _enable_specular_map = false;
	bool _enable_normal_map = false;

private:

	std::string _color_map_filename = "";
	std::string _specular_map_filename = "";
	std::string _normal_map_filename = "";
	
	int _color_map_desired_channels = NULL;
	int _specular_map_desired_channels = NULL;
	int _normal_map_desired_channels = NULL;


};
