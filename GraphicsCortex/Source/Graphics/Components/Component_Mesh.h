#pragma once
#include "Component/Component.h"

#include "Mesh.h"

class MeshComponent : public Component {
public:

	MeshComponent() { this->mesh = std::make_shared<Mesh>(); }
	MeshComponent(std::shared_ptr<Mesh> mesh) { this->mesh = mesh; }

	std::shared_ptr<Mesh> mesh = nullptr;
};