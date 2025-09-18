#include "Font.h"

#include "WindowBoundGlobalResources.h"

#include "ft2build.h"
#include FT_FREETYPE_H

FontBank& FontBank::get()
{
	if (active_global_resources->FontBank == nullptr)
		active_global_resources->FontBank = std::unique_ptr<FontBank>(new FontBank());
	
	return *active_global_resources->FontBank;
}

font_id FontBank::load_font(const std::filesystem::path& font_file_path, int32_t font_size)
{
	auto iterator = _path_to_id.find(font_file_path);
	if (iterator != _path_to_id.end())
		return _path_to_id[font_file_path];
	else {
		_path_to_id[font_file_path] = _next_id;
		_id_to_font[_next_id] = _load(font_file_path, font_size);
		_next_id++;
		return _next_id - 1;
	}
}

FontBank::Font FontBank::get_font(font_id font)
{
	if (does_font_exist(font))
		return _id_to_font[font];
	else
		return Font();
}

bool FontBank::does_font_exist(font_id font)
{
	auto iterator = _id_to_font.find(font);
	return iterator != _id_to_font.end();
}

void FontBank::clear()
{
	_path_to_id.clear();
	_id_to_font.clear();
	_next_id = 1;
}

FontBank::Font FontBank::_load(const std::filesystem::path& font_file_path, int32_t font_size)
{
	Font font;

	FT_Library ft;
	FT_Face    face;

	FT_Init_FreeType(&ft);
	FT_New_Face(ft, (const char*)font_file_path.string().c_str(), 0, &face);
	
	FT_Set_Char_Size(
		face, 
		0, 
		font_size * 64, 
		96, 
		96
	);

	glm::ivec2 max_texture_resolution(4096);

	glm::ivec2 pen(0);
	int32_t glyph_counter;
	for (glyph_counter = 0; glyph_counter < face->num_glyphs; glyph_counter++) {
		if (!FT_Get_Char_Index(face, glyph_counter))
			continue;

		FT_Error char_load_error = FT_Load_Char(face, glyph_counter, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (pen.x + bmp->width + 1 >= max_texture_resolution.x) {
			if (pen.y + (face->size->metrics.height >> 6) >= max_texture_resolution.y) {
				std::cout << "here" << std::endl;
				break;
			}
			pen.x = 0;
			pen.y += ((face->size->metrics.height >> 6));
		}
		pen.x += bmp->width + 1;

		glyph_counter++;
	}

	glm::ivec2 texture_resolution = glm::ivec2(max_texture_resolution.x, pen.y);
	std::cout << texture_resolution.x << ", " << texture_resolution.y << std::endl;

	unsigned char* png_data = new unsigned char[
		texture_resolution.x * 
		texture_resolution.y * 
		Texture2D::ColorTextureFormat_bytes_per_pixel(Texture2D::ColorTextureFormat::RGBA8)
		];

	
	pen = glm::ivec2(0);

	for (int i = 0; i < glyph_counter; ++i) {
		if (!FT_Get_Char_Index(face, i)) {
			//std::cout << "failed to load char with index: " << i << std::endl;
			continue;
		}

		FT_Error char_load_error = FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
		FT_Bitmap* bmp = &face->glyph->bitmap;

		if (pen.x + bmp->width + 1 >= max_texture_resolution.x) {
			if (pen.y + (face->size->metrics.height >> 6) >= texture_resolution.y) {
				std::cout << "here2" << std::endl;
				break;
			}
			pen.x = 0;
			pen.y += ((face->size->metrics.height >> 6));
		}
		
		for (int row = 0; row < bmp->rows; ++row) {
			for (int col = 0; col < bmp->width; ++col) {
				int x = pen.x + col;
				int y = pen.y + row;

				png_data[((texture_resolution.y - 1 - y) * texture_resolution.x + x) * 4 + 0] = 0xff;
				png_data[((texture_resolution.y - 1 - y) * texture_resolution.x + x) * 4 + 1] = 0xff;
				png_data[((texture_resolution.y - 1 - y) * texture_resolution.x + x) * 4 + 2] = 0xff;
				png_data[((texture_resolution.y - 1 - y) * texture_resolution.x + x) * 4 + 3] = bmp->buffer[row * bmp->width + col];
			}
		}

		// this is stuff you'd need when rendering individual glyphs out of the atlas
		glyph_info info;
		info.coords_low.x = (float)pen.x / texture_resolution.x;
		info.coords_low.y = (float)pen.y / texture_resolution.y;
		info.coords_hi.x = (float)(pen.x + bmp->width) / texture_resolution.x;
		info.coords_hi.y = (float)(pen.y + bmp->rows)  / texture_resolution.y;
		info.offset.x = (float)face->glyph->bitmap_left / texture_resolution.x;
		info.offset.y = (float)face->glyph->bitmap_top / texture_resolution.y;
		info.advance = (float)(face->glyph->advance.x >> 6) / texture_resolution.x;

		font.glyph_table[i] = info;

		pen.x += bmp->width;
	}

	glyph_info new_line;
	new_line.coords_low.x = 0;
	new_line.coords_low.y = 0;
	new_line.coords_hi.x = 0;
	new_line.coords_hi.y = (float)((face->size->metrics.height) >> 6) / texture_resolution.y;
	font.glyph_table['\n'] = new_line;

	FT_Done_FreeType(ft);

	std::shared_ptr<Image> font_atlas = std::make_shared<Image>(png_data, 
		texture_resolution.x, 
		texture_resolution.y, 
		1, 
		Texture2D::ColorTextureFormat_bytes_per_pixel(Texture2D::ColorTextureFormat::RGBA8), 
		1, 
		true
	);

	font_atlas->save_to_disc("atlas.png");

	font.atlas = std::make_shared<Texture2D>(texture_resolution.x, texture_resolution.y, Texture2D::ColorTextureFormat::RGBA8, 32, -0.75f, 0);
	font.atlas->load_data_with_mipmaps(*font_atlas, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE);
	font.font_size = font_size;

	return font;
}
