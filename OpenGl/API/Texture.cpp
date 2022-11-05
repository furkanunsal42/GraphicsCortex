#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

unsigned int Texture::CurrentBindedTexture[MAX_TEXTURE_SLOTS];

Texture::Texture(bool renderbuffer, int renderbuffer_multisample) : 
	use_renderbuffer(renderbuffer), multisample_amount(renderbuffer_multisample)
{
	if (renderbuffer){
		GLCall(glGenRenderbuffers(1, &id));
	}
	else {
		GLCall(glGenTextures(1, &id));
	}
}

Texture::~Texture() {
	release();
}

void Texture::release()
{
	if (use_renderbuffer) {
		GLCall(glDeleteRenderbuffers(1, &id));
	}
	else {
		GLCall(glDeleteTextures(1, &id));
	}
}

bool Texture::_load_image_check(bool print_errors) {
	if (use_renderbuffer) {
		std::cout << "Texture::_load_image_check() is called while use_renderbuffer == true, operation cancelled.\n";
		return false;
	}

	if (_loaded_on_gpu) {
		return false;
	}

	return true;
}

// keeps the image
void Texture::load_image(Image& image) {
	if (!_load_image_check(true))
		return;

	if (image.get_channels() == 3) {
		if (format == NULL)
			format = GL_RGB;
		if (internal_format == NULL)
		{
			if (compress_image)
				internal_format = GL_COMPRESSED_RGB;
			else
				internal_format = GL_RGB8;
		}
	}
	else if (image.get_channels() == 4) {
		if (format == NULL)
			format = GL_RGBA;
		if (internal_format == NULL)
		{
			if (compress_image)
				internal_format = GL_COMPRESSED_RGBA;
			else
				internal_format = GL_RGBA8;
		}
	}

	width = image.get_width();
	height = image.get_height();
	channels = image.get_channels();

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));

	//if (compress_image)
		//glCompressedTexImage2D(target, 0, internal_format, width, height, 0, );
	//else 
	GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, image.get_image_data()));

	if (generate_mipmap)
		GLCall(glGenerateMipmap(target));

	_loaded_on_gpu = true;
	CurrentBindedTexture[texture_slot] = id;
}

void Texture::initialize_blank_image(int width, int height) {
	if (use_renderbuffer) {
		std::cout << "Texture::initialize_blank_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	this->width = width;
	this->height = height;

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	if (target == GL_TEXTURE_2D){
		GLCall(glTexImage2D(target, 0, internal_format, this->width, this->height, 0, format, data_type, NULL));
		GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
		if (generate_mipmap) {
			GLCall(glGenerateMipmap(target));
		}
	}
	else{
		GLCall(glTexImage2DMultisample(target, multisample_amount, internal_format, this->width, this->height, GL_TRUE));
	}
	CurrentBindedTexture[texture_slot] = id;
}


void Texture::bind() {
	if (use_renderbuffer) {
		GLCall(glBindRenderbuffer(target, id));
		if (multisample_amount == 0) {
			GLCall(glRenderbufferStorage(target, internal_format, width, height));
		}
		else {
			GLCall(glRenderbufferStorageMultisample(target, multisample_amount, internal_format, width, height));
		}
		return;
	}

	#ifdef TEXTURE_REPEATED_BIND_OPTIMIZATION
	if (CurrentBindedTexture[texture_slot] == id)
		return;
	#endif
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));

	CurrentBindedTexture[texture_slot] = id;
}

void Texture::unbind() {
	if (use_renderbuffer) {
		GLCall(glBindRenderbuffer(target, 0));
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, 0));
	CurrentBindedTexture[texture_slot] = 0;
}

Image Texture::save(bool vertical_flip) {
	bind();

	int w, h;
	int image_internal_format = internal_format;
	int image_compressed = false;

	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w));
	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h));
	
	int image_size = w * h * channels * sizeof(unsigned char);

	if (compress_image){
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &image_size));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED, &image_compressed));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &image_internal_format));
	}
	
	unsigned char* i_pixels = new unsigned char [image_size / sizeof(unsigned char)];
	
	if (compress_image){
		GLCall(glGetCompressedTexImage(target, 0, i_pixels));
	}
	else {
		GLCall(glGetTexImage(target, 0, format, data_type, i_pixels));
	}
	
	return Image (i_pixels, w, h, channels, vertical_flip);

	//if (compress_image){
	//	GLCall(glCompressedTexImage2D(target, 0, image_internal_format, width, height, 0, image_size, i_pixels));
	//}
	//else {
	//	GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, i_pixels));
	//}
}

