#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"

#include "Debuger.h"

#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

unsigned int Texture::CurrentBindedTexture[MAX_TEXTURE_SLOTS];

Texture::Texture(bool renderbuffer, int renderbuffer_multisample) : 
	use_renderbuffer(renderbuffer), multisample_amount(renderbuffer_multisample)
{
	if (renderbuffer){
		GLCall(glGenRenderbuffers(1, &id));
	}
	else {
		GLCall(glGenTextures(1, &id));
	}
}

bool Texture::_read_image_check(std::string& file_path, int desired_channels, bool print_errors) {
	if (use_renderbuffer) {
		std::cout << "Texture::_read_image_check() is called while use_renderbuffer == true, operation cancelled.\n";
		return false;
	}

	if (file_path == currently_stored_file_name) {
		if (print_errors)
			std::cout << "[Opengl Warning] Texture::_read_image() is called but file_path was same with previously loaded file. Loading is cancelled. \n";
		return false;
	}

	if (CurrentBindedTexture[texture_slot] == id) {
		if (print_errors)
			std::cout << "[Opengl Warning] Texture::_read_image() is called but last binded texture is identity. Loading is cancelled. \n";
		return false;
	}

	return true;
}

void Texture::_read_image(std::string& file_path, int desired_channels) {
	if (use_renderbuffer) {
		std::cout << "Texture::_read_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	if (!_read_image_check(file_path, desired_channels))
		return;
	
	if (image_data != nullptr)
		stbi_image_free(image_data);

	stbi_set_flip_vertically_on_load(vertical_flip);

	image_data = stbi_load(file_path.c_str(), &width, &height, &channels, desired_channels);
	if (desired_channels == 3) {
		if (format == NULL)
			format = GL_RGB;
		if (internal_format == NULL)
		{
			if (compress_image)
				internal_format = GL_COMPRESSED_RGB;
			else
				internal_format = GL_RGB8;
		}
	}
	else if (desired_channels == 4) {
		if (format == NULL)
			format = GL_RGBA;
		if (internal_format == NULL)
		{
			if (compress_image)
				internal_format = GL_COMPRESSED_RGBA;
			else
				internal_format = GL_RGBA8;
		}
	}

	currently_stored_file_name = file_path;
	//std::cout << "Texture::read_image() is called \n";
}

bool Texture::_load_image_check(bool free_ram, bool print_errors) {
	if (use_renderbuffer) {
		std::cout << "Texture::_load_image_check() is called while use_renderbuffer == true, operation cancelled.\n";
		return false;
	}

	if (image_data == nullptr && CurrentBindedTexture[texture_slot] != id) {
		if (print_errors)
			std::cout << "[Opengl Error] Texture::_load_image() is called but no already loaded image is found\n";
		return true; // was false
	}

	if (CurrentBindedTexture[texture_slot] == id) {
		if (print_errors)
			std::cout << "[Opengl Warning] Texture::_load_iamge() is called but last binded texture is identity. Loading is cancelled. \n";
		return false;
	}
	return true;
}

void Texture::_load_image(bool free_ram) {
	if (use_renderbuffer) {
		std::cout << "Texture::_load_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	if (!_load_image_check(free_ram))
		return;

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));

	//if (compress_image)
		//glCompressedTexImage2D(target, 0, internal_format, width, height, 0, );
	//else 
	GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, image_data));
	
	if (generate_mipmap)
		GLCall(glGenerateMipmap(target));

	if (free_ram)
		free_image();

	// save it's id on current binded texture list, for optimization
	CurrentBindedTexture[texture_slot] = id;
}

void Texture::load_image(std::string file_path, int desired_channels, bool free_ram) {
	if (use_renderbuffer) {
		std::cout << "Texture::load_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	_read_image(file_path, desired_channels);
	_load_image(free_ram);
}

void Texture::queue_image(std::string file_path, int desired_channels, bool free_ram) {
	if (use_renderbuffer) {
		std::cout << "Texture::queue_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	queued_image_path = file_path;
	queued_desired_channels = desired_channels;
	queued_free_ram = free_ram;
}

void Texture::read_queue() {
	if (use_renderbuffer) {
		std::cout << "Texture::read_queue() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	if (queued_image_path == "") {
		std::cout << "[Opengl Error] Texture::read_queue() is ran but queue was empty, Read is cancelled. \n";
		return;
	}
	_read_image(queued_image_path, queued_desired_channels);
}

void Texture::load_queue() {
	if (use_renderbuffer) {
		std::cout << "Texture::load_queue() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	if (queued_image_path == "") {
		if (image_data != nullptr)
			bind();
		std::cout << "[Opengl Error] Texture::load_queue() ran but queue was empty, Load is cancelled. \n";
		return;
	}
	_load_image(queued_free_ram);
}

void Texture::free_image() {
	if (use_renderbuffer) {
		std::cout << "Texture::free_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

	stbi_image_free(image_data);
	image_data = nullptr;
	currently_stored_file_name = "";
}


void Texture::initialize_blank_image() {
	if (use_renderbuffer) {
		std::cout << "Texture::initialize_blank_image() is called while use_renderbuffer == true, operation cancelled.\n";
		return;
	}

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
	// save it's id on current binded texture list, for optimization
	CurrentBindedTexture[texture_slot] = id;
}


void Texture::bind() {
	if (use_renderbuffer) {
		GLCall(glBindRenderbuffer(target, id));
		if (multisample_amount == 0) {
			GLCall(glRenderbufferStorage(target, internal_format, width, height));
		}
		else {
			GLCall(glRenderbufferStorageMultisample(target, multisample_amount, internal_format, width, height));
		}
		return;
	}

	#ifdef TEXTURE_REPEATED_BIND_OPTIMIZATION
	if (CurrentBindedTexture[texture_slot] == id)
		return;
	#endif
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));

	// save it's id on current binded texture list, for optimization
	CurrentBindedTexture[texture_slot] = id;
}

void Texture::unbind() {
	if (use_renderbuffer) {
		GLCall(glBindRenderbuffer(target, 0));
		return;
	}

	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, 0));
	CurrentBindedTexture[texture_slot] = 0;
}

