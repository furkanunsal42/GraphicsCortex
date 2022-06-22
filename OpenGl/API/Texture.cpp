#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"

Texture::Texture() {
	GLCall(glGenTextures(1, &id));
}

void Texture::load_image(std::string file_path, int desired_channels) {
	stbi_set_flip_vertically_on_load(vertical_flip);

	image_data = stbi_load(file_path.c_str(), &width, &height, &channels, desired_channels);
	if (desired_channels == 3) {
		if (format == NULL)
			format = GL_RGB;
		if (internal_format == NULL)
			internal_format = GL_RGB8;
	}
	else if (desired_channels == 4) {
		if (format == NULL)
			format = GL_RGBA;
		if (internal_format == NULL)
			internal_format = GL_RGBA8;
	}
	//GLCall(glGenTextures(1, &id));
}

void Texture::free_image() {
	stbi_image_free(image_data);
}

void Texture::initialize_blank_image() {
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
}


void Texture::bind(unsigned short texture_slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, image_data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Material::Material()
{
	/*
	Texture a;
	Texture b;
	Texture c;
	color_map = &a;
	specular_map = &b; 
	normal_map = &c;
	*/
}

Material::Material(Texture& color, Texture& specular, Texture& normal) :
	color_map(&color), specular_map(&specular), normal_map(&normal) {}

void Material::bind() {
	if (color_map != nullptr)
		color_map->bind(color_map_slot);
	if (specular_map != nullptr)
		specular_map->bind(specular_map_slot);
	if (normal_map != nullptr)
		normal_map->bind(normal_map_slot);
}

void Material::unbind() {
	if (color_map != nullptr)
		color_map->unbind();
	if (specular_map != nullptr)
		specular_map->unbind();
	if (normal_map != nullptr)
	normal_map->unbind();
}