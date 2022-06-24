
#include "CubeMap.h"

#include "stb_image.h"

#include "Debuger.h"
#include <iostream>

CubeMapTexture::CubeMapTexture() {
	GLCall(glGenTextures(1, &id));
}

void CubeMapTexture::load_data(int desired_channels, bool free_ram) {
	for (std::string file_path : face_texture_filepaths) {
		if (file_path == ""){
			std::cout << "[Opengl Error] At least one of the texture file paths is not specified for CubeMapTexture.load_data" << std::endl;
			return;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (image_data[i] != nullptr)
			stbi_image_free(image_data[i]);
		
		stbi_set_flip_vertically_on_load(vertical_flip[i]);

		image_data[i] = stbi_load(face_texture_filepaths[i].c_str(), &(width[i]), &(height[i]), &(channels[i]), desired_channels);

		if (!image_data[i]) {
			const char* sides[6] = { "RIGHT", "LEFT", "TOP", "BOTTOM", "FRONT", "BACK" };
			std::cout << "[Opengl Error] Error on loading iamge " << sides[i] << " while executing CubeMapTexture.load_data" << std::endl;
			return;
		}
	}

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
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r));

	for (int i = 0; i < 6; i++) {
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width[i], height[i], 0, format, data_type, image_data[i]));
	}
	if (free_ram)
		free_image_ram();
}

void CubeMapTexture::free_image_ram() {
	for (unsigned char* data : image_data){
		stbi_image_free(data);
	}
}


void CubeMapTexture::bind() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r));
}

void CubeMapTexture::draw() {
	bind();
	cube.draw();
}

void CubeMapTexture::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}