void Texture::save() {
	glBindTexture(target, id);
	//bind();

	int w, h;
	int image_size = width * height * channels * sizeof(unsigned char);
	int image_internal_format = internal_format;
	int image_compressed = false;

	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_WIDTH, &w));
	GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_HEIGHT, &h));

	if (compress_image){
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &image_size));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_COMPRESSED, &image_compressed));
		GLCall(glGetTexLevelParameteriv(target, 0, GL_TEXTURE_INTERNAL_FORMAT, &image_internal_format));
	}
	
	unsigned char* i_pixels = new unsigned char [image_size / sizeof(unsigned char)];
	
	if (compress_image == true){
		GLCall(glGetCompressedTexImage(target, 0, i_pixels));
	}
	else if (compress_image == false) {
		//GLCall(glGetTexImage(target, 0, format, data_type, i_pixels));
	}
	glDeleteTextures(1, &id);
	
	glGenTextures(1, &id);
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_slot));
	GLCall(glBindTexture(target, id));
	GLCall(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag_filter));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s));
	GLCall(glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t));

	if (compress_image){
		GLCall(glCompressedTexImage2D(target, 0, image_internal_format, width, height, 0, image_size, i_pixels));
	}
	else {
		GLCall(glTexImage2D(target, 0, internal_format, width, height, 0, format, data_type, image_data));
	}
	delete[] i_pixels;

	// save it's id on current binded texture list, for optimization
	CurrentBindedTexture[texture_slot] = id;
}

void Texture::print_info(unsigned int opengl_code) {
	bind();
	int data;
	GLCall(glGetTexLevelParameteriv(target, 0, opengl_code, &data));
	std::cout << "[INFO] Texture::print_info returns " << data << std::endl;
}

Material::Material() {}

Material::Material(Texture& color, Texture& specular, Texture& normal) :
	color_map(&color), specular_map(&specular), normal_map(&normal) {}

void temp() {

}

void Material::bind() {
	std::vector<std::thread> task;
	if (color_map != nullptr) {
		color_map->texture_slot = color_map_slot;
		if (color_map->queued_image_path != "" && color_map->_read_image_check(color_map->queued_image_path, color_map->queued_desired_channels, false))
			task.push_back(std::thread(&Texture::read_queue, color_map));
	}
	if (specular_map != nullptr) {
		specular_map->texture_slot = specular_map_slot;
		if (specular_map->queued_image_path != "" && specular_map->_read_image_check(specular_map->queued_image_path, specular_map->queued_desired_channels, false))
			task.push_back(std::thread(&Texture::read_queue, specular_map));
	}
	if (normal_map != nullptr) {
		normal_map->texture_slot = normal_map_slot;
		if (normal_map->queued_image_path != "" && normal_map->_read_image_check(normal_map->queued_image_path, normal_map->queued_desired_channels, false))
			task.push_back(std::thread(&Texture::read_queue, normal_map));
	}

	for (int i = 0; i < task.size(); i++) {
		task[i].join();
	}

	if (color_map != nullptr) {
		if (color_map->queued_image_path != "" && color_map->_load_image_check(color_map->queued_free_ram, false))
			color_map->load_queue();
		else
			color_map->bind();
	}
	if (specular_map != nullptr) {
		if (specular_map->queued_image_path != "" && specular_map->_load_image_check(specular_map->queued_free_ram, false))
			specular_map->load_queue();
		else
			specular_map->bind();
	}
	if (normal_map != nullptr) {
		if (normal_map->queued_image_path != "" && normal_map->_load_image_check(normal_map->queued_free_ram, false))
			normal_map->load_queue();
		else
			normal_map->bind();
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