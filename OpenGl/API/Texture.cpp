#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"


Texture::Texture():
	id(0), width(0), height(0), channels(0), image_data(nullptr) { }

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
	GLCall(glGenTextures(1, &id));
}

void Texture::bind(unsigned short texture_slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Material::Material() :
	color_map(Texture()), specular_map(Texture()), normal_map(Texture()) {}

Material::Material(Texture color, Texture specular, Texture normal) :
	color_map(color), specular_map(specular), normal_map(normal) {}

void Material::bind() {
	color_map.bind(color_map_slot);
	specular_map.bind(specular_map_slot);
	normal_map.bind(normal_map_slot);
}

void Material::unbind() {
	color_map.unbind();
	specular_map.unbind();
	normal_map.unbind();
}