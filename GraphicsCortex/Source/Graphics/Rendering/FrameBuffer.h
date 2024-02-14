#pragma once
#include "Config.h"

#include "GL\glew.h"

#include <string>
#include "Texture.h"
#include "ShaderCompiler.h"
#include "Graphic.h"

#include <type_traits>
#include <memory>

class FrameBuffer {
public:
	enum
	{
		COLOR_TEXTURE = 0,
		DEPTH_TEXTURE,
		STENCIL_TEXTURE,
	};

	unsigned int id = 0;
	int width = 0, height = 0;
	int texture_slot = 9;
	Texture color_texture;
	Texture depth_stencil_texture;
	RenderBuffer depth_stencil_renderbuffer;
	bool readable_depth_stencil_buffer;

	std::shared_ptr<Program> program;

	FrameBuffer(int width = 1024, int height = 1024, int anti_alliasing = 0, bool readable_depth_stencil_buffer = false);
	~FrameBuffer();

	void release();

	bool is_initialized();

	enum {
		WRITE_TARGET = 1,
		READ_TARGET = 2,
		WRITE_READ_TARGET = 1 | 2,
	};

	void bind(unsigned int bind_target = WRITE_READ_TARGET);
	void unbind();
	void render(unsigned int source_texture = FrameBuffer::COLOR_TEXTURE);
	Image save(bool vertical_flip = true);

	void load_program(std::shared_ptr<Program> program);

	enum {
		COLOR_CHANNEL = 1,
		DEPTH_CHANNEL = 2,
		STENCIL_CHANNEL = 4,
		ALL_CHANNELS = 1 | 2 | 4
	};

	enum {
		BLIT_NEAREST_INTERPOLATION,
		BLIT_LINEAR_INTERPOLATION,
	};

	void blit_section(FrameBuffer& target_buffer, const glm::vec4& source_rect, const glm::vec4& target_rect, unsigned int mask_bits = ALL_CHANNELS, unsigned int interpolation = BLIT_NEAREST_INTERPOLATION);
	void blit_section_to_screen(const glm::vec4& source_rect, const glm::vec4& target_rect, unsigned int mask_bits = ALL_CHANNELS, unsigned int interpolation = BLIT_NEAREST_INTERPOLATION);

private:

	void generate_framebuffer_object();
	std::shared_ptr<Graphic> screen;
	
	bool _is_initialized = false;
	bool _is_program_loaded = false;
	int multisample = 0;
	bool screen_initialized = false;
};