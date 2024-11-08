#include "GraphicsCortex.h"

int main() {
	Frame frame(512, 512, "Buffer Test", 0, 0, true, true, true, Frame::NOTIFICATION, false);
	Scene scene(frame);

	size_t size = 40;
	Buffer buffer(size);

	char* data = new char[size];
	std::memset(data, 69, size);

	buffer.set_data(10, 0, 20, data);
	buffer.map(5, 16);

	std::vector<float> vector = buffer.get_mapped_vector<float>();

	for (auto& element : vector)
		std::cout << (int)element << " ";
	std::cout << std::endl;

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();


	}
}