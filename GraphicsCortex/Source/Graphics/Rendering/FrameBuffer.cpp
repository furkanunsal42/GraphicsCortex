#include "Framebuffer.h"
#include "Debuger.h"
#include <iostream>

unsigned int Framebuffer::Channel_to_OpenGL(Channel channel)
{
	switch (channel) {
	case Framebuffer::Channel::COLOR:					return GL_COLOR_BUFFER_BIT;
	case Framebuffer::Channel::DEPTH:					return GL_DEPTH_BUFFER_BIT;
	case Framebuffer::Channel::STENCIL:					return GL_STENCIL_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_DEPTH:				return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_STENCIL:			return GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
	case Framebuffer::Channel::COLOR_DEPTH_STENCIL:		return GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
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

Framebuffer& Framebuffer::get_screen()
{
	static Framebuffer screen = Framebuffer(0);
	return screen;
}

bool Framebuffer::is_screen()
{
	return id.value == 0;
}

Framebuffer::Framebuffer()
{
	generate_framebuffer();
}

Framebuffer::~Framebuffer()
{
	release();
}

void Framebuffer::release()
{
	if (!framebuffer_generated) return;
	if (id.value == 0) return;

	GLCall(glDeleteFramebuffers(1, &id.value));

	framebuffer_generated = false;
}

void Framebuffer::bind_read_draw()
{
    if (!is_screen())
        _check_framebuffer_status(GL_FRAMEBUFFER);

	if (!framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to bind_read_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, id.value));
}

void Framebuffer::bind_read()
{
	_check_framebuffer_status(GL_READ_FRAMEBUFFER);

	if (!framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to bind_read() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, id.value));
}

void Framebuffer::bind_draw()
{
    if (!is_screen())
        _check_framebuffer_status(GL_DRAW_FRAMEBUFFER);

	if (!framebuffer_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to bind_draw() but resource was released" << std::endl;
		ASSERT(false);
	}

	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id.value));
}

void Framebuffer::attach_color(int32_t slot, Texture1D& texture1d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture1d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture1D was released" << std::endl;
		ASSERT(false);
	}
	
	owned_color_attachments[slot] = nullptr;
	texture1d._allocate_texture();

	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, texture1d.id, mipmap_level));
}

void Framebuffer::attach_color(int32_t slot, Texture2D& texture2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture2D was released" << std::endl;
		ASSERT(false);
	}
	
	owned_color_attachments[slot] = nullptr;
	texture2d._allocate_texture();

	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, texture2d.id, mipmap_level));
}

void Framebuffer::attach_color(int32_t slot, Texture2DArray& texture_array, int32_t z, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture_array._allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id.value, GL_COLOR_ATTACHMENT0 + slot, texture_array.id, mipmap_level, z));
}

void Framebuffer::attach_color(int32_t slot, TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int32_t mipmap){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture_cube_map._allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id.value, GL_COLOR_ATTACHMENT0 + slot, texture_cube_map.id, mipmap, TextureCubeMap::get_gl_face_index(face)));
}

void Framebuffer::attach_color(int32_t slot, Texture3D& texture3d, int32_t z, int32_t mipmap){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture3D was released" << std::endl;
		ASSERT(false);
	}
	if (z >= texture3d.depth) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but z was greater than depth of Texture3D" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture3d._allocate_texture();

	GLCall(glNamedFramebufferTextureLayer(id.value, GL_COLOR_ATTACHMENT0 + slot, texture3d.id, mipmap, z));
}

void Framebuffer::attach_color_layered(int32_t slot, Texture2DArray& texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture_array._allocate_texture();

	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, texture_array.id, mipmap_level));
}

void Framebuffer::attach_color_layered(int32_t slot, TextureCubeMap& texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture_cube_map._allocate_texture();

	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, texture_cube_map.id, mipmap_level));
}

void Framebuffer::attach_color_layered(int32_t slot, Texture3D& texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color_layered() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	texture3d._allocate_texture();

	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, texture3d.id, mipmap_level));
}

void Framebuffer::attach_color(int32_t slot, Renderbuffer& render_buffer){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Renderbuffer was released" << std::endl;
		ASSERT(false);
	}

	owned_color_attachments[slot] = nullptr;
	render_buffer._allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id.value, GL_COLOR_ATTACHMENT0 + slot, GL_RENDERBUFFER, render_buffer.id));
}


void Framebuffer::attach_depth(Texture2D& texture2d, int32_t mipmap_level) {
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture2D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture2d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_ATTACHMENT, texture2d.id, mipmap_level));
}

