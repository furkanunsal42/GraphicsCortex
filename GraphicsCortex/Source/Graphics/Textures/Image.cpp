#include "Image.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"
#include "tiffio.h"

#include <filesystem>
#include "Debuger.h"

Image::Image(int width, int height, int channel_count, int byte_per_channel, bool vertical_flip) :
	_width(width), _height(height), _depth(1), _channel_count(channel_count), _bytes_per_channel(byte_per_channel), _vertical_flip(vertical_flip), _source_filepath("")
{

}

Image::Image(int width, int height, int depth, int channel_count, int byte_per_channel, bool vertical_flip) : 
	_width(width), _height(height), _depth(depth), _channel_count(channel_count), _bytes_per_channel(byte_per_channel), _vertical_flip(vertical_flip), _source_filepath("")
{

}

Image::Image(const std::string& file_path, int desired_channels, bool vertical_flip) :
	_source_filepath(file_path), _vertical_flip(vertical_flip)
{
	std::string type = std::filesystem::path(file_path).extension().string();
	if (type == ".raw" || type == ".RAW") {
		std::cout << "[Image Loading Error] Image constructor is called without spesifying width and height but can't read the metadata of .raw files. please choose a constructor with target width and height parameters or import other image formats." << std::endl;
		ASSERT(false);
	}

	int target_width, target_height, target_channel_count, target_bytes_per_channel;
	stbi_info(file_path.c_str(), &target_width, &target_height, &target_channel_count);
	target_bytes_per_channel = stbi_is_16_bit(_source_filepath.c_str()) ? 2 : 1;

	ImageParameters target_parameters;
	target_parameters.width = target_width;
	target_parameters.height = target_height;
	target_parameters.depth = 1;
	target_parameters.channel_count = desired_channels;
	target_parameters.bytes_per_channel = target_bytes_per_channel;
	target_parameters.path = file_path;
	target_parameters.vertical_flip = vertical_flip;

	_read_image_data(target_parameters);
}

Image::Image(const std::string& file_path, int target_width, int target_height, int channel_count, int byte_per_channel, bool vertical_flip) :
	_source_filepath(file_path), _vertical_flip(vertical_flip)
{
	ImageParameters target_parameters;
	target_parameters.width = target_width;
	target_parameters.height = target_height;
	target_parameters.depth = 1;
	target_parameters.channel_count = channel_count;
	target_parameters.bytes_per_channel = byte_per_channel;
	target_parameters.path = file_path;
	target_parameters.vertical_flip = vertical_flip;

	_read_image_data(target_parameters);
}

Image::Image(const std::string& file_path, int target_width, int target_height, int target_depth, int channel_count, int byte_per_channel, bool vertical_flip) :
	_source_filepath(file_path), _vertical_flip(vertical_flip)
{
	ImageParameters target_parameters;
	target_parameters.width = target_width;
	target_parameters.height = target_height;
	target_parameters.depth = target_depth;
	target_parameters.channel_count = channel_count;
	target_parameters.bytes_per_channel = byte_per_channel;
	target_parameters.path = file_path;
	target_parameters.vertical_flip = vertical_flip;

	_read_image_data(target_parameters);
}

Image::Image(unsigned char* image_data, int width, int height, int depth, int channel_count, int byte_per_channel, bool vertical_flip) : 
	_width(width), _height(height), _depth(depth), _channel_count(channel_count), _bytes_per_channel(byte_per_channel), _vertical_flip(vertical_flip), _source_filepath(""), _image_data(image_data)
{

}

Image::Image(ImageParameters parameters)
{
	_read_image_data(parameters);
}

Image::~Image() {
	release();
}

void Image::release() {
	if (_is_tiff) {
		TIFFClose(_tiff_handle);

	}
	else if (_image_is_loaded_from_stbi)
		stbi_image_free(_image_data);
	else
		delete[] _image_data;

	_tiff_handle = nullptr;
	_image_data = nullptr;

	_vertical_flip = true;
	_width = 0;
	_height = 0;
	_depth = 0;
	_channel_count = 0;
	_bytes_per_channel = 0;
	_image_is_loaded_from_stbi = false;
	_float_image = false;
	_is_tiff = false;
}

