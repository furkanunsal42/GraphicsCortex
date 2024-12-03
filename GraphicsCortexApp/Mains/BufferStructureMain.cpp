#include "GraphicsCortex.h"

int main() {
	Frame frame(512, 512, "BufferStructureTest", 0, 1, true, true, false, Frame::NOTIFICATION, false);
		
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(512, Buffer::GPU_BUFFER);

	struct cpu_layout_struct {
		float a;
		float b[12];
		glm::vec4 c;
		glm::vec4 d;
		glm::vec2 e;

		struct x_struct {
			glm::vec4 a;
			glm::vec4 b[12];
		} x;

		struct y_struct {
			glm::vec4 a;
		} y[512];

	};

	auto layout =
		Buffer::layout(
			Buffer::float32(offsetof(cpu_layout_struct::a)),
			Buffer::float32_array(offsetof(cpu_layout_struct::b), 12),
			Buffer::vec4(),
			Buffer::vec4(),
			Buffer::vec2()
			//Buffer::structure(
			//	Buffer::vec4(),
			//	Buffer::vec4_array(12)
			//),
			//Buffer::structure_array(512,
			//	Buffer::vec4()
			//)
		);

	std::cout << "std430 size, alignment = " << layout._size_std430 << " " << layout._alignment_std430 << std::endl;
	std::cout << "std140 size, alignment = " << layout._size_std140 << " " << layout._alignment_std140 << std::endl;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();
	}
}