void Framebuffer::attach_depth(Texture2DArray& texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_ATTACHMENT, texture_array.id, mipmap_level, z));
}

void Framebuffer::attach_depth(TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_ATTACHMENT, texture_cube_map.id, mipmap_level, TextureCubeMap::get_gl_face_index(face)));
}

void Framebuffer::attach_depth(Texture3D& texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_ATTACHMENT, texture3d.id, mipmap_level, z));
}

void Framebuffer::attach_depth_layered(Texture2DArray& texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;
	
	owned_depth_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_ATTACHMENT, texture_array.id, mipmap_level));
}

void Framebuffer::attach_depth_layered(TextureCubeMap& texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_ATTACHMENT, texture_cube_map.id, mipmap_level));
}

void Framebuffer::attach_depth_layered(Texture3D& texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_ATTACHMENT, texture3d.id, mipmap_level));
}

void Framebuffer::attach_depth(Renderbuffer& render_buffer2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Renderbuffer was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_depth_attachment = nullptr;
	render_buffer2d._allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id.value, GL_DEPTH_ATTACHMENT, render_buffer2d.target, render_buffer2d.id));
}

void Framebuffer::attach_stencil(Texture2D& texture2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture2D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture2d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_STENCIL_ATTACHMENT, texture2d.id, mipmap_level));
}

void Framebuffer::attach_stencil(Texture2DArray& texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_STENCIL_ATTACHMENT, texture_array.id, mipmap_level, z));
}

void Framebuffer::attach_stencil(TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_STENCIL_ATTACHMENT, texture_cube_map.id, mipmap_level, TextureCubeMap::get_gl_face_index(face)));
}

void Framebuffer::attach_stencil(Texture3D& texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_STENCIL_ATTACHMENT, texture3d.id, mipmap_level, z));
}


void Framebuffer::attach_stencil_layered(Texture2DArray& texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_STENCIL_ATTACHMENT, texture_array.id, mipmap_level));
}

void Framebuffer::attach_stencil_layered(TextureCubeMap& texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_STENCIL_ATTACHMENT, texture_cube_map.id, mipmap_level));
}

void Framebuffer::attach_stencil_layered(Texture3D& texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_STENCIL_ATTACHMENT, texture3d.id, mipmap_level));
}
void Framebuffer::attach_stencil(Renderbuffer& render_buffer2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Renderbuffer was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_stencil_attachment = nullptr;

	owned_stencil_attachment = nullptr;
	render_buffer2d._allocate_texture();
	GLCall(glNamedFramebufferRenderbuffer(id.value, GL_STENCIL_ATTACHMENT, render_buffer2d.target, render_buffer2d.id));
}

void Framebuffer::attach_depth_stencil(Texture2D& texture2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture2D was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_attachment = nullptr;
	owned_stencil_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture2d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture2d.id, mipmap_level));
}

void Framebuffer::attach_depth_stencil(Texture2DArray& texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture_array.id, mipmap_level, z));
}

void Framebuffer::attach_depth_stencil(TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture_cube_map.id, mipmap_level, TextureCubeMap::get_gl_face_index(face)));
}

void Framebuffer::attach_depth_stencil(Texture3D& texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTextureLayer(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture3d.id, mipmap_level, z));
}

void Framebuffer::attach_depth_stencil_layered(Texture2DArray& texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_array._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but Texture2DArray was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture_array._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture_array.id, mipmap_level));
}

void Framebuffer::attach_depth_stencil_layered(TextureCubeMap& texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture_cube_map._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but TextureCubeMap was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture_cube_map._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture_cube_map.id, mipmap_level));
}

void Framebuffer::attach_depth_stencil_layered(Texture3D& texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!texture3d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but Texture3D was released" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	owned_depth_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	texture3d._allocate_texture();
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_STENCIL_ATTACHMENT, texture3d.id, mipmap_level));
}

