#include "GraphicsCortex.h"
#include "GraphRenderer/GraphRenderer.h"
#include "TextureArithmatic/TextureArithmatic.h"
#include "TextureArithmatic/GraphicsOperation.h"

int main() {
	int window_width = 1024;

	Frame frame(window_width, window_width, "CTReconstructor GraphRenderer", 0, 0, true, true, false, Frame::CallbackLevel::NOTIFICATION, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	GraphicsOperation op;

	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(32);
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(512, 512 ,Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	op.compute(
		*buffer,
		*buffer, "",
		*texture, false
	);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.display_performance();
		frame.clear_window();
	}
}