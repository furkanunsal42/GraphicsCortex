#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

Texture::Texture() {
	GLCall(glGenTextures(1, &id));
}

void Texture::read_image(std::string file_path, int desired_channels) {
	if (file_path == current_loaded_file_name) {
		std::cout << "[Opengl Warning] Texture::read_image() is called but file_path was same with previously loaded file. Loading is cancelled. \n";
		return;
	}
	
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
	current_loaded_file_name = file_path;
	//std::cout << "Texture::read_image() is called \n";
}

// set file_path to "" to use already loaded image
void Texture::load_image(std::string file_path, int desired_channels, bool free_ram) {
	if (file_path != "")
		read_image(file_path, desired_channels);
	
	if (image_data == nullptr) {
		std::cout << "[Opengl Error] load image is called with blank file_path but no already loaded image is found\n";
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
	
	GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, image_data));
	GLCall(glGenerateMipmap(target));

	if (free_ram)
		free_image();
}

void Texture::queue_image(std::string file_path, int desired_channels, bool free_ram) {
	queued_image_path = file_path;
	queued_desired_channels = desired_channels;
	queued_free_ram = free_ram;
}

void Texture::read_queue() {
	if (queued_image_path == "") {
		std::cout << "[Opengl Error] Texture::read_queue() is ran but queue was empty, Read is cancelled. \n";
		return;
	}
	read_image(queued_image_path, queued_desired_channels);
}

void Texture::load_queue() {
	if (queued_image_path == "") {
		std::cout << "[Opengl Error] Texture::load_queue() ran but queue was empty, Load is cancelled. \n";
		return;
	}
	load_image("", queued_desired_channels, queued_free_ram);
}

void Texture::free_image() {
	stbi_image_free(image_data);
	image_data = nullptr;
	current_loaded_file_name = "";
}


void Texture::initialize_blank_image() {
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	if (target == GL_TEXTURE_2D){
		GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, NULL));
		GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
		GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
		GLCall(glGenerateMipmap(target));
	}
	else{
		GLCall(glTexImage2DMultisample(target, multisample_amount, internal_format, width, height, GL_TRUE));
	}
}


void Texture::bind() {
	// load queue if any image is queued
	if (queued_image_path != ""){
		load_queue();
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));
	
	//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, data_type, image_data));
	//GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::unbind() {
	GLCall(glBindTexture(target, 0));
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
	std::vector<std::thread> task;
	if (color_map != nullptr){
		color_map->texture_slot = color_map_slot;
		task.push_back(std::thread(&Texture::read_queue, color_map));
	}
	if (specular_map != nullptr){
		specular_map->texture_slot = specular_map_slot;
		task.push_back(std::thread(&Texture::read_queue, specular_map));
	}
	if (normal_map != nullptr){
		normal_map->texture_slot = normal_map_slot;
		task.push_back(std::thread(&Texture::read_queue, normal_map));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].join();
	}

	if (color_map != nullptr) {
		color_map->load_queue();
	}
	if (specular_map != nullptr) {
		specular_map->load_queue();
	}
	if (normal_map != nullptr) {
		normal_map->load_queue();
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