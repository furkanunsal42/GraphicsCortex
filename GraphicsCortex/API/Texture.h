#pragma once
#include "Config.h"
#include "Image.h"
#include "glm.hpp"

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
	unsigned int min_filter = GL_LINEAR_MIPMAP_LINEAR;
	unsigned int mag_filter = GL_LINEAR;
	unsigned int wrap_s = GL_REPEAT;
	unsigned int wrap_t = GL_REPEAT;
	float mipmap_bias = 0.0f;
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
	bool _is_initialized = false;
};

class Texture : public TextureBase {
public:
	unsigned int target = GL_TEXTURE_2D;

	Texture(int multisample = 0);
	~Texture();
	void release();

	void load_image(Image& image);
	void initialize_blank_image(int width, int height);
	
	void generate_texture_object();
	bool is_initialized();

	void bind();
	void unbind();

	Image save(bool vertical_flip = true);

	void print_info(unsigned int opengl_code);
	
	bool is_loaded();

	friend Material;
	friend FrameBuffer;

private:
	bool _is_initialized = false;
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

	void generate_texture_object();
	bool is_initialized();

	Image save(bool vertical_flip = true);

	void print_info(unsigned int opengl_code);

	bool is_loaded();

	friend Material;
	friend FrameBuffer;
private:
	bool _is_initialized = false;
};

class TextureArray : public TextureBase {
public:
	unsigned int target = GL_TEXTURE_2D_ARRAY;

	TextureArray(int multisample = 0);
	~TextureArray();

	void release();

	void load_images(std::vector<Image>& images);
	void initialize_blank_images(int width, int height, int depth, int channels);

	void load_single_image(Image& image, int index, bool generate_mipmaps = false);
	void generate_mipmaps();

	void bind();
	void unbind();

	void generate_texture_object();
	bool is_initialized();

	Image save(int index, bool vertical_flip = true);

	void print_info(unsigned int opengl_code);

	bool is_loaded();
private:
	bool _is_initialized = false;
	int depth = NULL;
};

class Camera;
class Graphic_s;
class uniform_update_queue;
namespace default_program {
	uniform_update_queue solid_default_uniform_queue(Camera& camera, Graphic_s graphic);
}


class UnorderedMaterial {
public:
	int material_texture_slot = 0;

	TextureArray texture_array;

	UnorderedMaterial(int size);
	UnorderedMaterial(const UnorderedMaterial& other) = default;
	UnorderedMaterial(UnorderedMaterial&& other) = default;
	UnorderedMaterial(const std::string& filename);

	UnorderedMaterial();
	~UnorderedMaterial();

	UnorderedMaterial& operator=(const UnorderedMaterial& other) = default;
	UnorderedMaterial& operator=(UnorderedMaterial&& other) = default;

	enum TextureType {
		COLOR,
		SPECULAR,
		NORMAL,
	};

	void set_texture(const std::string& filename, int desired_channels, int index, TextureType type = COLOR);

	void bind();
	void bind_single_threaded();
	void unbind();

	void set_texture_width(unsigned int width);
	void set_texture_height(unsigned int height);
	void set_texture_size(unsigned int width, unsigned int height);

	unsigned int get_texture_width();
	unsigned int get_texture_height();

	glm::vec3 get_active_textures_by_type();

protected:
	unsigned int texture_width = 1024;
	unsigned int texture_height = 1024;
	int array_size;
	std::vector<bool> _is_texture_loaded;
	std::vector<int> _texture_desired_channels;
	std::vector<TextureType> _texture_types;
	std::vector<std::string> _texture_filenames;
	
	// texture type is enabled if at least one of type is loaded
	glm::vec3 _active_textures_by_type = glm::vec3(-1.0f, -1.0f, -1.0f);	// example: _active_textures_by_type[0] -> active color map index to use in texture array, -1 means none

	bool _first_texture_set = true;

	friend uniform_update_queue default_program::solid_default_uniform_queue(Camera& camera, Graphic_s graphic);
};

class Material : public UnorderedMaterial{
public:
	int color_map_index = 0;
	int specular_map_index = 1;
	int normal_map_index = 2;

	TextureArray texture_array;

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
	int array_size = 3;
	std::vector<bool> _is_texture_loaded = { false, false, false };
	std::vector<int> _texture_desired_channels = { NULL, NULL, NULL };
	std::vector<std::string> _texture_filenames = { "", "", "" };
};
