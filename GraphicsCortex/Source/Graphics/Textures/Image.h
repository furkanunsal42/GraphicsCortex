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

	//void flip_verticaly();
	unsigned char* get_image_data();
	int get_width() const;
	int get_height() const;
	int get_channels() const;
	bool get_vertical_flip() const;
	size_t get_size() const;

	friend Texture;
	friend FrameBuffer;

	void resize(int target_width, int target_height);
	void save_to_disc(const std::string& target_filename) const;

	Image(unsigned char* image_data, int width, int height, int channels, bool vertical_flip);
	unsigned char* _image_data = nullptr;
private:

	bool _vertical_flip = true;
	int _width = NULL, _height = NULL, _channels = NULL;
	void _read_image(const std::string& file_path, int desired_channels = 4);
	void _clear_ram();
};
