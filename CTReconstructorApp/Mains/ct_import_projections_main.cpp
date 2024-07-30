#include "GraphicsCortex.h"
#include "Data/CTData.h"

int main() {
	Frame frame(2048 / 2, 1440 / 4, "CTAnalyzer", 0, 1, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	CTData data("C:/Users/FurkanPC/Desktop/Projektionen", 2048, 2048, 2, 1440 / 2);
	//CTData data2("C:/Users/FURKAN.UNSAL/Desktop/Projektionen", 2048, 2048, 2, 1440 / 2);

	std::shared_ptr<Framebuffer> readframebuffer = std::make_shared<Framebuffer>();
	readframebuffer->attach_color(0, data.projections, 0);
	readframebuffer->set_read_buffer(0);

	int i = 0;
	while (frame.is_running()) {
		double delta_time = frame.handle_window();
		frame.clear_window();
		frame.display_performance(180);

		i++;
		i = i % 2048;
		readframebuffer->attach_color(0, data.projections, i);
		readframebuffer->blit_to_screen(0, 0, 2048, 1440 / 2, 0, 0, 2048 / 2, 1440 / 4, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
	}
}