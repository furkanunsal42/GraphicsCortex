#include "Framebuffer2.h"
#include "Debuger.h"
#include <iostream>

void Framebuffer2::bind_screen_read_draw()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer2::bind_screen_read()
{
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
}

void Framebuffer2::bind_screen_draw()
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

unsigned int Framebuffer2::Channel_to_OpenGL(Channel channel)
{
	switch (channel) {
	case Framebuffer2::Channel::COLOR:					return GL_COLOR_BUFFER_BIT;
	case Framebuffer2::Channel::DEPTH:					return GL_DEPTH_BUFFER_BIT;
	case Framebuffer2::Channel::STENCIL:				return GL_STENCIL_BUFFER_BIT;
	case Framebuffer2::Channel::COLOR_DEPTH:			return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	case Framebuffer2::Channel::COLOR_STENCIL:			return GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	case Framebuffer2::Channel::COLOR_DEPTH_STENCIL:	return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	}
	return 0;
}

unsigned int Framebuffer2::Filter_to_OpenGL(Filter filter)
{
	switch (filter) {
	case Framebuffer2::Filter::NEAREST: return GL_NEAREST;
	case Framebuffer2::Filter::LINEAR: return GL_LINEAR;
	}
	return 0;
}

Framebuffer2::Framebuffer2()
{
	GLCall(glCreateFramebuffers(1, &id));
	_framebuffer_generated = true;
}

Framebuffer2::~Framebuffer2()
{
	release();
}

void Framebuffer2::release()
{
	if (!_framebuffer_generated) return;

	GLCall(glDeleteFramebuffers(1, &id));

	_framebuffer_generated = false;
}

void Framebuffer2::bind_read_draw()
{
	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_read_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void Framebuffer2::bind_read()
{
	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_read() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, id));
}

void Framebuffer2::bind_draw()
{
	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id));
}


void Framebuffer2::attach_color(int slot, std::shared_ptr<Texture2D> texture2d, int mipmap_level)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture2d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach() but texture was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texture2d;
	texture2d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + slot, texture2d->id, mipmap_level));
	_draw_buffers_are_updated = false;
}

void Framebuffer2::attach_depth(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
{
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture2d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth() but texture was released" << std::endl;
		ASSERT(false);
	}

	_depth_attachment = texture2d;
	texture2d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_DEPTH_ATTACHMENT, texture2d->id, mipmap_level));
}

void Framebuffer2::attach_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
{
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_stencil() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture2d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_stencil() but texture was released" << std::endl;
		ASSERT(false);
	}

	_stencil_attachment = texture2d;
	texture2d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_STENCIL_ATTACHMENT, texture2d->id, mipmap_level));
}

void Framebuffer2::attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
{
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth_stencil() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture2d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth_stencil() but texture was released" << std::endl;
		ASSERT(false);
	}

	_stencil_attachment = texture2d;
	texture2d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_DEPTH_STENCIL_ATTACHMENT, texture2d->id, mipmap_level));
}

void Framebuffer2::set_read_buffer(int slot)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to set_read_buffer() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (_color_attachments[slot] == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to set_read_buffer() but slot was nullptr" << std::endl;
		ASSERT(false);
	}

	_active_read_buffer = slot;
	GLCall(glNamedFramebufferReadBuffer(id, GL_COLOR_ATTACHMENT0 + slot););
	_active_read_buffer_ever_set = true;
}

void Framebuffer2::activate_draw_buffer(int slot)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to activate_draw_buffer() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (_color_attachments[slot] == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to activate_draw_buffer() but slot was nullptr" << std::endl;
		ASSERT(false);
	}

	_active_draw_buffers.insert(slot);
	_draw_buffers_are_updated = false;
}

void Framebuffer2::deactivate_draw_buffer(int slot)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to deactivate_draw_buffer() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (_color_attachments[slot] == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to deactivate_draw_buffer() but slot was nullptr" << std::endl;
		ASSERT(false);
	}

	_active_draw_buffers.erase(slot);
	_draw_buffers_are_updated = false;
}

void Framebuffer2::update_activated_draw_buffers()
{
	if (_draw_buffers_are_updated) return;

	int count = _active_draw_buffers.size();
	int* buffer_enums = new int[count];
	int i = 0;
	for (auto iterator = _active_draw_buffers.begin(); iterator != _active_draw_buffers.end(); iterator++) {
		int slot = *iterator;
		ASSERT(i < count);
		buffer_enums[i] = GL_COLOR_ATTACHMENT0 + slot;
		i++;
	}
	GLCall(glNamedFramebufferDrawBuffers(id, count, (unsigned int*)buffer_enums));
	_draw_buffers_are_updated = true;
}

void Framebuffer2::clear_draw_buffer()
{
	_active_draw_buffers.clear();
	_draw_buffers_are_updated = false;
}

void Framebuffer2::blit(Framebuffer2& target, int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter)
{
	update_activated_draw_buffers();
	target.update_activated_draw_buffers();

	GLCall(glBlitNamedFramebuffer(id, target.id, self_x0, self_y0, self_x1, self_y1, target_x0, target_y0, target_x1, target_y1, Channel_to_OpenGL(channel), Filter_to_OpenGL(filter)));
}

void Framebuffer2::blit_to_screen(int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter)
{
	update_activated_draw_buffers();
	GLCall(glBlitNamedFramebuffer(id, 0, self_x0, self_y0, self_x1, self_y1, target_x0, target_y0, target_x1, target_y1, Channel_to_OpenGL(channel), Filter_to_OpenGL(filter)));
}