void Framebuffer::attach_depth_stencil(Renderbuffer& render_buffer2d, int32_t mipmap_level){
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (!render_buffer2d._texture_generated) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Renderbuffer was released" << std::endl;
		ASSERT(false);
	}

	owned_depth_attachment = nullptr;
	owned_stencil_attachment = nullptr;

	owned_depth_stencil_attachment = nullptr;
	render_buffer2d.force_allocation();
	GLCall(glNamedFramebufferRenderbuffer(id.value, GL_DEPTH_STENCIL_ATTACHMENT, render_buffer2d.target, render_buffer2d.id));
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<Texture1D> texture1d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture1d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture1D was nullptr" << std::endl;
		ASSERT(false);
	}
	
	attach_color(slot, *texture1d, mipmap_level);

	owned_color_attachments[slot] = texture1d;
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture2D was nullptr" << std::endl;
		ASSERT(false);
	}
	
	attach_color(slot, *texture2d, mipmap_level);

	owned_color_attachments[slot] = texture2d;
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<Texture2DArray> texture_array, int32_t z, int32_t mipmap_level) {
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_color(slot, *texture_array, z, mipmap_level);
	
	owned_color_attachments[slot] = texture_array;
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<TextureCubeMap> texturecubemap, TextureCubeMap::Face face, int32_t mipmap_level) {
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texturecubemap == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_color(slot, *texturecubemap, face, mipmap_level);

	owned_color_attachments[slot] = texturecubemap;
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<Texture3D> texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_color(slot, *texture3d, z, mipmap_level);

	owned_color_attachments[slot] = texture3d;
}

void Framebuffer::attach_color(int32_t slot, std::shared_ptr<Renderbuffer> render_buffer)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_color() but Renderbuffer was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_color(slot, *render_buffer);

	owned_color_attachments[slot] = render_buffer;
}

void Framebuffer::deattach_color(int32_t slot)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deattach_color() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to deattach_color() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}

	deactivate_draw_buffer(slot);
	owned_color_attachments[slot] = nullptr;
	GLCall(glNamedFramebufferTexture(id.value, GL_COLOR_ATTACHMENT0 + slot, 0, 0));
}

void Framebuffer::attach_depth(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture2D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth(*texture2d, mipmap_level);

	owned_depth_attachment = texture2d;
}

void Framebuffer::attach_depth(std::shared_ptr<Texture2DArray> texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth(*texture_array, z, mipmap_level);

	owned_depth_attachment = texture_array;
}

void Framebuffer::attach_depth(std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth(*texture_cube_map, face, mipmap_level);

	owned_depth_attachment = texture_cube_map;
}

void Framebuffer::attach_depth(std::shared_ptr<Texture3D> texture3d, int32_t z, int32_t mipmap_level) 
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth(*texture3d, z, mipmap_level);

	owned_depth_attachment = texture3d;
}

void Framebuffer::attach_depth_layered(std::shared_ptr<Texture2DArray> texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_layered(*texture_array, mipmap_level);

	owned_depth_attachment = texture_array;
}

void Framebuffer::attach_depth_layered(std::shared_ptr<TextureCubeMap> texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_layered(*texture_cube_map, mipmap_level);

	owned_depth_attachment = texture_cube_map;
}

void Framebuffer::attach_depth_layered(std::shared_ptr<Texture3D> texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_layered() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_layered(*texture3d, mipmap_level);

	owned_depth_attachment = texture3d;
}

void Framebuffer::attach_depth(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth() but Renderbuffer was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth(*render_buffer);

	owned_depth_attachment = render_buffer;
}

void Framebuffer::deattach_depth()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deattach_depth() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}

	owned_depth_attachment = nullptr;
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_ATTACHMENT, 0, 0));
}

void Framebuffer::attach_stencil(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture2D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil(*texture2d, mipmap_level);

	owned_stencil_attachment = texture2d;
}

void Framebuffer::attach_stencil(std::shared_ptr<Texture2DArray> texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil(*texture_array, z, mipmap_level);

	owned_stencil_attachment = texture_array;
}

void Framebuffer::attach_stencil(std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil(*texture_cube_map, face, mipmap_level);

	owned_stencil_attachment = texture_cube_map;
}

void Framebuffer::attach_stencil(std::shared_ptr<Texture3D> texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil(*texture3d, z, mipmap_level);

	owned_stencil_attachment = texture3d;
}

void Framebuffer::attach_stencil_layered(std::shared_ptr<Texture2DArray> texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil_layered(*texture_array, mipmap_level);

	owned_stencil_attachment = texture_array;
}

void Framebuffer::attach_stencil_layered(std::shared_ptr<TextureCubeMap> texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil_layered(*texture_cube_map, mipmap_level);

	owned_stencil_attachment = texture_cube_map;
}

void Framebuffer::attach_stencil_layered(std::shared_ptr<Texture3D> texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil_layered() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil_layered(*texture3d, mipmap_level);

	owned_stencil_attachment = texture3d;
}

