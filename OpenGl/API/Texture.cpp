#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"
#include "AssetImporter.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>

TextureBase::TextureBase(int multisample) {
	//std::cout << "TextureBase classes cannot be constructed" << std::endl;
	//ASSERT(false);
}

TextureBase::~TextureBase() {
	//std::cout << "TextureBase classes cannot be deconstructed" << std::endl;
	//ASSERT(false);
}

void TextureBase::release() {
	std::cout << "TextureBase classes cannot released" << std::endl;
	ASSERT(false);
}

void TextureBase::bind() {
	std::cout << "TextureBase classes cannot binded" << std::endl;
	ASSERT(false);
}

void TextureBase::unbind() {
	std::cout << "TextureBase classes cannot unbinded" << std::endl;
	ASSERT(false);
}

void TextureBase::print_info(unsigned int opengl_code) {
	std::cout << "TextureBase classes cannot print info" << std::endl;
	ASSERT(false);
}

bool TextureBase::is_loaded() {
	std::cout << "TextureBase classes cannot load" << std::endl;
	ASSERT(false);
	return false;
}

// ----------------------------------------------------------------------------------

Texture::Texture(int multisample){
	multisample_amount = multisample;
}

Texture::~Texture() {
	release();
}

void Texture::release()
{
	GLCall(glDeleteTextures(1, &id));
}

bool Texture::is_initialized() {
	return _is_initialized;
}

void Texture::generate_texture_object() {
	if (is_initialized())
		return;

	if (multisample_amount == 0)
		target = GL_TEXTURE_2D;
	else
		target = GL_TEXTURE_2D_MULTISAMPLE;

	GLCall(glGenTextures(1, &id));

	_is_initialized = true;
}


bool Texture::_load_image_check(bool print_errors) {
	if (_loaded_on_gpu) {
		return false;
	}

	return true;
}

void Texture::load_image(Image& image) {
	if (!_load_image_check(true))
		return;

	generate_texture_object();

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
}

void Texture::initialize_blank_image(int width, int height) {

	generate_texture_object();

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
	else if(target == GL_TEXTURE_2D_MULTISAMPLE) {
		GLCall(glTexImage2DMultisample(target, multisample_amount, internal_format, this->width, this->height, GL_TRUE));
	}
	else {
		std::cout << "Texture::initialize_blank_image() was called but target is not supported." << std::endl;
		ASSERT(false);
	}
}

void Texture::bind() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
}

void Texture::unbind() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, 0));
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

RenderBuffer::RenderBuffer(int multisample) {
	multisample_amount = multisample;
}

RenderBuffer::~RenderBuffer() {
	release();
}

void RenderBuffer::generate_texture_object() {
	if (is_initialized())
		return;

	if (multisample_amount == 0)
		target = GL_RENDERBUFFER;
	else
		target = GL_RENDERBUFFER_SAMPLES;

	GLCall(glGenRenderbuffers(1, &id));

	_is_initialized = true;
}

void RenderBuffer::release() {
	GLCall(glDeleteRenderbuffers(1, &id));
}

void RenderBuffer::initialize_blank_image(int width, int height) {

	generate_texture_object();

	bind();
	if (multisample_amount == 0) {
		GLCall(glRenderbufferStorage(target, internal_format, width, height));
	}
	else {
		GLCall(glRenderbufferStorageMultisample(target, multisample_amount, internal_format, width, height));
	}
}

void RenderBuffer::bind() {
	GLCall(glBindRenderbuffer(target, id));
}

void RenderBuffer::unbind() {
	GLCall(glBindRenderbuffer(target, 0));
}

bool RenderBuffer::is_initialized() {
	return _is_initialized;
}

Image RenderBuffer::save(bool vertical_flip) {
	bind();

	int w, h;
	int image_internal_format = internal_format;
	int image_compressed = false;

	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w));
	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h));

	int image_size = w * h * channels * sizeof(unsigned char);

	if (compress_image) {
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &image_size));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED, &image_compressed));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &image_internal_format));
	}

	unsigned char* i_pixels = new unsigned char[image_size / sizeof(unsigned char)];

	if (compress_image) {
		GLCall(glGetCompressedTexImage(target, 0, i_pixels));
	}
	else {
		GLCall(glGetTexImage(target, 0, format, data_type, i_pixels));
	}

	return Image(i_pixels, w, h, channels, vertical_flip);

	//if (compress_image){
	//	GLCall(glCompressedTexImage2D(target, 0, image_internal_format, width, height, 0, image_size, i_pixels));
	//}
	//else {
	//	GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, i_pixels));
	//}
}

