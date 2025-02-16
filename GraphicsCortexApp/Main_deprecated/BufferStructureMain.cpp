#include "GraphicsCortex.h"
#include<source_location>
#include<type_traits>

int main() {
	Frame frame(0, 0, "BufferStructureTest", 0, 1, true, true, false, Frame::NOTIFICATION, false);
		
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(512, Buffer::GPU_BUFFER);

	struct cpu_layout_struct {
		float a;
		glm::vec3 b;
		float c[32];
		float d;
	};

	Buffer::layout layout =
		Buffer::layout(
			Buffer::float32(layout_map_to(cpu_layout_struct, a)),
			Buffer::vec3(layout_map_to(cpu_layout_struct, b)),
			Buffer::float32(layout_map_to(cpu_layout_struct, c)),
			Buffer::float32(layout_map_to(cpu_layout_struct, d))
		);

	std::cout << "std140 layout" << std::endl;
	for (int i = 0; i < layout.layout_std140.size(); i++)
		std::cout << layout.layout_std140[i].begin_offset << " ";
	std::cout << std::endl;
	
	std::cout << std::endl;
	std::cout << "std430 layout" << std::endl;
	for (int i = 0; i < layout.layout_std430.size(); i++)
		std::cout << layout.layout_std430[i].begin_offset << " ";
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "cpu layout" << std::endl;
	for (int i = 0; i < layout.layout_cpu.size(); i++)
		std::cout << layout.layout_cpu[i].count << " ";
	std::cout << std::endl;

	//std::cout << "std430 size, alignment = " << layout._size_std430 << " " << layout._alignment_std430 << std::endl;
	//std::cout << "std140 size, alignment = " << layout._size_std140 << " " << layout._alignment_std140 << std::endl;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();
	}
}