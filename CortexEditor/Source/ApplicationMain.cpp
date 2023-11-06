#include "GraphicsCortex.h"

int main() {
	Frame frame(1920, 1080);
	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(1, 1, 1, 1);
		frame.display_performance(180);

	}
}