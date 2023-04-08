#include "FrameBuffer.h"
#include "GL\glew.h"

#include <glm.hpp>

#include "Debuger.h"
#include "Graphic.h"
#include "Default_Assets.h"
#include "Frame.h"

#include <iostream>

FrameBuffer::FrameBuffer(int width, int height, int anti_alliasing, bool readable_depth_stencil) :
	width(width), height(height), multisample(anti_alliasing), color_texture(Texture(anti_alliasing)), depth_stencil_texture(Texture(anti_alliasing)), depth_stencil_renderbuffer(RenderBuffer(anti_alliasing)), readable_depth_stencil_buffer(readable_depth_stencil)
{	
	// setup color texture
	color_texture.internal_format = GL_RGB;
	color_texture.format = GL_RGB;
	color_texture.data_type = GL_UNSIGNED_BYTE;
	color_texture.min_filter = GL_NEAREST;
	color_texture.mag_filter = GL_NEAREST;
	color_texture.wrap_s = GL_CLAMP_TO_EDGE;
	color_texture.wrap_t = GL_CLAMP_TO_EDGE;
	color_texture.width = width;
	color_texture.height = height;
	

	if (readable_depth_stencil_buffer) {
		depth_stencil_texture.internal_format = GL_DEPTH24_STENCIL8;
		depth_stencil_texture.format = GL_DEPTH_STENCIL;
		depth_stencil_texture.data_type = GL_UNSIGNED_INT_24_8;
		depth_stencil_texture.min_filter = GL_NEAREST;
		depth_stencil_texture.mag_filter = GL_NEAREST;
		depth_stencil_texture.wrap_s = GL_CLAMP_TO_EDGE;
		depth_stencil_texture.wrap_t = GL_CLAMP_TO_EDGE;
		depth_stencil_texture.width = width;
		depth_stencil_texture.height = height;
		depth_stencil_texture.channels = 4; // 2?
	}
	else {
		depth_stencil_renderbuffer.internal_format = GL_DEPTH24_STENCIL8;
		depth_stencil_renderbuffer.format = GL_DEPTH_STENCIL;
		depth_stencil_renderbuffer.data_type = GL_UNSIGNED_INT_24_8;
		depth_stencil_renderbuffer.min_filter = GL_NEAREST;
		depth_stencil_renderbuffer.mag_filter = GL_NEAREST;
		depth_stencil_renderbuffer.wrap_s = GL_CLAMP_TO_EDGE;
		depth_stencil_renderbuffer.wrap_t = GL_CLAMP_TO_EDGE;
		depth_stencil_renderbuffer.width = width;
		depth_stencil_renderbuffer.height = height;
		depth_stencil_renderbuffer.channels = 4; // 2?
	}
}

void FrameBuffer::generate_framebuffer_object() {
	if (is_initialized())
		return;

	GLCall(glGenFramebuffers(1, &id));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
	color_texture.texture_slot = texture_slot;

	color_texture.initialize_blank_image(width, height);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture.target, color_texture.id, 0));

	if (readable_depth_stencil_buffer) {
		depth_stencil_texture.initialize_blank_image(width, height);
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_texture.target, depth_stencil_texture.id, 0));
	}
	else {
		depth_stencil_renderbuffer.initialize_blank_image(width, height);
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_renderbuffer.target, depth_stencil_renderbuffer.id));
	}

	unsigned int error_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error_status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[OPENGL ERROR] Framebuffer Error: " << error_status;
		ASSERT(false);
	}
	unbind();

	_is_initialized = true;
}

bool FrameBuffer::is_initialized() {
	return _is_initialized;
}

void FrameBuffer::bind() {
	generate_framebuffer_object();

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::render(unsigned int source_texture) {
	generate_framebuffer_object();

	if (multisample == 0){
		GLCall(glDisable(GL_DEPTH_TEST));
		if (!screen_initialized) {
			screen = default_geometry::rectangle(glm::vec2(2.0f));
			screen.load_program(program);
			screen_initialized = true;
		}

		if (source_texture == COLOR_TEXTURE) {
			color_texture.texture_slot = 9; // make it dynamic
			color_texture.bind();
		}
		if (source_texture == DEPTH_TEXTURE) {
			depth_stencil_texture.texture_slot = 9;
			depth_stencil_texture.bind();
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT));
		}
		if (source_texture == STENCIL_TEXTURE) {
			depth_stencil_texture.texture_slot = 9;
			depth_stencil_texture.bind();
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX));
		}
		program->update_uniform("texture_slot", 9);
		screen.draw(false);
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
	generate_framebuffer_object();

	int w = width;
	int h = height;
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
	unbind();

	return Image(i_pixels, w, h, channels, vertical_flip);
}


FrameBuffer::~FrameBuffer() {
	release();
}

void FrameBuffer::load_program(Program_s program) {
	this->program = program;
	_is_program_loaded = true;
}

void FrameBuffer::release() {
	color_texture.release();
	depth_stencil_renderbuffer.release();
	depth_stencil_texture.release();
	GLCall(glDeleteFramebuffers(1, &id));
	
	_is_initialized = false;
}

void FrameBuffer::blit_section(const FrameBuffer& target_buffer, const glm::vec4& source_rect, const glm::vec4& target_rect, unsigned int mask_bits, unsigned int interpolation){
		

}

void FrameBuffer::blit_section_to_screen(const glm::vec4& source_rect, const glm::vec4& target_rect, unsigned int mask_bits, unsigned int interpolation){
		

}