void Image::_read_image_data(const ImageParameters& requested_parameters)
{
	ASSERT(_image_data == nullptr);

	std::filesystem::path extension = std::filesystem::path(requested_parameters.path).extension();

	if (requested_parameters.path.size() >= 4) {
		std::string image_type = extension.string();
		if (image_type == ".raw" || image_type == ".RAW") {
			_read_image_data_raw(requested_parameters);
			return;
		}
	}

	if (requested_parameters.path.size() >= 5) {
		std::string image_type = extension.string();
		if (image_type == ".tiff" || image_type == ".TIFF") {
			_read_image_data_tiff(requested_parameters);
			return;
		}
	}
	
	if (extension == ".hdr" || extension == ".HDR"){
		_read_image_data_hdr(requested_parameters);
		return;
	}

	//_bytes_per_channel = stbi_is_16_bit(requested_parameters.path.c_str()) ? 2 : 1;
	_bytes_per_channel = requested_parameters.bytes_per_channel;
	
	stbi_set_flip_vertically_on_load_thread(_vertical_flip);
	if (_bytes_per_channel == 1)
		_image_data = stbi_load(requested_parameters.path.c_str(), &_width, &_height, &_channel_count, requested_parameters.channel_count);
	if (_bytes_per_channel == 2)
		_image_data = (unsigned char*)stbi_load_16(requested_parameters.path.c_str(), &_width, &_height, &_channel_count, requested_parameters.channel_count);

	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image read failed : " << requested_parameters.path << std::endl;
		ASSERT(false);
	}

	_channel_count = requested_parameters.channel_count;
	_depth = 1;
	
	_image_is_loaded_from_stbi = true;

	resize(requested_parameters.width, requested_parameters.height);
	resize_stride(requested_parameters.bytes_per_channel);
}

void Image::_read_image_data_raw(const ImageParameters& requested_parameters)
{
		std::ifstream file;
		file.open(requested_parameters.path, std::ios::in | std::ios::binary);
		if (file) {
			int file_size = requested_parameters.width * requested_parameters.height * requested_parameters.channel_count * requested_parameters.bytes_per_channel;
			_width = requested_parameters.width;
			_height = requested_parameters.height;
			_depth = requested_parameters.depth;
			_channel_count = requested_parameters.channel_count;
			_bytes_per_channel = requested_parameters.bytes_per_channel;
			_vertical_flip = requested_parameters.vertical_flip;
			_image_is_loaded_from_stbi = false;
			_image_data = (unsigned char*)malloc(file_size);
		}

		int file_size = requested_parameters.width * requested_parameters.height * requested_parameters.channel_count * requested_parameters.bytes_per_channel;
		int row_size = _width * _channel_count * _bytes_per_channel;

		int buffer_size = row_size;
		if (buffer_size <= 0) {
			std::cout << "[Image Loading Error] Image::_read_image_data() is called but image size couldn't determined properly. metadata of the target file format may not have determined properly" << std::endl;
			ASSERT(false);
		}

		int buffer_pointer = requested_parameters.vertical_flip ? file_size - row_size : 0;

		while (!file.eof()) {
			file.read((char*)_image_data + buffer_pointer, buffer_size);
			buffer_pointer += requested_parameters.vertical_flip ? -buffer_size : buffer_size;
		}
}

void Image::_read_image_data_tiff(const ImageParameters& requested_parameters)
{
	_tiff_handle = TIFFOpen(requested_parameters.path.c_str(), "r");

	TIFFGetField(_tiff_handle, TIFFTAG_IMAGEWIDTH, &_width);
	TIFFGetField(_tiff_handle, TIFFTAG_IMAGELENGTH, &_height);
	
	uint32_t bits_per_channel;
	TIFFGetField(_tiff_handle, TIFFTAG_BITSPERSAMPLE, &bits_per_channel);
	_bytes_per_channel = /*bits_per_channel / 8*/ 2;

	void* temp_image_data = _TIFFmalloc(_width * _height * sizeof(uint32_t));
	TIFFReadRGBAImage(_tiff_handle, _width, _height, (uint32_t*)temp_image_data, true);
	
	//std::cout << _width << " " << _height << " " << bits_per_channel << " " << (unsigned int*)_image_data << std::endl;

	_channel_count = 1;
	_image_data = new unsigned char[_width * _height * _bytes_per_channel * _channel_count];

	for (int byte = 0; byte < _width * _height * _bytes_per_channel; byte++) {
		int padded_byte = byte / _bytes_per_channel * sizeof(uint32_t) + byte % _bytes_per_channel;
		_image_data[byte] = ((unsigned char*)temp_image_data)[padded_byte];
	}

	_TIFFfree(temp_image_data);
	_is_tiff = false;
}