void Framebuffer::attach_stencil(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_stencil() but Renderbuffer was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_stencil(*render_buffer);

	owned_stencil_attachment = render_buffer;
}

void Framebuffer::deattach_stencil()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deattach_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}

	owned_stencil_attachment = nullptr;
	GLCall(glNamedFramebufferTexture(id.value, GL_STENCIL_ATTACHMENT, 0, 0));
}

void Framebuffer::attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture2d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture2D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil(*texture2d, mipmap_level);

	owned_depth_stencil_attachment = texture2d;
}

void Framebuffer::attach_depth_stencil(std::shared_ptr<Texture2DArray> texture_array, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil(*texture_array, z, mipmap_level);

	owned_depth_stencil_attachment = texture_array;
}

void Framebuffer::attach_depth_stencil(std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil(*texture_cube_map, face, mipmap_level);

	owned_depth_stencil_attachment = texture_cube_map;
}

void Framebuffer::attach_depth_stencil(std::shared_ptr<Texture3D> texture3d, int32_t z, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil(*texture3d, z, mipmap_level);

	owned_depth_stencil_attachment = texture3d;
}

void Framebuffer::attach_depth_stencil_layered(std::shared_ptr<Texture2DArray> texture_array, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_array == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but Texture2DArray was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil_layered(*texture_array, mipmap_level);

	owned_depth_stencil_attachment = texture_array;
}

void Framebuffer::attach_depth_stencil_layered(std::shared_ptr<TextureCubeMap> texture_cube_map, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture_cube_map == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but TextureCubeMap was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil_layered(*texture_cube_map, mipmap_level);

	owned_depth_stencil_attachment = texture_cube_map;
}

void Framebuffer::attach_depth_stencil_layered(std::shared_ptr<Texture3D> texture3d, int32_t mipmap_level)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil_layered() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (texture3d == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil_layered() but Texture3D was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil_layered(*texture3d, mipmap_level);

	owned_depth_stencil_attachment = texture3d;
}


void Framebuffer::attach_depth_stencil(std::shared_ptr<Renderbuffer> render_buffer)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to attach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (render_buffer == nullptr) {
		std::cout << "[OpenGL Error] Framebuffer tried to attach_depth_stencil() but Renderbuffer was nullptr" << std::endl;
		ASSERT(false);
	}

	attach_depth_stencil(*render_buffer);

	owned_depth_stencil_attachment = render_buffer;
}

void Framebuffer::deattach_depth_stencil()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deattach_depth_stencil() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	
	owned_depth_stencil_attachment = nullptr;
	GLCall(glNamedFramebufferTexture(id.value, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0));
}

void Framebuffer::clear()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to clear() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	
	deattach_depth();
	deattach_stencil();
	deattach_depth_stencil();
	for (int i = 0; i < slot_count; i++)
		deattach_color(i);
}

void Framebuffer::set_read_buffer(int32_t slot)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to set_read_buffer() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to set_read_buffer() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}

	GLCall(glNamedFramebufferReadBuffer(id.value, GL_COLOR_ATTACHMENT0 + slot));
}

void Framebuffer::activate_draw_buffer(int32_t slot)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to activate_draw_buffer() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to activate_draw_buffer() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	
	active_draw_buffers.insert(slot);
	draw_buffers_are_updated = false;
}

void Framebuffer::deactivate_draw_buffer(int32_t slot)
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deactivate_draw_buffer() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (slot < 0 || slot >= slot_count) {
		std::cout << "[OpenGL Error] Framebuffer tried to deactivate_draw_buffer() to slot " << slot << " but there are maximum of " << slot_count << " attacment slots in a Framebuffer" << std::endl;
		ASSERT(false);
	}
	
	active_draw_buffers.erase(slot);
	draw_buffers_are_updated = false;
}

void Framebuffer::update_activated_draw_buffers()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to update_activated_draw_buffers() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	if (draw_buffers_are_updated) return;

	int count = active_draw_buffers.size();
	unsigned int buffer_enums[slot_count];
	int i = 0;
	for (auto iterator = active_draw_buffers.begin(); iterator != active_draw_buffers.end(); iterator++) {
		int slot = *iterator;
		ASSERT(i < count);
		ASSERT(slot >= 0 && slot < slot_count);
		buffer_enums[i] = GL_COLOR_ATTACHMENT0 + (unsigned int)slot;
		i++;
	}
	GLCall(glNamedFramebufferDrawBuffers(id.value, count, buffer_enums));
	draw_buffers_are_updated = true;
}

