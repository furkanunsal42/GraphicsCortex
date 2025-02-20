#pragma once
#include "Component.h"
#include <iostream>

class MeshRendererComponent : public Component {
public:
	
	void get_material();
	void get_mesh();

};