void Image::_read_image_data_hdr(const ImageParameters& requested_parameters)
{
	_bytes_per_channel = 4;
	stbi_set_flip_vertically_on_load_thread(_vertical_flip);
	
	_image_data = (uint8_t*)stbi_loadf(requested_parameters.path.c_str(), &_width, &_height, &_channel_count, requested_parameters.channel_count);
	std::cout << "channel count : " << _channel_count << std::endl;
	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image read failed : " << requested_parameters.path << std::endl;
		ASSERT(false);
	}

	_channel_count = requested_parameters.channel_count;
	_depth = 1;
	_image_is_loaded_from_stbi = true;
	_float_image = true;

	resize(requested_parameters.width, requested_parameters.height);
}

void Image::swap_endian()
{
	int image_size = _width * _height * _channel_count * _bytes_per_channel;
	
	if (_bytes_per_channel == 1) return;
	for (char* pixel = (char*)_image_data; (char*)pixel < ((char*)_image_data + image_size); pixel += _bytes_per_channel)
	{
		if (_bytes_per_channel == 2) {
			*(short*)pixel = (*(short*)pixel & 0x00FF) << 8  | (*(short*)pixel & 0xFF00) >> 8;
		}
		if (_bytes_per_channel == 4) {
			*(int*)pixel = (*(int*)pixel & 0x0000FFFF) << 16 | (*(int*)pixel & 0xFFFF0000) >> 16;
			*(int*)pixel = (*(int*)pixel & 0x00FF00FF) << 8  | (*(int*)pixel & 0xFF00FF00) >> 8;
		}
	}
}

void Image::resize(int target_width, int target_height) {
	
	if (_width == target_width && _height == target_height)
		return;

	unsigned char* resized_image = new unsigned char[target_width * target_height * _channel_count * _bytes_per_channel];
	
	if (_float_image)
		stbir_resize_float((float*)_image_data, _width, _height, _width * _channel_count * _bytes_per_channel, (float*)resized_image, target_width, target_height, target_width * _channel_count * _bytes_per_channel, _channel_count);
	if(!_float_image && _bytes_per_channel == 1)
		stbir_resize_uint8(_image_data, _width, _height, _width * _channel_count * _bytes_per_channel, resized_image, target_width, target_height, target_width * _channel_count * _bytes_per_channel, _channel_count);
	if (!_float_image && _bytes_per_channel == 2)
		stbir_resize_uint16_generic((unsigned short*)_image_data, _width, _height, _width * _channel_count * _bytes_per_channel, (unsigned short*)resized_image, target_width, target_height, target_width * _channel_count * _bytes_per_channel, _channel_count, -1, 0, STBIR_EDGE_CLAMP, STBIR_FILTER_DEFAULT, STBIR_COLORSPACE_LINEAR, 0);
	
	if (_is_tiff) {
		TIFFClose(_tiff_handle);

	}
	else if (_image_is_loaded_from_stbi)
		stbi_image_free(_image_data);
	else
		delete[] _image_data;

	_width = target_width;
	_height = target_height;
	_image_data = resized_image;
}

void Image::resize_stride(int target_bytes_per_channel)
{
}

void Image::save_to_disc(const std::string& target_filename) const
{
	if (std::filesystem::path(target_filename).has_parent_path())
		std::filesystem::create_directories(std::filesystem::path(target_filename).parent_path());

	if (target_filename.size() > 4) {
		if (target_filename.substr(target_filename.size() - 4, 4) == ".raw" || target_filename.substr(target_filename.size() - 4, 4) == ".RAW") {
			std::fstream file(target_filename, std::fstream::out | std::fstream::binary);
			for (int32_t z = 0; z < get_depth(); z++) {
				for (int32_t y = 0; y < get_height(); y++) {
					size_t y_to_use = get_vertical_flip() ? get_height() - 1 - y : y;
					size_t pixel_stride = get_channel_count() * get_byte_per_channel();
					size_t begin = (z * get_width() * get_height() + y_to_use * get_width()) * pixel_stride;
					file.write((const char*)_image_data + begin, get_width() * pixel_stride);
				}
			}
			return;
		}
	}

	stbi_flip_vertically_on_write(_vertical_flip);
	if (target_filename.size() > 4) {
		if (target_filename.substr(target_filename.size() - 4, 4) == ".jpg" || target_filename.substr(target_filename.size() - 4, 4) == ".JPG")
			int result_flag = stbi_write_jpg(target_filename.c_str(), _width, _height, _channel_count, _image_data, _width * _channel_count * _bytes_per_channel);
	}

	if (target_filename.size() > 4) {
		if (target_filename.substr(target_filename.size() - 4, 4) == ".png" || target_filename.substr(target_filename.size() - 4, 4) == ".PNG")
			int result_flag = stbi_write_png(target_filename.c_str(), _width, _height, _channel_count, _image_data, _width * _channel_count * _bytes_per_channel);
	}

	else {
		int result_flag = stbi_write_png(target_filename.c_str(), _width, _height, _channel_count, _image_data, _width * _channel_count * _bytes_per_channel);
	}
}



