#pragma once
#include "SharedPtr.h"
#include "Camera.h"

class Camera_s {
public:
	_SHARED_POINTER_DEFINITIONS(Camera);

	Camera_s() :
		obj(std::make_shared<Camera>()) {}
	Camera_s(float width, float height) :
		obj(std::make_shared<Camera>(width, height)) {}

};