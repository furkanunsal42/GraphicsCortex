#include "FrameBuffer.h"
#include "GL\glew.h"

#include <glm.hpp>

#include "Debuger.h"
#include "Graphic.h"
#include "Default_Assets.h"
#include "Frame.h"

#include <iostream>


RenderBuffer::RenderBuffer()
{
	GLCall(glGenRenderbuffers(1, &id));
}

void RenderBuffer::bind() {
	GLCall(glBindRenderbuffer(target, id));
	if (multisample == 0){
		GLCall(glRenderbufferStorage(target, internal_format, width, height));
	}
	else {
		GLCall(glRenderbufferStorageMultisample(target, multisample, internal_format, width, height));
	}
}

void RenderBuffer::unbind() {
	GLCall(glBindRenderbuffer(target, 0));
}

FrameBuffer::FrameBuffer(int width, int height, int anti_alliasing, bool readable_depth_stencil_buffer) :
	width(width), height(height), multisample(anti_alliasing), depth_stencil_buffer(!readable_depth_stencil_buffer, multisample)
{	
	GLCall(glGenFramebuffers(1, &id));
	color_texture.internal_format = GL_RGB;
	color_texture.format = GL_RGB;
	color_texture.data_type = GL_UNSIGNED_BYTE;
	color_texture.min_filter = GL_NEAREST;
	color_texture.mag_filter = GL_NEAREST;
	color_texture.wrap_s = GL_CLAMP_TO_EDGE;
	color_texture.wrap_t = GL_CLAMP_TO_EDGE;

	color_texture.width = width;
	color_texture.height = height;

	color_texture.multisample_amount = multisample;

	if (multisample == 0)
		color_texture.target = GL_TEXTURE_2D;
	else
		color_texture.target = GL_TEXTURE_2D_MULTISAMPLE;
	

	depth_stencil_buffer.internal_format = GL_DEPTH24_STENCIL8;
	depth_stencil_buffer.format = GL_DEPTH_STENCIL;
	depth_stencil_buffer.data_type = GL_UNSIGNED_INT_24_8;
	depth_stencil_buffer.min_filter = GL_NEAREST;
	depth_stencil_buffer.mag_filter = GL_NEAREST;
	depth_stencil_buffer.wrap_s = GL_CLAMP_TO_EDGE;
	depth_stencil_buffer.wrap_t = GL_CLAMP_TO_EDGE;

	depth_stencil_buffer.width = width;
	depth_stencil_buffer.height = height;

	depth_stencil_buffer.multisample_amount = multisample;

	if (depth_stencil_buffer.use_renderbuffer)
		depth_stencil_buffer.target = GL_RENDERBUFFER;
	else
		depth_stencil_buffer.target = GL_TEXTURE_2D;

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
	color_texture.texture_slot = texture_slot;
	
	color_texture.initialize_blank_image(width, height);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture.target, color_texture.id, 0));

	if (depth_stencil_buffer.use_renderbuffer) {
		depth_stencil_buffer.bind();
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_buffer.target, depth_stencil_buffer.id));
	}
	else {
		depth_stencil_buffer.initialize_blank_image(width, height);
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_buffer.target, depth_stencil_buffer.id, 0));
	}

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

void FrameBuffer::render(unsigned int source_texture) {
	if (multisample == 0){
		GLCall(glDisable(GL_DEPTH_TEST));
		if (!screen_initialized) {
			Material* material = new Material();
			material->color_map = &color_texture;
			material->color_map_slot = 5;
			screen = default_geometry::rectangle(*material, *program, glm::vec2(2.0f));
			screen_initialized = true;
		}

		Texture::CurrentBindedTexture[9] = color_texture.id;
		GLCall(glActiveTexture(GL_TEXTURE0 + 9));
		if (source_texture == COLOR_TEXTURE) {
			GLCall(glBindTexture(color_texture.target, color_texture.id));
		}
		if (source_texture == DEPTH_TEXTURE) {
			glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
			GLCall(glBindTexture(depth_stencil_buffer.target, depth_stencil_buffer.id));
		}
		if (source_texture == STENCIL_TEXTURE) {
			glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
			GLCall(glBindTexture(depth_stencil_buffer.target, depth_stencil_buffer.id));
		}

		program->update_uniform("texture_slot", 9);
		screen.draw();
		GLCall(glEnable(GL_DEPTH_TEST));
	}
	else {
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, id));
		GLCall(glDrawBuffer(GL_BACK));
		GLCall(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT , GL_LINEAR));
	}
}

Image FrameBuffer::save(bool vertical_flip) {

	int w = frame::window_width;
	int h = frame::window_height;
	int image_internal_format = color_texture.internal_format;
	int channels;

	if (image_internal_format == GL_RGB)
		channels = 3;
	else if (image_internal_format == GL_RGBA)
		channels = 4;

	int image_size = w * h * channels * sizeof(unsigned char);

	bind();
	unsigned char* i_pixels = new unsigned char[image_size / sizeof(unsigned char)];
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, id));
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
	GLCall(glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, i_pixels));
	GLCall(glReadPixels(0, 0, w, h, color_texture.format, color_texture.data_type, i_pixels));

	return Image(i_pixels, w, h, channels, vertical_flip);
}