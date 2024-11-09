#include "GraphicsCortex.h"

int main() {
	Frame frame(512, 512, "Buffer Test", 0, 0, true, true, true, Frame::NOTIFICATION, false);
	Scene scene(frame);

	size_t size = 40;
	Buffer buffer(size);
	Buffer buffer2(size);

	std::vector<int> vector_a = { 10, 20, 30, 40 };
	buffer.set_data(4, 0, vector_a);

	buffer.map();
	std::span<int> span = buffer.get_mapped_span<int>();
	
	span[0] = 100;
	span[9] = 1;

	buffer.copy_to(0, 0, size, buffer2);

	auto vector_b = buffer2.get_data<int8_t>();

	AttributedVertexBuffer vao;
	std::cout << vao.get_max_attribute_count() << std::endl;

	for (auto& element : vector_b)
		std::cout << (int)element << " ";
	std::cout << std::endl;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();


	}
}