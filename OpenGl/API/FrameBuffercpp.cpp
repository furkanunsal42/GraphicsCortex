#include "FrameBuffer.h"
#include "GL\glew.h"

#include <glm.hpp>

#include "Debuger.h"
#include "Graphic.h"
#include "Default_Geometries.h"

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
	if (error_status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OPENGL ERROR] Framebuffer Error: " << error_status;
		ASSERT(false);
	}
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	//color_texture.unbind();
	//depth_stencil_buffer.unbind();
}

void FrameBuffer::render() {
	GLCall(glActiveTexture(GL_TEXTURE0 + 4));
	GLCall(glBindTexture(GL_TEXTURE_2D, color_texture.id));
	program->update_uniform("texture_slot", 4);
	GLCall(glDisable(GL_DEPTH_TEST));

	glBindVertexArray(screen_vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	GLCall(glEnable(GL_DEPTH_TEST));
}