#include "GUITextureBank.h"

#include "Package.h"
#include "Debuger.h"
#include <iostream>

std::shared_ptr<Texture2D> GUITextureBank::get_texture(
	const std::filesystem::path& filepath,
	Texture2D::ColorTextureFormat internal_format,
	Texture2D::ColorFormat format,
	Texture2D::Type type,
	glm::ivec2 target_resolution,
	int32_t mipmap,
	bool vertical_flip
) {
	
	if (created_textures.find(filepath) != created_textures.end() && !created_textures.at(filepath).expired())
		return created_textures.at(filepath).lock();
	
	Image image;

	if (Package::loaded_package != nullptr) {
		if (glm::all(glm::greaterThan(target_resolution, glm::ivec2(0)))) {
			
			Image::ImageParameters params;
			params.bytes_per_channel = 1;
			params.channel_count = 4;
			params.depth = 1;
			params.height = 0;
			params.width = 0;
			params.vertical_flip = true;
			params.path = "x.svg";
			params.source_data = (unsigned char*)Package::loaded_package->get(filepath).c_str();

			if (filepath.extension() == ".svg" || filepath.extension() == ".SVG") {
				image = Image(params);
				for (size_t pixel = 0; pixel < image.get_width() * image.get_height(); pixel++) {
					if (image.get_image_data()[pixel * image.get_channel_count() + 3] != 0) {
						image.get_image_data()[pixel * image.get_channel_count() + 0] = 255;
						image.get_image_data()[pixel * image.get_channel_count() + 1] = 255;
						image.get_image_data()[pixel * image.get_channel_count() + 2] = 255;
					}
				}
			}
			else 
				image = Image((unsigned char*)Package::loaded_package->get(filepath).c_str(), target_resolution.x, target_resolution.y, 1, Texture2D::ColorFormat_channels(format), Texture2D::Type_bytes_per_channel(type), vertical_flip);
		}
		else {
			std::cout << "[GUI Error] GUITextureBank::get_texture() is called with a package loaded but automatic resolution detection is not supported on files from a package" << std::endl;
			ASSERT(false);
		}
	}
	else {
		if (glm::all(glm::greaterThan(target_resolution, glm::ivec2(0))))
			image = Image(filepath.string(), target_resolution.x, target_resolution.y, Texture2D::ColorFormat_channels(format), Texture2D::Type_bytes_per_channel(type), vertical_flip);
		else
			image = Image(filepath.string(), Texture2D::ColorFormat_channels(format), vertical_flip);
	}
	
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(image.get_width(), image.get_height(), internal_format, mipmap, 0, 0);

	texture->is_bindless = true;
	texture->load_data_with_mipmaps(image, format, type);

	created_textures[filepath] = texture;
	return texture;
}