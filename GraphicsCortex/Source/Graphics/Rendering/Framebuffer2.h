#pragma once
#include "TextureBase.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "TextureArray.h"
#include "TextureCubeMap.h"
#include "Texture3D.h"
#include "Renderbuffer.h"
#include <array>
#include <unordered_set>

class Framebuffer2 {
public:
	enum class Channel {
		COLOR,
		STENCIL,
		DEPTH,
		COLOR_STENCIL,
		COLOR_DEPTH,
		DEPTH_STENCIL,
		COLOR_DEPTH_STENCIL,
	};
	
	enum class Filter {
		NEAREST,
		LINEAR,
	};
	
	unsigned int id;

	Framebuffer2();
	Framebuffer2(const Framebuffer2& other) = delete;
	~Framebuffer2();
	void release();

	static void bind_screen_read_draw();
	static void bind_screen_read();
	static void bind_screen_draw();

	void bind_read_draw();
	void bind_read();
	void bind_draw();

	void attach_color(int slot, std::shared_ptr<Texture1D> texture1d, int mipmap_level = 0);
	void attach_color(int slot, std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_color(int slot, std::shared_ptr<TextureArray2> texture_array, int z, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<Texture3D> texture3d, int z, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<Renderbuffer2> render_buffer);

	void attach_depth(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_depth(std::shared_ptr<Renderbuffer2> render_buffer);

	void attach_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_stencil(std::shared_ptr<Renderbuffer2> render_buffer);

	void attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_depth_stencil(std::shared_ptr<Renderbuffer2> render_buffer);

	void set_read_buffer(int slot);

	void activate_draw_buffer(int slot);
	void deactivate_draw_buffer(int slot);
	void update_activated_draw_buffers();
	void deactivate_all_draw_buffers();

	void blit(Framebuffer2& target, int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter);
	void blit_to_screen(int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter);

private:
	
	static unsigned int Channel_to_OpenGL(Channel channel);
	static unsigned int Filter_to_OpenGL(Filter filter);

	bool _framebuffer_generated = false;
	bool _draw_buffers_are_updated = false;

	std::unordered_set<int> _active_draw_buffers;
	int _active_read_buffer = 0;
	bool _active_read_buffer_ever_set = false;

	std::array<std::shared_ptr<TextureBase2>, 16> _color_attachments;
	std::shared_ptr<TextureBase2> _depth_attachment;
	std::shared_ptr<TextureBase2> _stencil_attachment;
	std::shared_ptr<TextureBase2> _depth_stencil_attachment;

	void _check_framebuffer_status(unsigned int gl_bind_target);
};