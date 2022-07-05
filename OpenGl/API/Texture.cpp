#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"

#include <future>
#include <iostream>

Texture::Texture() {
	GLCall(glGenTextures(1, &id));
}

void Texture::read_image(std::string file_path, int desired_channels) {
	if (image_data != nullptr)
		stbi_image_free(image_data);

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
}

void Texture::load_image(std::string file_path, int desired_channels, bool free_ram) {
	if (image_data == nullptr)
		read_image(file_path, desired_channels);

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, image_data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	if (free_ram)
		free_image();
}

void Texture::queue_image(std::string file_path, int desired_channels, bool free_ram) {
	queued_image_path = file_path;
	queued_desired_channels = desired_channels;
	queued_free_ram = free_ram;
}

void Texture::read_queue() {
	if (queued_image_path != ""){
		read_image(queued_image_path, queued_desired_channels);
	}
}

void Texture::finalize_queue_and_bind() {
	//read_image(queued_image_path, queued_desired_channels);
	
	// if queue was empty, just bind
	if (queued_image_path == "") {
		bind();
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, image_data));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	if (queued_free_ram)
		free_image();

	queued_image_path = "";
	queued_desired_channels = NULL;
	queued_free_ram = NULL;
}

void Texture::free_image() {
	stbi_image_free(image_data);
}


void Texture::initialize_blank_image() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
}


void Texture::bind() {
	// load queue if any image is queued
	if (queued_image_path != ""){
		read_image(queued_image_path, queued_desired_channels);
		finalize_queue_and_bind();
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
	
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, image_data));
	//GLCall(glGenerateMipmap(GL_TEXTURE_2D));
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

void temp() {

}

void Material::bind() {
	std::vector<std::future<void>> task;
	if (color_map != nullptr){
		color_map->texture_slot = color_map_slot;
		task.push_back(std::async(std::launch::async, &Texture::read_queue, color_map));
	}
	if (specular_map != nullptr){
		specular_map->texture_slot = specular_map_slot;
		task.push_back(std::async(std::launch::async, &Texture::read_queue, specular_map));
	}
	if (normal_map != nullptr){
		normal_map->texture_slot = normal_map_slot;
		task.push_back(std::async(std::launch::async, &Texture::read_queue, normal_map));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].get();
	}

	if (color_map != nullptr) {
		color_map->finalize_queue_and_bind();
	}
	if (specular_map != nullptr) {
		specular_map->finalize_queue_and_bind();
	}
	if (normal_map != nullptr) {
		normal_map->finalize_queue_and_bind();
	}
}

void Material::unbind() {
	if (color_map != nullptr)
		color_map->unbind();
	if (specular_map != nullptr)
		specular_map->unbind();
	if (normal_map != nullptr)
	normal_map->unbind();
}