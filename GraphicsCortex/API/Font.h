#pragma once

#include <string>
#include "Graphic.h"
#include "Scene.h"

#include <ft2build.h>
#include FT_FREETYPE_H

struct glyph_info {
	float x0, y0, x1, y1;	// coords of glyph in the texture atlas
	float x_off, y_off;		// left & top bearing when rendering
	float advance;			// x advance when rendering
};

class Font {
public:
	Font(const std::string& filename, int font_size = 100);

	void generate_text_graphic(const std::string& text, Scene& scene, float scale = 1, float text_max_width = 0, bool wait_for_words = true);
	void render();
	Graphic_s graphics_representation;
	Texture _font_atlas;
private:
	
	std::unordered_map<uint32_t, glyph_info> glyphs;
	vec4 color;
};