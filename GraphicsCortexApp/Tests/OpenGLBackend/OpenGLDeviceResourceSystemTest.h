#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class OpenGLDeviceResourceSystemTest : public TestBench {
public:

	bool run() {
		default_init();

		OpenGLBackend::DeviceResource<
			offsetof(Texture2D, mipmap_begin_level),
			offsetof(Texture2D, texture_handle)
			> resource1;


		return true;
	}
};
