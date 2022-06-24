#include "FrameBuffer.h"
#include "GL\glew.h"

#include <glm.hpp>

#include "Debuger.h"
#include "Graphic.h"
#include "Default_Assets.h"

#include <iostream>


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
	GLCall(glGenFramebuffers(1, &id));
	color_texture.internal_format = GL_RGB;
	color_texture.format = GL_RGB;
	color_texture.data_type = GL_UNSIGNED_BYTE;
	color_texture.width = width;
	color_texture.height = height;
	color_texture.min_filter = GL_NEAREST;
	color_texture.mag_filter = GL_NEAREST;
	color_texture.wrap_s = GL_CLAMP_TO_EDGE;
	color_texture.wrap_t = GL_CLAMP_TO_EDGE;

	depth_stencil_buffer.width = width;
	depth_stencil_buffer.height = height;
	depth_stencil_buffer.internal_format = GL_DEPTH24_STENCIL8;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
	color_texture.texture_slot = texture_slot;
	color_texture.initialize_blank_image();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture.id, 0));
	depth_stencil_buffer.bind();
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer.id));

	unsigned int error_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error_status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OPENGL ERROR] Framebuffer Error: " << error_status;
		ASSERT(false);
	}
	unbind();
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//color_texture.unbind();
	//depth_stencil_buffer.unbind();
}

void FrameBuffer::render() {
	glDisable(GL_DEPTH_TEST);
	if (!screen_initialized) {
		Material material;
		material.color_map = &color_texture;
		screen = default_geometry::rectangle(material, *program, glm::vec2(2.0f));
		screen_initialized = true;
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, color_texture.id));
	program->update_uniform("texture_slot", 9);
	screen.draw();
	glEnable(GL_DEPTH_TEST);
}