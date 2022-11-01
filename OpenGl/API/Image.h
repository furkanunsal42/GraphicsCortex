#pragma once 

#include <string>

class Texture;
class FrameBuffer;

class Image {
public:
	Image(const std::string& file_path, int desired_channels = 4, bool vertical_flip = true);
	Image(const Image& copy_image);
	Image(Image&& move_image);

	~Image();

	unsigned char* get_image_data();
	int get_width();
	int get_height();
	int get_channels();
	bool get_vertical_flip();
	size_t get_size();

	friend Texture;
	friend FrameBuffer;

	void save_to_disc(const std::string& target_filename);

	Image(unsigned char* image_data, int width, int height, int channels, bool vertical_flip);
private:

	bool _vertical_flip = true;
	int _width = NULL, _height = NULL, _channels = NULL;
	unsigned char* _image_data = nullptr;
	void _read_image(const std::string& file_path, int desired_channels = 4);
	void _clear_ram();
};
