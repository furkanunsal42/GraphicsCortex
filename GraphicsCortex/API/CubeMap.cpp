
#include "CubeMap.h"

#include "stb_image.h"

#include "Debuger.h"

#include <iostream>
#include <thread>
#include <vector>

CubeMapTexture::CubeMapTexture() {
	GLCall(glGenTextures(1, &id));
}

void CubeMapTexture::_read_data_stbi(int n, int desired_channels) {
	image_data[n] = stbi_load(face_texture_filepaths[n].c_str(), &(width[n]), &(height[n]), &(channels[n]), desired_channels);
}

void CubeMapTexture::read_queue(int desired_channels) {
	for (std::string file_path : face_texture_filepaths) {
		if (file_path == "") {
			std::cout << "[Opengl Error] At least one of the texture file paths is not specified for CubeMapTexture.load_data" << std::endl;
			return;
		}
	}
	
	std::vector<std::thread> tasks;
	for (int i = 0; i < 6; i++) {
		if (image_data[i] != nullptr)
			stbi_image_free(image_data[i]);

		stbi_set_flip_vertically_on_load(vertical_flip[i]);
		
		tasks.push_back(std::thread(&CubeMapTexture::_read_data_stbi, this, i, desired_channels));
	}
	for (int i = 0; i < 6; i++) {
		tasks[i].join();
	}
	for (int i = 0; i < 6; i++) {
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
}

void CubeMapTexture::load_queue(bool free_ram) {
	for (int i = 0; i < 6; i++) {
		if (image_data[i] == nullptr) {
			std::cout << "[Opengl Error] CubeMapTexture::load_queue() is ran but at data wasn't read previously. Cancelling Operation. \n";
			return;
		}
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

void CubeMapTexture::read_and_load_queue(int desired_channels, bool free_ram) {
	read_queue(desired_channels);
	load_queue(free_ram);
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
	camera->update_matrixes();
	camera->update_default_uniforms(*cube.renderer);
	cube.renderer->bind();

	cube.update_uniforms();
	cube.update_default_uniforms(*cube.renderer);
	
	bool face_culling = glIsEnabled(GL_CULL_FACE);
	GLCall(glDisable(GL_CULL_FACE));
	int depth_function;
	glGetIntegerv(GL_DEPTH_FUNC, &depth_function);
	GLCall(glDepthFunc(GL_LEQUAL));
	
	cube.draw(false);
	
	if (face_culling){
		GLCall(glEnable(GL_CULL_FACE));
	}
	GLCall(glDepthFunc(depth_function));

}

void CubeMapTexture::unbind() {
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMapTexture::set_program(Program_s renderer) {
	cube.load_program(renderer);
}
void CubeMapTexture::set_update_queue(uniform_update_queue& update_queue) {
	cube.set_uniform_all(update_queue);
}

void CubeMapTexture::set_update_queue(uniform_update_queue&& update_queue) {
	cube.set_uniform_all(update_queue);
}

void CubeMapTexture::update_default_uniforms(Program& program) {
	program.update_uniform("cubemap", texture_slot);
}