void RenderBuffer::print_info(unsigned int opengl_code) {
	bind();
	int data;
	GLCall(glGetTexLevelParameteriv(target, 0, opengl_code, &data));
	std::cout << "[INFO] Texture::print_info returns " << data << std::endl;
}

bool RenderBuffer::is_loaded() {
	return _loaded_on_gpu;
}

// ----------------------------------------------------------------------------------
TextureArray::TextureArray(int multisample) {
	multisample_amount = multisample;
}

TextureArray::~TextureArray() {
	release();
}

void TextureArray::generate_texture_object() {
	if (is_initialized())
		return;
	
	if (multisample_amount == 0)
		target = GL_TEXTURE_2D_ARRAY;
	else
		target = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;

	multisample_amount = multisample_amount;

	GLCall(glGenTextures(1, &id));

	_is_initialized = true;
}

void TextureArray::release() {
	GLCall(glDeleteTextures(1, &id));
}

bool TextureArray::is_initialized() {
	return _is_initialized;
}

void TextureArray::load_single_image(Image& image, int index) {

	generate_texture_object();

	if (width == NULL && height == NULL && channels == NULL) {
		width = image.get_width();
		height = image.get_height();
		channels = image.get_channels();
		
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
	}
	else if (width != image.get_width() || height != image.get_height() || channels != image.get_channels()) {
		std::cout << "[OpenGL ERROR] TextureArray::load_single_image() was called but given image doesn't match TextureArray's dimentions." << std::endl;
		//ASSERT(false);
	}

	bind();

	//if (compress_image)
		//glCompressedTexImage2D(target, 0, internal_format, width, height, 0, );
	//else 
	GLCall(glTexSubImage3D(target, 0, 0, 0, index, image.get_width(), image.get_height(), 1, format, data_type, image.get_image_data()));

	if (generate_mipmap)
		GLCall(glGenerateMipmap(target));

	_loaded_on_gpu = true;
}

void TextureArray::load_images(std::vector<Image>& images) {

	generate_texture_object();

	if (images.size() == 0) {
		std::cout << "[OpenGL Warning] TextureArray::load_images() was called with empty image array." << std::endl;
		Image default_image = Image("Images/missing_texture.png");
		initialize_blank_images(default_image.get_width(), default_image.get_height(), 1, default_image.get_channels());
		load_single_image(default_image, 0);
		return;
	}

	depth = images.size();
	channels = images[0].get_channels();

	if (images[0].get_channels() == 3) {
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
	else if (images[0].get_channels() == 4) {
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
	initialize_blank_images(images[0].get_width(), images[0].get_height(), depth, images[0].get_channels());
	for (int i = 0; i < images.size(); i++) {
		load_single_image(images[i], i);
	}
	_loaded_on_gpu = true;
}

void TextureArray::initialize_blank_images(int width, int height, int depth, int channels) {

	generate_texture_object();

	this->width = width;
	this->height = height;
	this->depth = depth;
	this->channels = channels;

	if (channels == 3) {
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
	else if (channels == 4) {
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

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	if (target == GL_TEXTURE_2D_ARRAY) {
		GLCall(glTexImage3D(target, 0, internal_format, this->width, this->height, this->depth, 0, format, data_type, NULL));
		GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
		if (generate_mipmap) {
			GLCall(glGenerateMipmap(target));
		}
	}
	else if (target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY) {
		GLCall(glTexImage3DMultisample(target, multisample_amount, internal_format, this->width, this->height, this->depth, GL_TRUE));
	}
	else {
		std::cout << "TextureArray::initialize_blank_image() was called but target is not supported." << std::endl;
		ASSERT(false);
	}
}

void TextureArray::bind() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
}

void TextureArray::unbind() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, 0));
}

