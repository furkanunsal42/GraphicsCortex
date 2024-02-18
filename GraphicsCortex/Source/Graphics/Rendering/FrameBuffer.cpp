#include "Framebuffer.h"
#include "Debuger.h"
#include <iostream>

void Framebuffer::bind_screen_read_draw()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::bind_screen_read()
{
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
}

void Framebuffer::bind_screen_draw()
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

unsigned int Framebuffer::Channel_to_OpenGL(Channel channel)
{
	switch (channel) {
	case Framebuffer::Channel::COLOR:					return GL_COLOR_BUFFER_BIT;
	case Framebuffer::Channel::DEPTH:					return GL_DEPTH_BUFFER_BIT;
	case Framebuffer::Channel::STENCIL:				return GL_STENCIL_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_DEPTH:			return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_STENCIL:			return GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_DEPTH_STENCIL:	return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	}
	return 0;
}

unsigned int Framebuffer::Filter_to_OpenGL(Filter filter)
{
	switch (filter) {
	case Framebuffer::Filter::NEAREST: return GL_NEAREST;
	case Framebuffer::Filter::LINEAR: return GL_LINEAR;
	}
	return 0;
}

Framebuffer::Framebuffer()
{
	GLCall(glCreateFramebuffers(1, &id));
	_framebuffer_generated = true;
}

Framebuffer::~Framebuffer()
{
	release();
}

void Framebuffer::release()
{
	if (!_framebuffer_generated) return;

	GLCall(glDeleteFramebuffers(1, &id));

	_framebuffer_generated = false;
}

void Framebuffer::bind_read_draw()
{
	update_activated_draw_buffers();

	_check_framebuffer_status(GL_FRAMEBUFFER);

	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_read_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id));
}

void Framebuffer::bind_read()
{
	_check_framebuffer_status(GL_READ_FRAMEBUFFER);

	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_read() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, id));
}

void Framebuffer::bind_draw()
{
	update_activated_draw_buffers();

	_check_framebuffer_status(GL_DRAW_FRAMEBUFFER);

	if (!_framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to bind_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id));
}

void Framebuffer::attach_color(int slot, std::shared_ptr<Texture1D> texture1d, int mipmap_level)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texture1d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture1d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but texture was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texture1d;
	texture1d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + slot, texture1d->id, mipmap_level));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_color(int slot, std::shared_ptr<Texture2D> texture2d, int mipmap_level)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but texture was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture2d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but texture was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texture2d;
	texture2d->_allocate_texture();
	GLCall(glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + slot, texture2d->id, mipmap_level));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_color(int slot, std::shared_ptr<Texture2DArray> texture_array, int z, int mipmap) {
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but TextureArray was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture_array->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but TextureArray was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texture_array;
	texture_array->_allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + slot, texture_array->id, mipmap, z));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_color(int slot, std::shared_ptr<TextureCubeMap> texturecubemap, TextureCubeMap::Face face, int mipmap) {
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texturecubemap == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texturecubemap->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texturecubemap;
	texturecubemap->_allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + slot, texturecubemap->id, mipmap, TextureCubeMap::get_gl_face_index(face)));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_color(int slot, std::shared_ptr<Texture3D> texture3d, int z, int mipmap)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!texture3d->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but Texture3D was released" << std::endl;
		ASSERT(false);
	}
	if (z >= texture3d->depth) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but z was greater than depth of Texture3D" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = texture3d;
	texture3d->_allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id, GL_COLOR_ATTACHMENT0 + slot, texture3d->id, mipmap, z));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_color(int slot, std::shared_ptr<Renderbuffer> render_buffer)
{
	if (slot < 0 || slot >= _color_attachments.size()) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << _color_attachments.size() << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but render_buffer was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_color() but render_buffer was released" << std::endl;
		ASSERT(false);
	}

	_color_attachments[slot] = render_buffer;
	render_buffer->_allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id, GL_COLOR_ATTACHMENT0 + slot, render_buffer->target, render_buffer->id));
	_draw_buffers_are_updated = false;
}

