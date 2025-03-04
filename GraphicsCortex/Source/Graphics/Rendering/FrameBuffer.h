#pragma once
#include "TextureBase.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"
#include "Texture3D.h"
#include "Renderbuffer.h"
#include <array>
#include <unordered_set>

class Framebuffer {
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
	
	uint32_t id;

	static Framebuffer& get_screen();

	Framebuffer();
	Framebuffer(const Framebuffer& other) = delete;
	~Framebuffer();
	void release();

	void bind_read_draw();
	void bind_read();
	void bind_draw();
	
	// doesn't assume ownership

	void attach_color(int slot, Texture1D& texture1d, int mipmap_level = 0);
	void attach_color(int slot, Texture2D& texture2d, int mipmap_level = 0);
	void attach_color(int slot, Texture2DArray& texture_array, int z, int mipmap = 0);
	void attach_color(int slot, TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int mipmap = 0);
	void attach_color(int slot, Texture3D& texture3d, int z, int mipmap = 0);
	void attach_color(int slot, Renderbuffer& render_buffer);
	
	void atatch_depth(Texture2D& texture2d, int mipmap_level = 0);
	void atatch_depth(Renderbuffer& render_buffer2d, int mipmap_level = 0);
	
	void attach_stencil(Texture2D& texture2d, int mipmap_level = 0);
	void attach_stencil(Renderbuffer& render_buffer2d, int mipmap_level = 0);
	
	void attach_depth_stencil(Texture2D& texture2d, int mipmap_level = 0);
	void attach_depth_stencil(Renderbuffer& render_buffer2d, int mipmap_level = 0);

	// assumes ownership

	void attach_color(int slot, std::shared_ptr<Texture1D> texture1d, int mipmap_level = 0);
	void attach_color(int slot, std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_color(int slot, std::shared_ptr<Texture2DArray> texture_array, int z, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<Texture3D> texture3d, int z, int mipmap = 0);
	void attach_color(int slot, std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_color(int slot);

	void attach_depth(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_depth(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_depth();

	void attach_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_stencil(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_stencil();

	void attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int mipmap_level = 0);
	void attach_depth_stencil(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_depth_stencil();

	void clear();

	void set_read_buffer(int slot);
	void activate_draw_buffer(int slot);
	void deactivate_draw_buffer(int slot);
	void deactivate_all_draw_buffers();
	
	//void update_activated_draw_buffers();

	void blit(Framebuffer& target, int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter);
	void blit_to_screen(int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Channel channel, Filter filter);

private:
	
	static constexpr uint32_t null_slot = -1;
	static constexpr uint32_t slot_count = 16;

	Framebuffer(uint32_t id);

	void generate_framebuffer();

	bool framebuffer_generated = false;
	int active_read_buffer = 0;
	std::unordered_set<int> active_draw_buffers;
	//bool draw_buffers_are_updated = false;
	
	std::array<std::shared_ptr<TextureBase2>, slot_count> owned_color_attachments;
	std::shared_ptr<TextureBase2> owned_depth_attachment;
	std::shared_ptr<TextureBase2> owned_stencil_attachment;
	std::shared_ptr<TextureBase2> owned_depth_stencil_attachment;

	static unsigned int Channel_to_OpenGL(Channel channel);
	static unsigned int Filter_to_OpenGL(Filter filter);

	//bool _active_read_buffer_ever_set = false;

	void _check_framebuffer_status(unsigned int gl_bind_target);
};