Image TextureArray::save(int index, bool vertical_flip) {
	bind();
	if (index >= 0) {
		int w, h;

		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h));

		int image_size = w * h * channels;

		unsigned char* i_pixels = new unsigned char[image_size];

		GLCall(glGetTextureSubImage(id, 0, 0, 0, index, w, h, 1, format, data_type, image_size, i_pixels));

		return Image(i_pixels, w, h, channels, vertical_flip);
	}
	else if (index == -1) {
		int w, h;

		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h));

		int image_size = w * h * channels * depth;

		unsigned char* i_pixels = new unsigned char[image_size];

		GLCall(glGetTextureSubImage(id, 0, 0, 0, 0, w, h, depth, format, data_type, image_size, i_pixels));

		return Image(i_pixels, w, h * depth, channels, vertical_flip);

	}
}

void TextureArray::print_info(unsigned int opengl_code) {
	bind();
	int data;
	GLCall(glGetTexLevelParameteriv(target, 0, opengl_code, &data));
	std::cout << "[INFO] Texture::print_info returns " << data << std::endl;
}

bool TextureArray::is_loaded() {
	return _loaded_on_gpu;
}
	
// ------------------------------------------------------------------------------------

UnorderedMaterial::UnorderedMaterial() : 
	array_size(0) { }

UnorderedMaterial::UnorderedMaterial(int size) :
	array_size(size) 
{
	for (int i = 0; i < size; i++) {
		_is_texture_loaded.push_back(false);
		_texture_desired_channels.push_back(NULL);
		_texture_filenames.push_back("");
	}
} 
s
UnorderedMaterial::~UnorderedMaterial() {
	texture_array.~TextureArray();
}

UnorderedMaterial::UnorderedMaterial(const std::string& filename) :
	UnorderedMaterial(std::move(AssetImporter::generate_material(filename))) { }

namespace {
	void read_image(std::string& filename, int desired_channels, Image*& output_image, unsigned int texture_width, unsigned int texture_height) {
		std::ifstream file;
		file.open(filename);
		if (file) {
			output_image = new Image(filename, desired_channels);
			//Image&& image = AssetImporter::read_image_cached(filename, desired_channels);
			//output_image = &image;
		}
		else { // file doesn't exist
			std::cout << "[ERROR] Image path not found : " << filename << std::endl;
			output_image = new Image("Images/missing_texture.png", desired_channels);
			//Image&& image = AssetImporter::read_image_cached("Images/missing_texture.png", desired_channels);
			//output_image = &image;
		}
		output_image->resize(texture_width, texture_height);

	}
}

void UnorderedMaterial::set_texture(const std::string& filename, int desired_channels, int index) {
	_texture_filenames[index] = filename;
	_texture_desired_channels[index] = desired_channels;
	_is_texture_loaded[index] = false;
}

void UnorderedMaterial::bind() {
	std::vector<Image*> images;
	for (int i = 0; i < array_size; i++) {
		images.push_back(nullptr);
	}

	/*
	for (int i = 0; i < _texture_filenames.size(); i++) {
		std::fstream file;
		file.open(_texture_filenames[i]);
		if (!file) {
			std::cout << "Repleace the filename " << _texture_filenames[i] << " with : " << std::endl;
			std::string input;
			std::cin >> input;
			_texture_filenames[i] = input;
		}
		else {
			file.close();
		}
	}
	*/

	std::vector<std::thread> task;
	for (int i = 0; i < array_size; i++) {
		if (!_is_texture_loaded[i])
			task.push_back(std::thread(&read_image, std::ref(_texture_filenames[i]), _texture_desired_channels[i], std::ref(images[i]), texture_width, texture_height));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].join();
	}

	if (_first_texture_set) {
		for (Image*& image : images) {
			if (image != nullptr) {
				_first_texture_set = false;
				texture_array.initialize_blank_images(image->get_width(), image->get_height(), array_size, image->get_channels());
				break;
			}
		}
	}

	texture_array.texture_slot = material_texture_slot;
	for (int i = 0; i < array_size; i++) {
		if (images[i] != nullptr) {
			texture_array.load_single_image(*images[i], i);
			delete images[i];
			_is_texture_loaded[i] = true;
		}
		else
			texture_array.bind();
	}
}