void Framebuffer::attach_depth(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
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

void Framebuffer::attach_depth(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth() but render_buffer was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth() but render_buffer was released" << std::endl;
		ASSERT(false);
	}

	_depth_attachment = render_buffer;
	render_buffer->_allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id, GL_DEPTH_ATTACHMENT, render_buffer->target, render_buffer->id));
}

void Framebuffer::attach_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
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

void Framebuffer::attach_stencil(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_stencil() but render_buffer was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_stencil() but render_buffer was released" << std::endl;
		ASSERT(false);
	}

	_depth_attachment = render_buffer;
	render_buffer->_allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id, GL_STENCIL_ATTACHMENT, render_buffer->target, render_buffer->id));
}

void Framebuffer::attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level)
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

void Framebuffer::attach_depth_stencil(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth_stencil() but render_buffer was nullptr" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer->_texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer treid to attach_depth_stencil() but render_buffer was released" << std::endl;
		ASSERT(false);
	}

	_depth_attachment = render_buffer;
	render_buffer->_allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, render_buffer->target, render_buffer->id));
}

void Framebuffer::set_read_buffer(int slot)
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
	GLCall(glNamedFramebufferReadBuffer(id, GL_COLOR_ATTACHMENT0 + slot));
	_active_read_buffer_ever_set = true;
}

void Framebuffer::activate_draw_buffer(int slot)
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

void Framebuffer::deactivate_draw_buffer(int slot)
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

void Framebuffer::update_activated_draw_buffers()
{
	if (_draw_buffers_are_updated) return;

	int count = _active_draw_buffers.size();
	unsigned int* buffer_enums = new unsigned int[count];
	int i = 0;
	for (auto iterator = _active_draw_buffers.begin(); iterator != _active_draw_buffers.end(); iterator++) {
		int slot = *iterator;
		ASSERT(i < count);
		buffer_enums[i] = GL_COLOR_ATTACHMENT0 + (unsigned int)slot;
		i++;
	}
	GLCall(glNamedFramebufferDrawBuffers(id, count, buffer_enums));
	_draw_buffers_are_updated = true;
}

void Framebuffer::deactivate_all_draw_buffers()
{
	_active_draw_buffers.clear();
	_draw_buffers_are_updated = false;
}

void Framebuffer::blit(Framebuffer& target, int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter)
{
	update_activated_draw_buffers();
	target.update_activated_draw_buffers();

	GLCall(glBlitNamedFramebuffer(id, target.id, self_x0, self_y0, self_x1, self_y1, target_x0, target_y0, target_x1, target_y1, Channel_to_OpenGL(channel), Filter_to_OpenGL(filter)));
}

void Framebuffer::blit_to_screen(int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter)
{
	update_activated_draw_buffers();
	GLCall(glBlitNamedFramebuffer(id, 0, self_x0, self_y0, self_x1, self_y1, target_x0, target_y0, target_x1, target_y1, Channel_to_OpenGL(channel), Filter_to_OpenGL(filter)));
}

void Framebuffer::_check_framebuffer_status(unsigned int gl_bind_target)
{
	ASSERT(_framebuffer_generated);

	GLCall(unsigned int error_status = glCheckFramebufferStatus(gl_bind_target));
	switch (error_status) {
	case GL_FRAMEBUFFER_COMPLETE: return;
	case GL_FRAMEBUFFER_UNDEFINED:						{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_UNDEFINED" << std::endl;						return;}
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:			{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;			return;}
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:	{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;	return;}
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:			{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;			return;}
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:			{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;			return;}
	case GL_FRAMEBUFFER_UNSUPPORTED:					{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;						return;}
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:			{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;			return;}
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:		{std::cout << "[OpenGL Error] Framebuffer failed to create : GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;			return;}
	}
}