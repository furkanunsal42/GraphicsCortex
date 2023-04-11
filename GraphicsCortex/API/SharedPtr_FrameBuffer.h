#pragma once
#include "SharedPtr.h"
#include "FrameBuffer.h"

class FrameBuffer_s {
public:

	_SHARED_POINTER_DEFINITIONS(FrameBuffer);

	FrameBuffer_s(int width, int height, int anti_alliasing = 0, bool readable_depth_stencil_buffer = false) :
		obj(std::make_shared<FrameBuffer>(width, height, anti_alliasing, readable_depth_stencil_buffer)) {}

};