void UnorderedMaterial::unbind() {
	texture_array.unbind();
}

void UnorderedMaterial::set_texture_width(unsigned int width) {
	texture_width = width;
}

void UnorderedMaterial::set_texture_height(unsigned int height) {
	texture_height = height;
}

void UnorderedMaterial::set_texture_size(unsigned int width, unsigned int height) {
	set_texture_width(width);
	set_texture_height(height);
}

unsigned int UnorderedMaterial::get_texture_width() {
	return texture_width;
}

unsigned int UnorderedMaterial::get_texture_height() {
	return texture_height;
}

// ------------------------------------------------------------------------------------

Material::Material() : UnorderedMaterial(3) { }

Material::~Material() {
	texture_array.~TextureArray();
}

void Material::set_color_texture(const std::string& filename, int desired_channels){
	_enable_color_map = filename != "";
	_texture_filenames[0] = filename;
	if (_enable_color_map) {
		_texture_desired_channels[0] = desired_channels;
	}
	_is_texture_loaded[0] = false;
}
void Material::set_specular_texture(const std::string& filename, int desired_channels){
	_enable_specular_map = filename != "";
	_texture_filenames[1] = filename;
	if (_enable_specular_map) {
		_texture_desired_channels[1] = desired_channels;
	}
	_is_texture_loaded[1] = false;

}
void Material::set_normal_texture(const std::string& filename, int desired_channels) {
	_enable_normal_map = filename != "";
	_texture_filenames[2] = filename;
	if (_enable_normal_map) {
		_texture_desired_channels[2] = desired_channels;
	}
	_is_texture_loaded[2] = false;
}

void Material::bind() {
	Image* color = nullptr;
	Image* specular = nullptr;
	Image* normal = nullptr;

	std::vector<std::thread> task;
	if (_enable_color_map) {
		texture_array.texture_slot = material_texture_slot;
		if (!_is_texture_loaded[0])
			task.push_back(std::thread(&read_image, std::ref(_texture_filenames[0]), _texture_desired_channels[0], std::ref(color), texture_width, texture_height));
	}
	if (_enable_specular_map) {
		texture_array.texture_slot = material_texture_slot;
		if (!_is_texture_loaded[1])
			task.push_back(std::thread(&read_image, std::ref(_texture_filenames[1]), _texture_desired_channels[1], std::ref(specular), texture_width, texture_height));
	}
	if (_enable_normal_map) {
		texture_array.texture_slot = material_texture_slot;
		if (!_is_texture_loaded[2])
			task.push_back(std::thread(&read_image, std::ref(_texture_filenames[2]), _texture_desired_channels[2], std::ref(normal), texture_width, texture_height));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].join();
	}

	if (_first_texture_set) {
		if (color != nullptr) {
			_first_texture_set = false;
			texture_array.initialize_blank_images(color->get_width(), color->get_height(), array_size, color->get_channels());
		}
		else if (specular != nullptr) {
			_first_texture_set = false;
			texture_array.initialize_blank_images(specular->get_width(), specular->get_height(), array_size, specular->get_channels());
		}
		else if (normal != nullptr) {
			_first_texture_set = false;
			texture_array.initialize_blank_images(normal->get_width(), normal->get_height(), array_size, normal->get_channels());
		}
	}

	if (_enable_color_map) {
		texture_array.texture_slot = material_texture_slot;
		if (color != nullptr) {
			texture_array.load_single_image(*color, 0);
			_is_texture_loaded[0] = true;
			delete color;
		}
		else
			texture_array.bind();
	}
	if (_enable_specular_map) {
		texture_array.texture_slot = material_texture_slot;
		if (specular != nullptr) {
			texture_array.load_single_image(*specular, 1);
			_is_texture_loaded[1] = true;
			delete specular;
		}
		else
			texture_array.bind();
	}
	if (_enable_normal_map) {
		texture_array.texture_slot = material_texture_slot;
		if (normal != nullptr) {
			texture_array.load_single_image(*normal, 2);
			_is_texture_loaded[2] = true;
			delete normal;
		}
		else
			texture_array.bind();
	}
}

void Material::unbind() {
	texture_array.unbind();
}
