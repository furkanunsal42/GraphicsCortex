#pragma once

#include <string>
#include "Graphic.h"
#include "Scene.h"
#include "SharedPtr_Materials.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Text;

struct glyph_info {
	float x0, y0, x1, y1;	// coords of glyph in the texture atlas
	float x_off, y_off;		// left & top bearing when rendering
	float advance;			// x advance when rendering
};

class Text;

class Font {
public:
	Font(const std::string& filename = "Fonts\\Roboto-Thin.ttf", int font_size = 100);



private:
	
	Texture _font_atlas;
	std::unordered_map<uint32_t, glyph_info> glyphs;

	friend Text;
};

