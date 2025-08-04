#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class OpenGLResourceSystemTest : public TestBench {
public:

	bool run() {
		default_init();

		//OpenGLBackend::OpenGLResource resource1(OpenGLBackend::Texture2D);
		//std::cout << resource1.id << " " << resource1.get_reference_count() << std::endl;
		//OpenGLBackend::OpenGLResource resource2 = resource1;
		//std::cout << resource1.id << " " << resource1.get_reference_count() << std::endl;
		//std::cout << resource2.id << " " << resource2.get_reference_count() << std::endl;
		//OpenGLBackend::OpenGLResource resource3(resource2);
		//std::cout << resource1.id << " " << resource1.get_reference_count() << std::endl;
		//std::cout << resource2.id << " " << resource2.get_reference_count() << std::endl;
		//std::cout << resource3.id << " " << resource3.get_reference_count() << std::endl;
		//resource1 = std::move(resource3);
		//std::cout << resource1.id << " " << resource1.get_reference_count() << std::endl;
		//std::cout << resource2.id << " " << resource2.get_reference_count() << std::endl;
		//std::cout << resource3.id << " " << resource3.get_reference_count() << std::endl;

		return true;
	}
};
