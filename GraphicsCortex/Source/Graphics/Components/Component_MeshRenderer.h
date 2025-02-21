#pragma once
#include "Component/Component.h"
#include "Component_Mesh.h"
#include "Component_Material.h"
#include "Component_Transform.h"

#include "Camera.h"
#include "FrameBuffer.h"

#include <iostream>

class MeshRendererComponent : public Component {
public:
	
	MaterialComponent* get_material();
	MeshComponent* get_mesh();
	TransformComponent* get_transform();

	void render(Framebuffer& framebuffer, Camera& camera);
	void render(Camera& camera);
};