#include "GraphicsCortex.h"
#include "Application/Application.h"

int main() {
	Newsletter<int> newsletter;
	
	size_t id = newsletter.subscribe([](int variable) {
		std::cout << variable << std::endl;
		});

	newsletter.unsubscribe(id);

	newsletter.publish(12);

	std::cin.get();
}