#pragma once
#include "TextureBase.h"
#include "Texture1D.h"
#include "Texture2D.h"
#include "TextureArray.h"
#include "TextureCubeMap.h"
#include "Texture3D.h"
#include "RenderBuffer.h"

class FrameBuffer2 {
public:
	enum class Filter {
		NEAREST,
		LINEAR,
	};

	enum class Channel {
		COLOR,
		STENCIL,
		DEPTH,
		COLOR_STENCIL,
		COLOR_DEPTH,
		DEPTH_STENCIL,
		COLOR_DEPTH_STENCIL,
	};
	
	unsigned int id;

	FrameBuffer2() = delete;
	FrameBuffer2(const FrameBuffer2& other) = delete;

	void bind_read_draw();
	void bind_read();
	void bind_draw();

	void attach(int slot, Texture1D texture1d);
	void attach(int slot, Texture2D texture2d);
	void attach(int slot, TextureArray2 texture_array);
	void attach(int slot, TextureCubeMap texture_cube_map);
	void attach(int slot, Texture3D texture3d);
	void attach(int slot, RenderBuffer2 render_buffer);

	void blit(FrameBuffer2& target, int self_x0, int self_y0, int self_x1, int self_y1, int target_x0, int target_y0, int target_x1, int target_y1, Filter filter, Channel channel);



private:

};