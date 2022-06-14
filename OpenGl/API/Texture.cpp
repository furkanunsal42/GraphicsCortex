#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"


Texture::Texture():
	id(0), width(0), height(0), channels(0), image_data(nullptr) { }

void Texture::load_image(std::string file_path) {
	stbi_set_flip_vertically_on_load(vertical_flip);
	image_data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
	GLCall(glGenTextures(1, &id));
}

void Texture::bind() {
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
