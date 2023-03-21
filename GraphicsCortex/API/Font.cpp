#include "Font.h"
#include "Default_Programs.h"

Font::Font(const std::string& filename, int font_size) {
	FT_Library ft;
	FT_Face    face;

	FT_Init_FreeType(&ft);
	FT_New_Face(ft, filename.c_str(), 0, &face);
	FT_Set_Char_Size(face, 0, font_size << 6, 96, 96);

	// quick and dirty max texture size estimate

	int max_glyph_amount = 256;
	int supported_glyph_amount = 0;
	for (int i = 0; i < max_glyph_amount; i++) {
		if (!FT_Get_Char_Index(face, i))
			continue;
		supported_glyph_amount++;
	}
	int tex_height = font_size * std::sqrt(supported_glyph_amount);
	//tex_height = std::pow(2, std::ceil(std::log2(tex_height)));
	tex_height = std::max(512, tex_height);
	int tex_width = tex_height;

	// render glyphs to atlas

	unsigned char* png_data = new unsigned char[tex_width * tex_height * 4];

	int pen_x = 0, pen_y = 0;

	for (int i = 0; i < max_glyph_amount; ++i) {
		if (!FT_Get_Char_Index(face, i)) {
			//std::cout << "failed to load char with index: " << i << std::endl;
			continue;
		}

		FT_Error char_load_error = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (pen_x + bmp->width >= tex_width) {
			pen_x = 0;
			pen_y += ((face->size->metrics.height >> 6) + 1);
		}

		for (int row = 0; row < bmp->rows; ++row) {
			for (int col = 0; col < bmp->width; ++col) {
				int x = pen_x + col;
				int y = pen_y + row;
				png_data[((tex_height - 1 - y) * tex_width + x) * 4 + 0] = 0xff;
				png_data[((tex_height - 1 - y) * tex_width + x) * 4 + 1] = 0xff;
				png_data[((tex_height - 1 - y) * tex_width + x) * 4 + 2] = 0xff;
				png_data[((tex_height - 1 - y) * tex_width + x) * 4 + 3] = bmp->buffer[row * bmp->pitch + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas
		glyph_info info;
		info.x0 = (float)pen_x / tex_width;
		info.y0 = (float)pen_y / tex_height;
		info.x1 = (float)(pen_x + bmp->width) / tex_width;
		info.y1 = (float)(pen_y + bmp->rows) / tex_height;

		info.x_off = (float)face->glyph->bitmap_left / tex_width;
		info.y_off = (float)face->glyph->bitmap_top / tex_height;
		info.advance = (float)(face->glyph->advance.x >> 6) / tex_width;

		glyphs[i] = info;

		pen_x += bmp->width + 1;
	}

	glyph_info new_line;
	new_line.x0 = 0;
	new_line.y0 = 0;
	new_line.x1 = 0;
	new_line.y1 = (float)((face->size->metrics.height) >> 6) / tex_height;
	glyphs['\n'] = new_line;

	FT_Done_FreeType(ft);

	Image font_atlas(png_data, tex_width, tex_height, 4, true);

	_font_atlas.mipmap_bias = 0.5f;
	_font_atlas.load_image(font_atlas);

	//graphics_representation->load_program(default_program::text_program_s());

}

void Font::generate_text_graphic(const std::u32string& text, Scene& scene, float scale, float text_max_width, bool wait_for_words) {

	std::vector<float> verticies;
	std::vector<unsigned int> indicies;

	float pen_x = 0;
	float pen_y = 0;

	int char_count = 0;

	bool next_line_at_next_space = false;

	for (const uint32_t& character : text) {

		if (text_max_width) {
			if (pen_x > text_max_width)
				next_line_at_next_space = true;
		}

		const glyph_info& character_info = glyphs[character];

		float uv_width = character_info.x1 - character_info.x0;
		float uv_height = character_info.y1 - character_info.y0;

		float xpos = pen_x + character_info.x_off * scale;
		float ypos = pen_y - (uv_height - character_info.y_off) * scale;

		float w = uv_width * scale;
		float h = uv_height * scale;

		if (character == ' ') {
			if (next_line_at_next_space) {
				pen_x = 0;
				pen_y -= glyphs['\n'].y1 * scale;
				next_line_at_next_space = false;
				continue;
			}
		}

		if (character == '\n') {
			pen_x = 0;
			pen_y -= uv_height * scale;
			next_line_at_next_space = false;
			continue;
		}

		float new_verticies[] = {
			xpos,		ypos,		0,	character_info.x0, 1 - character_info.y1,
			xpos + w,	ypos,		0,	character_info.x1, 1 - character_info.y1,
			xpos,		ypos + h,	0,	character_info.x0, 1 - character_info.y0,
			xpos + w,	ypos + h,	0,	character_info.x1, 1 - character_info.y0,
		};

		unsigned int new_indicies[] = {
			0 + char_count * 4, 1 + char_count * 4, 3 + char_count * 4,
			0 + char_count * 4, 3 + char_count * 4, 2 + char_count * 4,
		};

		for (float value : new_verticies)
			verticies.push_back(value);

		for (unsigned int value : new_indicies)
			indicies.push_back(value);

		pen_x += character_info.advance * scale;
		char_count++;

	}

	ArrayBuffer_s arraybuffer;
	arraybuffer->load_buffer(verticies);
	arraybuffer->push_attribute(3);
	arraybuffer->push_attribute(2);

	IndexBuffer_s indexbuffer;
	indexbuffer->load_buffer(indicies, 3);

	Mesh_s text_mesh(arraybuffer, indexbuffer);
	graphics_representation->load_model(text_mesh);
	//graphics_representation->set_uniform_all(default_program::basic_uniform_queue(scene, graphics_representation));
	//graphics_representation->set_uniform("texture_slot", 0);
	//graphics_representation->set_uniform("screen_resolution", (float*)&scene.camera.screen_width, (float*)&scene.camera.screen_height);
}

void Font::generate_text_graphic(const std::u16string& text, Scene& scene, float scale, float text_max_width, bool wait_for_words) {

	std::vector<float> verticies;
	std::vector<unsigned int> indicies;

	float pen_x = 0;
	float pen_y = 0;

	int char_count = 0;

	bool next_line_at_next_space = false;

	for (const uint16_t& character : text) {

		if (text_max_width) {
			if (pen_x > text_max_width)
				next_line_at_next_space = true;
		}

		const glyph_info& character_info = glyphs[character];

		float uv_width = character_info.x1 - character_info.x0;
		float uv_height = character_info.y1 - character_info.y0;

		float xpos = pen_x + character_info.x_off * scale;
		float ypos = pen_y - (uv_height - character_info.y_off) * scale;

		float w = uv_width * scale;
		float h = uv_height * scale;

		if (character == ' ') {
			if (next_line_at_next_space) {
				pen_x = 0;
				pen_y -= glyphs['\n'].y1 * scale;
				next_line_at_next_space = false;
				continue;
			}
		}

		if (character == '\n') {
			pen_x = 0;
			pen_y -= uv_height * scale;
			next_line_at_next_space = false;
			continue;
		}

		float new_verticies[] = {
			xpos,		ypos,		0,	character_info.x0, 1 - character_info.y1,
			xpos + w,	ypos,		0,	character_info.x1, 1 - character_info.y1,
			xpos,		ypos + h,	0,	character_info.x0, 1 - character_info.y0,
			xpos + w,	ypos + h,	0,	character_info.x1, 1 - character_info.y0,
		};

		unsigned int new_indicies[] = {
			0 + char_count * 4, 1 + char_count * 4, 3 + char_count * 4,
			0 + char_count * 4, 3 + char_count * 4, 2 + char_count * 4,
		};

		for (float value : new_verticies)
			verticies.push_back(value);

		for (unsigned int value : new_indicies)
			indicies.push_back(value);

		pen_x += character_info.advance * scale;
		char_count++;

	}

	ArrayBuffer_s arraybuffer;
	arraybuffer->load_buffer(verticies);
	arraybuffer->push_attribute(3);
	arraybuffer->push_attribute(2);

	IndexBuffer_s indexbuffer;
	indexbuffer->load_buffer(indicies, 3);

	Mesh_s text_mesh(arraybuffer, indexbuffer);
	graphics_representation->load_model(text_mesh);
	//graphics_representation->set_uniform_all(default_program::basic_uniform_queue(scene, graphics_representation));
	//graphics_representation->set_uniform("texture_slot", 0);
	//graphics_representation->set_uniform("screen_resolution", (float*)&scene.camera.screen_width, (float*)&scene.camera.screen_height);
}

void Font::generate_text_graphic(const std::string& text, Scene& scene, float scale, float text_max_width, bool wait_for_words) {

	std::vector<float> verticies;
	std::vector<unsigned int> indicies;

	float pen_x = 0;
	float pen_y = 0;

	int char_count = 0;

	bool next_line_at_next_space = false;

	for (const uint8_t& character : text) {

		if (text_max_width) {
			if (pen_x > text_max_width)
				next_line_at_next_space = true;
		}

		const glyph_info& character_info = glyphs[character];

		float uv_width = character_info.x1 - character_info.x0;
		float uv_height = character_info.y1 - character_info.y0;

		float xpos = pen_x + character_info.x_off * scale;
		float ypos = pen_y - (uv_height - character_info.y_off) * scale;

		float w = uv_width * scale;
		float h = uv_height * scale;

		if (character == ' ') {
			if (next_line_at_next_space) {
				pen_x = 0;
				pen_y -= glyphs['\n'].y1 * scale;
				next_line_at_next_space = false;
				continue;
			}
		}

		if (character == '\n') {
			pen_x = 0;
			pen_y -= uv_height * scale;
			next_line_at_next_space = false;
			continue;
		}

		float new_verticies[] = {
			xpos,		ypos,		0,	character_info.x0, 1 - character_info.y1,
			xpos + w,	ypos,		0,	character_info.x1, 1 - character_info.y1,
			xpos,		ypos + h,	0,	character_info.x0, 1 - character_info.y0,
			xpos + w,	ypos + h,	0,	character_info.x1, 1 - character_info.y0,
		};

		unsigned int new_indicies[] = {
			0 + char_count * 4, 1 + char_count * 4, 3 + char_count * 4,
			0 + char_count * 4, 3 + char_count * 4, 2 + char_count * 4,
		};

		for (float value : new_verticies)
			verticies.push_back(value);

		for (unsigned int value : new_indicies)
			indicies.push_back(value);

		pen_x += character_info.advance * scale;
		char_count++;

	}

	ArrayBuffer_s arraybuffer;
	arraybuffer->load_buffer(verticies);
	arraybuffer->push_attribute(3);
	arraybuffer->push_attribute(2);

	IndexBuffer_s indexbuffer;
	indexbuffer->load_buffer(indicies, 3);

	Mesh_s text_mesh(arraybuffer, indexbuffer);
	graphics_representation->load_model(text_mesh);
	//graphics_representation->set_uniform_all(default_program::basic_uniform_queue(scene, graphics_representation));
	//graphics_representation->set_uniform("texture_slot", 0);
	//graphics_representation->set_uniform("screen_resolution", (float*)&scene.camera.screen_width, (float*)&scene.camera.screen_height);
}
