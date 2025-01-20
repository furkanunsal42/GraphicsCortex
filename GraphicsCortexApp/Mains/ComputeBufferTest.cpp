#include "GraphicsCortex.h"

int main() {

	std::shared_ptr<Frame> frame = std::make_shared<Frame>(0, 0, "GraphicsCortexComputeBuffers", 0, 0, true, true, false, Frame::NOTIFICATION, false);

	size_t size = 1024 * sizeof(float);
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(size);
	std::shared_ptr<ComputeProgram> kernel = std::make_shared<ComputeProgram>(Shader("../GraphicsCortex/Source/GLSL/Compute/buffer_test.comp"));

	kernel->update_uniform_as_storage_buffer("my_buffer", *buffer, 0, 1024 * sizeof(float));
	kernel->dispatch_thread(1024, 1, 1);
	
	std::vector<float> data = buffer->get_data<float>(0, 1024);

	for (int i = 0; i < data.size(); i++)
		std::cout << data[i] << " ";
	std::cout << std::endl;

	std::cin.get();
}