void Texture::print_info(unsigned int opengl_code) {
	bind();
	int data;
	GLCall(glGetTexLevelParameteriv(target, 0, opengl_code, &data));
	std::cout << "[INFO] Texture::print_info returns " << data << std::endl;
}

bool Texture::is_loaded() {
	return _loaded_on_gpu;
}

// ----------------------------------------------------------------------------------

TextureArray::TextureArray(unsigned int array_size) {
	GLCall(glGenTextures(1, &id));
}

TextureArray::~TextureArray() {
	release();
}

void TextureArray::release() {
	GLCall(glDeleteTextures(1, &id));
}

void TextureArray::load_image(Image& image) {

}

void TextureArray::initialize_blank_image(int width, int height) {

}

void TextureArray::bind() {

}

void TextureArray::unbind() {

}

Image TextureArray::save(bool vertical_flip) {

}

void TextureArray::print_info(unsigned int opengl_code) {

}

bool TextureArray::is_loaded() {

}
	
bool TextureArray::_load_image_check(bool print_errors) {

}

// ------------------------------------------------------------------------------------

Material::Material() { }

Material::~Material() {
	color_map.~Texture();
	specular_map.~Texture();
	normal_map.~Texture();
}

void Material::set_color_texture(const std::string& filename, int desired_channels){
	_enable_color_map = filename != "";
	_color_map_filename = filename;
	if (_enable_color_map) {
		_color_map_desired_channels = desired_channels;
	}
}
void Material::set_specular_texture(const std::string& filename, int desired_channels){
	_enable_specular_map = filename != "";
	_specular_map_filename = filename;
	if (_enable_specular_map) {
		_specular_map_desired_channels = desired_channels;
	}
}
void Material::set_normal_texture(const std::string& filename, int desired_channels) {
	_enable_normal_map = filename != "";
	_normal_map_filename = filename;
	if (_enable_normal_map) {
		_normal_map_desired_channels = desired_channels;
	}
}

void read_image(std::string& filename, int desired_channels, Image*& output_image) {
	output_image = new Image(filename, desired_channels);

}

void Material::bind() {
	Image* color = nullptr;
	Image* specular = nullptr;
	Image* normal = nullptr;

	std::vector<std::thread> task;
	if (_enable_color_map) {
		color_map.texture_slot = color_map_slot;
		if (color_map._load_image_check(true))
			task.push_back(std::thread(&read_image, std::ref(_color_map_filename), _color_map_desired_channels, std::ref(color)));
	}
	if (_enable_specular_map) {
		specular_map.texture_slot = specular_map_slot;
		if (specular_map._load_image_check(true))
			task.push_back(std::thread(&read_image, std::ref(_specular_map_filename), _specular_map_desired_channels, std::ref(specular)));
	}
	if (_enable_normal_map) {
		normal_map.texture_slot = normal_map_slot;
		if (normal_map._load_image_check(true))
			task.push_back(std::thread(&read_image, std::ref(_normal_map_filename), _normal_map_desired_channels, std::ref(normal)));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].join();
	}

	if (_enable_color_map) {
		if (color_map._load_image_check(true) && color != nullptr) {
			color_map.load_image(*color);
			delete color;
		}
		else
			color_map.bind();
	}
	if (_enable_specular_map) {
		if (specular_map._load_image_check(true) && specular != nullptr) {
			specular_map.load_image(*specular);
			delete specular;
		}
		else
			specular_map.bind();
	}
	if (_enable_normal_map) {
		if (normal_map._load_image_check(true) && normal != nullptr) {
			normal_map.load_image(*normal);
			delete normal;
		}
		else
			normal_map.bind();
	}
}

void Material::unbind() {
	if (_enable_color_map)
		color_map.unbind();
	if (_enable_specular_map)
		specular_map.unbind();
	if (_enable_normal_map)
		normal_map.unbind();
}