Image::Image(const Image& other)
{
	if (other._image_data != nullptr && other.get_size() != 0) {
		_image_data = (uint8_t*)malloc(other.get_size());
		if (_image_data != nullptr) {
			std::memcpy(_image_data, other._image_data, other.get_size());
		}
		else {
			std::cout << "Image::Image() memory allocation failed" << std::endl;
			ASSERT(false);
		}
	}

	_source_filepath = other._source_filepath;
	_vertical_flip = other._vertical_flip;
	_width = other._width;
	_height = other._height;
	_depth = other._depth;
	_channel_count = other._channel_count;
	_bytes_per_channel = other._bytes_per_channel;
	_image_is_loaded_from_stbi = other._image_is_loaded_from_stbi;

	_is_tiff = other._is_tiff;
	_tiff_handle = other._tiff_handle;
}

Image& Image::operator=(const Image& other) {
	release();

	if (other._image_data != nullptr && other.get_size() != 0) {
		_image_data = (uint8_t*)malloc(other.get_size());
		if (_image_data != nullptr) {
			std::memcpy(_image_data, other._image_data, other.get_size());
		}
		else {
			std::cout << "Image::Image() memory allocation failed" << std::endl;
			ASSERT(false);
		}
	}

	_source_filepath = other._source_filepath;
	_vertical_flip = other._vertical_flip;
	_width = other._width;
	_height = other._height;
	_depth = other._depth;
	_channel_count = other._channel_count;
	_bytes_per_channel = other._bytes_per_channel;
	_image_is_loaded_from_stbi = other._image_is_loaded_from_stbi;

	_is_tiff = other._is_tiff;
	_tiff_handle = other._tiff_handle;

	return *this;
}

Image::Image(Image&& other)
{
	_image_data = other._image_data;
	other._image_data = nullptr;

	_source_filepath = std::move(other._source_filepath);
	_vertical_flip = other._vertical_flip;
	_width = other._width;
	_height = other._height;
	_depth = other._depth;
	_channel_count = other._channel_count;
	_bytes_per_channel = other._bytes_per_channel;
	_image_is_loaded_from_stbi = other._image_is_loaded_from_stbi;

	_is_tiff = other._is_tiff;
	_tiff_handle = other._tiff_handle;
}

Image& Image::operator=(Image&& other) {
	
	release();

	_image_data = other._image_data;
	other._image_data = nullptr;

	_source_filepath = std::move(other._source_filepath);
	_vertical_flip = other._vertical_flip;
	_width = other._width;
	_height = other._height;
	_depth = other._depth;
	_channel_count = other._channel_count;
	_bytes_per_channel = other._bytes_per_channel;
	_image_is_loaded_from_stbi = other._image_is_loaded_from_stbi;

	_is_tiff = other._is_tiff;
	_tiff_handle = other._tiff_handle;

	return *this;
}

Image::ImageParameters Image::detect_image_parameters(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path)) {
		ImageParameters params;
		params.width = 0;
		params.height = 0;
		params.depth = 0;
		params.channel_count = 0;
		params.bytes_per_channel = 0;
		params.path = "";
		params.vertical_flip = false;
		return params;
	}

	int image_width, image_height, iamge_channel_count, image_bytes_per_channel;
	stbi_info(path.string().c_str(), &image_width, &image_height, &iamge_channel_count);
	image_bytes_per_channel = stbi_is_16_bit(path.string().c_str()) ? 2 : 1;

	ImageParameters params;
	params.width = image_width;
	params.height = image_height;
	params.depth = 1;
	params.channel_count = iamge_channel_count;
	params.bytes_per_channel = image_bytes_per_channel;
	params.path = path.string();
	params.vertical_flip = false;
	return params;
}