void Framebuffer::deactivate_all_draw_buffers()
{
	if (id.value == 0) {
		std::cout << "[OpenGL Error] Screen Framebuffer tried to deactivate_all_draw_buffers() but Screen is not allowed to call this function" << std::endl;
		ASSERT(false);
	}
	active_draw_buffers.clear();
	draw_buffers_are_updated = false;
}

void Framebuffer::blit(
	Framebuffer& target, 
	int32_t self_x0, int32_t self_y0, int32_t self_x1, int32_t self_y1, 
	int32_t target_x0, int32_t target_y0, int32_t target_x1, int32_t target_y1, 
	Channel channel, Filter filter
) {
	if (id.value != 0)
		target.update_activated_draw_buffers();

	GLCall(glBlitNamedFramebuffer(
		id.value, target.id.value,
		self_x0, self_y0, self_x1, self_y1, 
		target_x0, target_y0, target_x1, target_y1, 
		Channel_to_OpenGL(channel), Filter_to_OpenGL(filter))
	);
}

void Framebuffer::blit(
	Framebuffer& target,
	glm::ivec4 self_offset_size,
	glm::ivec4 target_offset_size,
	Channel channel, Filter filter
) {
	blit(
		target,
		self_offset_size.x, self_offset_size.y, self_offset_size.z, self_offset_size.w,
		target_offset_size.x, target_offset_size.y, target_offset_size.z, target_offset_size.w,
		channel, filter
	);
}

void Framebuffer::blit(
	Framebuffer& target,
	glm::ivec2 self_offset, glm::ivec2 self_size,
	glm::ivec2 target_offset, glm::ivec2 target_size,
	Channel channel, Filter filter
) {
	blit(
		target,
		self_offset.x, self_offset.y, self_size.x, self_size.y,
		target_offset.x, target_offset.y, target_size.x, target_size.y,
		channel, filter
	);
}

void Framebuffer::blit(
	Framebuffer& target,
	glm::ivec2 self_size,
	glm::ivec2 target_size,
	Channel channel, Filter filter
) {
	blit(
		target,
		0, 0, self_size.x, self_size.y,
		0, 0, target_size.x, target_size.y,
		channel, filter
	);
}

void Framebuffer::blit_to_screen(
	int32_t self_x0, int32_t self_y0, int32_t self_x1, int32_t self_y1,
	int32_t target_x0, int32_t target_y0, int32_t target_x1, int32_t target_y1, 
	Channel channel, Filter filter
) {
	GLCall(glBlitNamedFramebuffer(
		id.value, 0,
		self_x0, self_y0, self_x1, self_y1, 
		target_x0, target_y0, target_x1, target_y1, 
		Channel_to_OpenGL(channel), Filter_to_OpenGL(filter))
	);
}

void Framebuffer::blit_to_screen(
	glm::ivec4 self_offset_size,
	glm::ivec4 target_offset_size,
	Channel channel, Filter filter
) {
	blit_to_screen(
		self_offset_size.x, self_offset_size.y, self_offset_size.z, self_offset_size.w,
		target_offset_size.x, target_offset_size.y, target_offset_size.z, target_offset_size.w,
		channel, filter
	);
}

void Framebuffer::blit_to_screen(
	glm::ivec2 self_offset, glm::ivec2 self_size,
	glm::ivec2 target_offset, glm::ivec2 target_size,
	Channel channel, Filter filter
) {
	blit_to_screen(
		self_offset.x, self_offset.y, self_size.x, self_size.y,
		target_offset.x, target_offset.y, target_size.x, target_size.y,
		channel, filter
	);
}

void Framebuffer::blit_to_screen(
	glm::ivec2 self_size,
	glm::ivec2 target_size,
	Channel channel, Filter filter
) {
	blit_to_screen(
		0, 0, self_size.x, self_size.y,
		0, 0, target_size.x, target_size.y,
		channel, filter
	);
}

Framebuffer::Framebuffer(uint32_t id) : 
	id(id) 
{ 
	if (id == 0)
		framebuffer_generated = true;
}

void Framebuffer::generate_framebuffer()
{
	GLCall(glCreateFramebuffers(1, &id.value));
	framebuffer_generated = true;
}

void Framebuffer::_check_framebuffer_status(unsigned int gl_bind_target)
{
	ASSERT(framebuffer_generated);

	GLCall(unsigned int error_status = glCheckNamedFramebufferStatus(id.value, gl_bind_target));
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
