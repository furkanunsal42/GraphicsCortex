#include "GraphicsCortex.h"
#include<source_location>

int main() {
	Frame frame(512, 512, "BufferStructureTest", 0, 1, true, true, false, Frame::NOTIFICATION, false);
		
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(512, Buffer::GPU_BUFFER);

	struct cpu_layout_struct {
		glm::vec3 a;
		float b;
		float c;
	};

	Buffer::layout layout =
		Buffer::layout(
			Buffer::float32(layout_map_to(cpu_layout_struct, a)),
			Buffer::float32(layout_map_to(cpu_layout_struct, b)),
			Buffer::float32_array(layout_map_to(cpu_layout_struct, c), 32)
		);


	//std::cout << "std430 size, alignment = " << layout._size_std430 << " " << layout._alignment_std430 << std::endl;
	//std::cout << "std140 size, alignment = " << layout._size_std140 << " " << layout._alignment_std140 << std::endl;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();
	}
}