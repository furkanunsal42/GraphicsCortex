#include "GraphicsCortex.h"

int main() {
	Vec2<float> vector(2.0f, 2.0f);
	vector = vector;
	std::cout << vector << std::endl;
	std::cin.get();
}