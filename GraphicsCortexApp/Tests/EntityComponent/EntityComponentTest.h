#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

class EntityComponentSystemTest : public TestBench {
public:

	bool run() {

		default_init();

		Scene scene;

		std::shared_ptr<Entity> entity_a = std::make_shared<Entity>();
		entity_a->add_component<MeshComponent>();
		entity_a->add_component<MeshRendererComponent>();

		std::shared_ptr<Entity> entity_b = std::make_shared<Entity>();
		entity_b->add_component<MeshComponent>();

		auto mesh_comp = entity_b->get_component<MeshComponent>();
		auto mesh_r_comp = entity_b->get_component<MeshRendererComponent>();
		std::cout << mesh_comp << std::endl;
		std::cout << mesh_r_comp << std::endl;
		std::cout << "----------------------------" << std::endl;

		auto print_state = [&]() {
			std::cout << "Entitiy count: " << scene.get_entities().size() << std::endl;
			std::cout << "MeshComponent count: " << scene.get_components<MeshComponent>().size() << std::endl;
			std::cout << "MeshRendererComponent count: " << scene.get_components<MeshRendererComponent>().size() << std::endl;
			std::cout << "----------------------------" << std::endl;
			};

		scene.add_entity(entity_a);
		std::cout << "A added" << std::endl;
		print_state();

		scene.remove_entity(entity_a);
		std::cout << "A removed" << std::endl;
		print_state();

		scene.add_entity(entity_b);
		std::cout << "B added" << std::endl;
		print_state();

		scene.add_entity(entity_a);
		std::cout << "A added" << std::endl;
		print_state();

		scene.remove_entity(entity_b);
		std::cout << "B removed" << std::endl;
		print_state();

		scene.remove_entity(entity_b);
		std::cout << "B removed again" << std::endl;
		print_state();


		while (true) {
			default_window->handle_events(true);
			primitive_renderer::clear(0, 0, 0, 1);

			scene.tick();
		}

		return true;
	}
};
