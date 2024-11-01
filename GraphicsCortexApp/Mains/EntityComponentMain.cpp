#include "GraphicsCortex.h"

int main() {

	Frame frame(512, 512, "EntityComponentTest", 0, 1, true, true, true, Frame::NOTIFICATION, false);
	Scene scene(frame);

	std::shared_ptr<Entity> entity = std::make_shared<Entity>();

	entity->add_component<MeshComponent>();
	entity->remove_component<MeshRendererComponent>();

	std::weak_ptr<MeshComponent> mesh_component = entity->get_component<MeshComponent>();

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window();
		frame.display_performance(128);

		
	}

}