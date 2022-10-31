#include "Image.h"

#include <iostream>

#include "stb_image.h"
#include "stb_image_write.h"
	
#include "Debuger.h"

Image::Image(const std::string& file_path, int desired_channels, bool vertical_flip) :
	_vertical_flip(vertical_flip)
{
	_read_image(file_path, desired_channels);
}

Image::Image(const Image& copy_image) :
	_width(copy_image._width), _height(copy_image._height), _channels(copy_image._channels)
{
	size_t buffer_size = copy_image._width * copy_image._height * copy_image._channels;
	_image_data = new unsigned char[buffer_size];
	std::memcpy(copy_image._image_data, _image_data, buffer_size);
}

Image::Image(Image&& move_image) :
	_width(move_image._width), _height(move_image._height), _channels(move_image._channels), _image_data(move_image._image_data)
{
	move_image._image_data = nullptr;
}

Image::~Image() {
	_clear_ram();
}

void Image::_read_image(const std::string& file_path, int desired_channels) {
	if (_image_data != nullptr){
		std::cout << "Image::_read_image() was called but _image_data was already initialized" << std::endl;
		ASSERT(false);
	}

	stbi_set_flip_vertically_on_load(_vertical_flip);

	_image_data = stbi_load(file_path.c_str(), &_width, &_height, &_channels, desired_channels);
	
	_channels = desired_channels;
}

void Image::_clear_ram() {
	stbi_image_free(_image_data);
}

unsigned char* Image::get_image_data() {
	return _image_data;
}
int Image::get_width(){
	return _width;
}
int Image::get_height(){
	return _height;
}
int Image::get_channels() {
	return _channels;
}

size_t Image::get_size() {
	return _width * _height * _channels;
}

// Image will take ownership of the data
Image::Image(unsigned char* image_data, int width, int height, int channels, bool vertical_flip) :
	_image_data(image_data), _width(width), _height(height), _channels(channels), _vertical_flip(vertical_flip) 
{ 
	if (image_data == nullptr) {
		ASSERT(false);
	}
	if (width == NULL) {
		ASSERT(false);
	}
	if (height == NULL) {
		ASSERT(false);
	}
	if (channels == NULL) {
		ASSERT(false);
	}

	image_data = nullptr;
}


void Image::save_to_disc(const std::string& target_filename) {
	stbi_flip_vertically_on_write(_vertical_flip);
	int result_flag = stbi_write_png(target_filename.c_str(), _width, _height, _channels, _image_data, _width * _channels);
}