namespace {
	uint32_t Channel_to_offset(Image::Channel channel) {
		switch (channel) {
		case Image::red: return 0;
		case Image::green: return 1;
		case Image::blue: return 2;
		case Image::alpha: return 3;
		}
		return 0;
	}
}

Image Image::copy_channels(Channel new_red_source, Channel new_green_source, Channel new_blue_source, Channel new_alpha_source)
{
	const uint32_t new_channel_count = 4;

	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called on an empty Image" << std::endl;
		ASSERT(false);
	}
	
	if (new_channel_count > _channel_count) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called with " << new_channel_count << " channels but the source image channel count is : " << _channel_count << std::endl;
		ASSERT(false);
	}

	if (_bytes_per_channel == 0) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called but bytes_per_channel is 0" << std::endl;
		ASSERT(false);
	}

	uint32_t target_red_offset = Channel_to_offset(new_red_source);
	uint32_t target_green_offset = Channel_to_offset(new_green_source);
	uint32_t target_blue_offset = Channel_to_offset(new_blue_source);
	uint32_t target_alpha_offset = Channel_to_offset(new_alpha_source);

	size_t pixel_count = _width * _height * _depth;
	uint8_t* new_image_ptr = new uint8_t[_bytes_per_channel * new_channel_count * pixel_count];

	if (_bytes_per_channel == 1) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			new_image_ptr[pixel * 4 + 0] = this->_image_data[pixel * 4 + target_red_offset];
			new_image_ptr[pixel * 4 + 1] = this->_image_data[pixel * 4 + target_green_offset];
			new_image_ptr[pixel * 4 + 2] = this->_image_data[pixel * 4 + target_blue_offset];
			new_image_ptr[pixel * 4 + 3] = this->_image_data[pixel * 4 + target_alpha_offset];
		}
	}

	if (_bytes_per_channel == 2) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			((uint16_t*)(new_image_ptr))[pixel * 4 + 0] = ((uint16_t*)this->_image_data)[pixel * 4 + target_red_offset];
			((uint16_t*)(new_image_ptr))[pixel * 4 + 1] = ((uint16_t*)this->_image_data)[pixel * 4 + target_green_offset];
			((uint16_t*)(new_image_ptr))[pixel * 4 + 2] = ((uint16_t*)this->_image_data)[pixel * 4 + target_blue_offset];
			((uint16_t*)(new_image_ptr))[pixel * 4 + 3] = ((uint16_t*)this->_image_data)[pixel * 4 + target_alpha_offset];
		}
	}

	return Image(new_image_ptr, _width, _height, _depth, new_channel_count, _bytes_per_channel, _vertical_flip);
}

Image Image::copy_channels(Channel new_red_source, Channel new_green_source, Channel new_blue_source)
{
	const uint32_t new_channel_count = 3;

	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called on an empty Image" << std::endl;
		ASSERT(false);
	}

	if (new_channel_count > _channel_count) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called with " << new_channel_count << " channels but the source image channel count is : " << _channel_count << std::endl;
		ASSERT(false);
	}

	if (_bytes_per_channel == 0) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called but bytes_per_channel is 0" << std::endl;
		ASSERT(false);
	}

	uint32_t target_red_offset = Channel_to_offset(new_red_source);
	uint32_t target_green_offset = Channel_to_offset(new_green_source);
	uint32_t target_blue_offset = Channel_to_offset(new_blue_source);

	if (target_red_offset >= _channel_count || target_blue_offset >= _channel_count || target_green_offset >= _channel_count) {
		uint32_t max_channel = std::max(target_red_offset, std::max(target_blue_offset, target_green_offset));
		std::cout << "[OpenGL Error] Image::copy_channels() is called but indexed channel of " << max_channel << " is greater than source channel count " << _channel_count << std::endl;
		ASSERT(false);
	}

	size_t pixel_count = _width * _height * _depth;
	uint8_t* new_image_ptr = new uint8_t[_bytes_per_channel * new_channel_count * pixel_count];

	if (_bytes_per_channel == 1) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			new_image_ptr[pixel * new_channel_count + 0] = this->_image_data[pixel * _channel_count + target_red_offset];
			new_image_ptr[pixel * new_channel_count + 1] = this->_image_data[pixel * _channel_count + target_green_offset];
			new_image_ptr[pixel * new_channel_count + 2] = this->_image_data[pixel * _channel_count + target_blue_offset];
		}
	}

	if (_bytes_per_channel == 2) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 0] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_red_offset];
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 1] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_green_offset];
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 2] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_blue_offset];
		}
	}

	return Image(new_image_ptr, _width, _height, _depth, new_channel_count, _bytes_per_channel, _vertical_flip);
}

