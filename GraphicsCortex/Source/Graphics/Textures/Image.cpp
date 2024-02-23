#include "Image.h"

#include <iostream>
#include <fstream>

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include "DirectoryUtils.h"
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
	std::memcpy(_image_data, copy_image._image_data, buffer_size);
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
	if (_image_data != nullptr) {
		std::cout << "Image::_read_image() was called but _image_data was already initialized" << std::endl;
		ASSERT(false);
	}

	if (file_path.size() >= 4) {
		std::string image_type = compute_filetype(file_path);
		if (image_type == ".raw" || image_type == ".RAW") {

			std::ifstream file;
			file.open(file_path, std::ios::in | std::ios::binary);
			if (file) {
				int file_size = 2048 * 2048 * 2;
				_width = 2048;
				_height = 2048;
				_channels = 1;
				_image_data = new unsigned char[file_size];
			}

			int buffer_size = 256;
			int buffer_pointer = 0;
			while (!file.eof()) {
				file.read((char*)_image_data + buffer_pointer, buffer_size);
				buffer_pointer += buffer_size;

			}

			return;
		}
	}

	//stbi_set_flip_vertically_on_load(_vertical_flip);	// causes wrong flags to be used while running in multithreaded mode
	stbi_set_flip_vertically_on_load_thread(_vertical_flip);	// may not link if thread-local variables aren't supported by compiler
	_image_data = stbi_load(file_path.c_str(), &_width, &_height, &_channels, desired_channels);
	_channels = desired_channels;
}

void Image::resize(int target_width, int target_height) {
	//if (_width == target_width && _height == target_height)
	//	return;
	unsigned char* resized_image = (unsigned char*)malloc(target_width * target_height * _channels);
	stbir_resize_uint8(_image_data, _width, _height, _channels * _width, resized_image, target_width, target_height, _channels * target_width, _channels);
	stbi_image_free(_image_data);
	_width = target_width;
	_height = target_height;
	_image_data = resized_image;
}

void Image::_clear_ram() {
	stbi_image_free(_image_data);
}

//void Image::flip_verticaly() {
//	
//	unsigned char* flipped_data = new unsigned char[get_size()];
//
//	for (int y = 0; y < _height; y++) {
//		for (int x = 0; x < _width; x++) {
//			for (int ch = 0; ch < _channels; ch++)
//				flipped_data[((_height - 1 - y) * _width + x) * _channels + ch] = flipped_data[(y * _width + x) * _channels + ch];
//		}
//	}
//
//
//}


unsigned char* Image::get_image_data() {
	return _image_data;
}
int Image::get_width() const{
	return _width;
}
int Image::get_height() const {
	return _height;
}
int Image::get_channels() const {
	return _channels;
}

size_t Image::get_size() const {
	return _width * _height * _channels;
}

bool Image::get_vertical_flip() const {
	return _vertical_flip;
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
}

void Image::save_to_disc(const std::string& target_filename) const{
	stbi_flip_vertically_on_write(_vertical_flip);
	int result_flag = stbi_write_png(target_filename.c_str(), _width, _height, _channels, _image_data, _width * _channels);
}
