#pragma once

#include "SharedPtr.h"
#include "Font.h"

class Font_s {
public:

	_SHARED_POINTER_DEFINITIONS(Font);

	Font_s(const std::string& filename = "Fonts\\Roboto-Thin.ttf", int font_size = 100) :
		obj(std::make_shared<Font>(filename, font_size)) {}
	
};