Image Image::copy_channels(Channel new_red_source, Channel new_green_source)
{
	const uint32_t new_channel_count = 2;

	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called on an empty Image" << std::endl;
		ASSERT(false);
	}

	if (new_channel_count > _channel_count) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called with " << new_channel_count << " channels but the source image channel count is : " << _channel_count << std::endl;
		ASSERT(false);
	}

	if (_bytes_per_channel == 0) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called but bytes_per_channel is 0" << std::endl;
		ASSERT(false);
	}

	uint32_t target_red_offset = Channel_to_offset(new_red_source);
	uint32_t target_green_offset = Channel_to_offset(new_green_source);

	if (target_red_offset >= _channel_count || target_green_offset >= _channel_count) {
		uint32_t max_channel = std::max(target_red_offset, target_green_offset);
		std::cout << "[OpenGL Error] Image::copy_channels() is called but indexed channel of " << max_channel << " is greater than source channel count " << _channel_count << std::endl;
		ASSERT(false);
	}

	size_t pixel_count = _width * _height * _depth;
	uint8_t* new_image_ptr = new uint8_t[_bytes_per_channel * new_channel_count * pixel_count];

	if (_bytes_per_channel == 1) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			new_image_ptr[pixel * new_channel_count + 0] = this->_image_data[pixel * _channel_count + target_red_offset];
			new_image_ptr[pixel * new_channel_count + 1] = this->_image_data[pixel * _channel_count + target_green_offset];
		}
	}

	if (_bytes_per_channel == 2) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 0] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_red_offset];
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 1] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_green_offset];
		}
	}

	return Image(new_image_ptr, _width, _height, _depth, new_channel_count, _bytes_per_channel, _vertical_flip);
}

Image Image::copy_channels(Channel new_red_source)
{
	const uint32_t new_channel_count = 1;

	if (_image_data == nullptr) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called on an empty Image" << std::endl;
		ASSERT(false);
	}

	if (new_channel_count > _channel_count) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called with " << new_channel_count << " channels but the source image channel count is : " << _channel_count << std::endl;
		ASSERT(false);
	}

	if (_bytes_per_channel == 0) {
		std::cout << "[OpenGL Error] Image::copy_channels() is called but bytes_per_channel is 0" << std::endl;
		ASSERT(false);
	}

	uint32_t target_red_offset = Channel_to_offset(new_red_source);

	if (target_red_offset >= _channel_count ) {
		uint32_t max_channel = target_red_offset;
		std::cout << "[OpenGL Error] Image::copy_channels() is called but indexed channel of " << max_channel << " is greater than source channel count " << _channel_count << std::endl;
		ASSERT(false);
	}

	size_t pixel_count = _width * _height * _depth;
	uint8_t* new_image_ptr = new uint8_t[_bytes_per_channel * new_channel_count * pixel_count];

	if (_bytes_per_channel == 1) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			new_image_ptr[pixel * new_channel_count + 0] = this->_image_data[pixel * _channel_count + target_red_offset];
		}
	}

	if (_bytes_per_channel == 2) {
		for (size_t pixel = 0; pixel < pixel_count; pixel++) {
			((uint16_t*)(new_image_ptr))[pixel * new_channel_count + 0] = ((uint16_t*)this->_image_data)[pixel * _channel_count + target_red_offset];
		}
	}

	return Image(new_image_ptr, _width, _height, _depth, new_channel_count, _bytes_per_channel, _vertical_flip);
}

unsigned char* Image::get_image_data() {
	return _image_data;
}

int Image::get_width() const {
	return _width;
}

int Image::get_height() const {
	return _height;
}

int Image::get_depth() const {
	return _depth;
}

int Image::get_channel_count() const {
	return _channel_count;
}

int Image::get_byte_per_channel() const {
	return _bytes_per_channel;
}

size_t Image::get_size() const {
	return _width * _height * _channel_count * _bytes_per_channel;
}

bool Image::get_vertical_flip() const {
	return _vertical_flip;
}