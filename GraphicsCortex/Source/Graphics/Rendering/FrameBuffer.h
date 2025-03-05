#pragma once
#include <array>
#include <unordered_set>

#include "Movable.h"

#include "TextureBase.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"
#include "Texture3D.h"
#include "Renderbuffer.h"

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

	Framebuffer();
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer(Framebuffer&& other) = default;
	Framebuffer& operator=(const Framebuffer& other) = delete;
	Framebuffer& operator=(Framebuffer&& other) = default;
	~Framebuffer();
	void release();

	static Framebuffer& get_screen();
	bool is_screen();

	void bind_read_draw();
	void bind_read();
	void bind_draw();
	
	// doesn't assume ownership

	void attach_color(int32_t slot, Texture1D& texture1d, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, Texture2D& texture2d, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, Texture2DArray& texture_array, int32_t z, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, TextureCubeMap& texture_cube_map, TextureCubeMap::Face face, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, Texture3D& texture3d, int32_t z, int32_t mipmap_level = 0);
	void attach_color_layered(int32_t slot, Texture2DArray& texture_array, int32_t mipmap_level = 0);
	void attach_color_layered(int32_t slot, TextureCubeMap& texture_cube_map, int32_t mipmap_level = 0);
	void attach_color_layered(int32_t slot, Texture3D& texture3d, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, Renderbuffer& render_buffer);
	
	void attach_depth(Texture2D& texture2d, int32_t mipmap_level = 0);
	void attach_depth(Renderbuffer& render_buffer2d, int32_t mipmap_level = 0);
	
	void attach_stencil(Texture2D& texture2d, int32_t mipmap_level = 0);
	void attach_stencil(Renderbuffer& render_buffer2d, int32_t mipmap_level = 0);
	
	void attach_depth_stencil(Texture2D& texture2d, int32_t mipmap_level = 0);
	void attach_depth_stencil(Renderbuffer& render_buffer2d, int32_t mipmap_level = 0);

	// assumes ownership

	void attach_color(int32_t slot, std::shared_ptr<Texture1D> texture1d, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level = 0);
	void attach_color(int32_t slot, std::shared_ptr<Texture2DArray> texture_array, int32_t z, int32_t mipmap = 0);
	void attach_color(int32_t slot, std::shared_ptr<TextureCubeMap> texture_cube_map, TextureCubeMap::Face face, int32_t mipmap = 0);
	void attach_color(int32_t slot, std::shared_ptr<Texture3D> texture3d, int32_t z, int32_t mipmap = 0);
	void attach_color(int32_t slot, std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_color(int32_t slot);

	void attach_depth(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level = 0);
	void attach_depth(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_depth();

	void attach_stencil(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level = 0);
	void attach_stencil(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_stencil();

	void attach_depth_stencil(std::shared_ptr<Texture2D> texture2d, int32_t mipmap_level = 0);
	void attach_depth_stencil(std::shared_ptr<Renderbuffer> render_buffer);
	void deattach_depth_stencil();

	void clear();

	void set_read_buffer(int32_t slot);
	void activate_draw_buffer(int32_t slot);
	void deactivate_draw_buffer(int32_t slot);
	void deactivate_all_draw_buffers();
	
	void update_activated_draw_buffers();

	void blit(
		Framebuffer& target,
		int32_t self_x0, int32_t self_y0, int32_t self_x1, int32_t self_y1, 
		int32_t target_x0, int32_t target_y0, int32_t target_x1, int32_t target_y1, 
		Channel channel, Filter filter
	);
	
	void blit(
		Framebuffer& target, 
		glm::ivec4 self_offset_size, 
		glm::ivec4 target_offset_size, 
		Channel channel, Filter filter
	);
	
	void blit(
		Framebuffer& target, 
		glm::ivec2 self_offset, glm::ivec2 self_size, 
		glm::ivec2 target_offset, glm::ivec2 target_size, 
		Channel channel, Filter filter
	);
	
	void blit(
		Framebuffer& target, 
		glm::ivec2 self_size, 
		glm::ivec2 target_size, 
		Channel channel, Filter filter
	);

	void blit_to_screen(
		int32_t self_x0, int32_t self_y0, int32_t self_x1, int32_t self_y1,
		int32_t target_x0, int32_t target_y0, int32_t target_x1, int32_t target_y1,
		Channel channel, Filter filter
	);

	void blit_to_screen(
		glm::ivec4 self_offset_size,
		glm::ivec4 target_offset_size,
		Channel channel, Filter filter
	);

	void blit_to_screen(
		glm::ivec2 self_offset, glm::ivec2 self_size,
		glm::ivec2 target_offset, glm::ivec2 target_size,
		Channel channel, Filter filter
	);

	void blit_to_screen(
		glm::ivec2 self_size,
		glm::ivec2 target_size,
		Channel channel, Filter filter
	);

private:
	
	static constexpr uint32_t null_slot = -1;
	static constexpr uint32_t slot_count = 16;

	Framebuffer(uint32_t id);

	void generate_framebuffer();

	gl_handle id;
	bool framebuffer_generated = false;
	std::unordered_set<int32_t> active_draw_buffers;
	bool draw_buffers_are_updated = false;
	
	std::array<std::shared_ptr<TextureBase2>, slot_count> owned_color_attachments;
	std::shared_ptr<TextureBase2> owned_depth_attachment;
	std::shared_ptr<TextureBase2> owned_stencil_attachment;
	std::shared_ptr<TextureBase2> owned_depth_stencil_attachment;

	static unsigned int Channel_to_OpenGL(Channel channel);
	static unsigned int Filter_to_OpenGL(Filter filter);

	void _check_framebuffer_status(unsigned int gl_bind_target);
};