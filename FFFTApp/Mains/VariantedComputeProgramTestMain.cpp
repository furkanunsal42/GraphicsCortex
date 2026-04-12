#include "GraphicsCortex.h"
#include "Tools/VariantedComputeProgram.h"

void main() {

	WindowDescription desc;
	desc.w_resolution = glm::ivec2(512);

	Window window(desc);

	VariantedComputeProgram program(Shader("../FFFT/Source/GLSL/FFT/copy.comp"));
	
	while (!window.should_close()) {

		window.handle_events(true);
		
		primitive_renderer::clear(0, 0, 0, 1);

		window.swap_buffers();
	}
}