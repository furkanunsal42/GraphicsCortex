#pragma once 

#include <string>
#include "tiffio.h"

class Framebuffer;

class Image {
	friend Framebuffer;
public:
	struct ImageParameters {
		int width;
		int height;
		int depth;
		int channel_count;
		int bytes_per_channel;
		std::string path;
		bool vertical_flip;
	};

	Image(int width, int height, int channel_count, int byte_per_channel, bool vertical_flip);
	Image(int width, int height, int depth, int channel_count, int byte_per_channel, bool vertical_flip);
	Image(const std::string& file_path, int desired_channels = 4, bool vertical_flip = false);
	Image(const std::string& file_path, int target_width, int target_height, int channel_count, int byte_per_channel, bool vertical_flip);
	Image(const std::string& file_path, int target_width, int target_height, int target_depth, int channel_count, int byte_per_channel, bool vertical_flip);
	Image(unsigned char* image_data, int width, int height, int depth, int channel_count, int byte_per_channel, bool vertical_flip);
	Image(ImageParameters parameters);
	~Image();
	void release();

	//Image(const std::string& file_path, int desired_channels = 4, bool vertical_flip = true);
	Image() = default;
	Image(const Image& copy_image);
	Image(Image&& other);

	uint8_t* get_image_data();
	int get_width() const;
	int get_height() const;
	int get_depth() const;
	int get_channel_count() const;
	int get_byte_per_channel() const;
	bool get_vertical_flip() const;
	size_t get_size() const;
	
	void swap_endian();
	void resize(int target_width, int target_height);
	void resize_stride(int target_bytes_per_channel);
	void save_to_disc(const std::string& target_filename) const;

	uint8_t* _image_data = nullptr;

	void _read_image_data(const ImageParameters& requested_parameters);
	void _read_image_data_raw(const ImageParameters& requested_parameters);
	void _read_image_data_tiff(const ImageParameters& requested_parameters);

	std::string _source_filepath = "";
	bool _vertical_flip = true;
	int _width = 0;
	int _height = 0;
	int _depth = 0;
	int _channel_count = 0;
	int _bytes_per_channel = 0;
	bool _image_is_loaded_from_stbi = false;

	bool _is_tiff = false;
	TIFF* _tiff_handle = nullptr;
private:

	//void _read_image(const std::string& file_path, int desired_channels = 4);
	//void _clear_ram();
};
