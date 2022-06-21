#include "Texture.h"
#include "GL\glew.h"

#include "stb_image.h"
#include <glm.hpp>

#include "Debuger.h"
#include "Graphic.h"
#include "Default_Geometries.h"

#include <iostream>

Texture::Texture() {
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
	GLCall(glGenTextures(1, &id));
}

void Texture::free_image() {
	stbi_image_free(image_data);
}

void Texture::initialize_blank_image() {
	GLCall(glGenTextures(1, &id));
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

RenderBuffer::RenderBuffer()
{
	GLCall(glGenRenderbuffers(1, &id));
}

void RenderBuffer::bind() {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, id));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height));
}

void RenderBuffer::unbind() {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

FrameBuffer::FrameBuffer(int width, int height) :
	width(width), height(height)
{
	// create color_texture
	GLCall(glGenFramebuffers(1, &id));
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
	color_texture.internal_format = GL_RGB;
	color_texture.format = GL_RGB;
	color_texture.data_type = GL_UNSIGNED_BYTE;
	color_texture.width = width;
	color_texture.height = height;
	color_texture.min_filter = GL_NEAREST;
	color_texture.mag_filter = GL_NEAREST;
	color_texture.wrap_s = GL_CLAMP_TO_EDGE;
	color_texture.wrap_t = GL_CLAMP_TO_EDGE;
	//color_texture.initialize_blank_image();
	//GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0));

	// create depth and stencil buffers
	depth_stencil_buffer.width = width;
	depth_stencil_buffer.height = height;
	depth_stencil_buffer.internal_format = GL_DEPTH24_STENCIL8;
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
	color_texture.initialize_blank_image();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture.id, 0));

	depth_stencil_buffer.bind();
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer.id));

	unsigned int error_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error_status != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "[OPENGL ERROR] Framebuffer Error: " << error_status;
		ASSERT(false);
	}
	else {
		std::cout << "[OPENGL] Framebuffer Initialized" << std::endl;
	}
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//color_texture.unbind();
	//depth_stencil_buffer.unbind();
}

void FrameBuffer::render() {
	Material material;
	GLCall(glActiveTexture(GL_TEXTURE0 + 4));
	GLCall(glBindTexture(GL_TEXTURE_2D, color_texture.id));
	Shader shader("Shaders/FrameBufferVertex.glsl", "Shaders/FrameBufferFragment.glsl");
	Program program(shader.vertex_shader, shader.fragment_shader);
	program.update_uniform("texture_slot", 4);
	Graphic screen = default_geometry::rectangle(material, program, glm::vec2(2.0f));
	GLCall(glDisable(GL_DEPTH_TEST));
	screen.draw();
	GLCall(glEnable(GL_DEPTH_TEST));
}