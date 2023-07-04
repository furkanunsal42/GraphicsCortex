#include "GraphicsCortex.h"

int main() {
	for (int i = 0; i < 1000000; i++) {
		Vehicle v;
		if ( i % 10000 == 0)
			std::cout << i << std::endl;
	}

	
	//Image image = AssetImporter::read_image_cached("Images/orange.png", 3);
	//image.save_to_disc("cached_image.png");

	//Image image("Images/orange.png", 4, true);
	//
	//std::ofstream file("buffered_image.gcimage", std::ios::binary | std::ios::out);
	//file << (int)image.get_size() << ' ';
	//file << image.get_width() << ' ';
	//file << image.get_height() << ' ';
	//file << image.get_channels() << ' ';
	//file << image.get_vertical_flip() << ' ';
	//save_buffer_to_disc("buffered_image.gcimage", image.get_image_data(), image.get_size(), true);
	//file.close();
	//
	//std::ifstream ifile("buffered_image.gcimage", std::ios::binary | std::ios::in);
	//size_t image_size;
	//int width, height, channels;
	//bool flip;
	//ifile >> image_size;
	//ifile >> width;
	//ifile >> height;
	//ifile >> channels;
	//ifile >> flip;
	//unsigned char* memory = (unsigned char*)malloc(image_size);
	//read_buffer_from_disc("buffered_image.gcimage", memory, image_size);
	//image._image_data = memory;
	//image.save_to_disc("swiched_buffer.png");

	std::cin.get();
}
