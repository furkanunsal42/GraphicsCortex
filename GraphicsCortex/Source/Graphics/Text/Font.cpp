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

font_id FontBank::load_font(const std::filesystem::path& font_file_path)
{
	auto iterator = _path_to_id.find(font_file_path);
	if (iterator != _path_to_id.end())
		return _path_to_id[font_file_path];
	else {
		_path_to_id[font_file_path] = _next_id;
		_id_to_font[_next_id] = _load(font_file_path);
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

FontBank::Font FontBank::_load(const std::filesystem::path& font_file_path)
{
	Font font;

	FT_Library ft;
	FT_Face    face;

	FT_Init_FreeType(&ft);
	FT_New_Face(ft, (const char*)font_file_path.u8string().c_str(), 0, &face);
	// TEMP
	int32_t font_size = 50;
	FT_Set_Char_Size(face, 0, font_size << 6, 96, 96);

	// quick and dirty max texture size estimate

	int max_glyph_amount = 2 << 12;
	int supported_glyph_amount = 0;
	for (int i = 0; i < max_glyph_amount; i++) {
		if (!FT_Get_Char_Index(face, i))
			continue;
		supported_glyph_amount++;
	}
	int tex_height = font_size * 1.2f * std::sqrt(supported_glyph_amount);
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
		info.coords_low.x = (float)pen_x / tex_width;
		info.coords_low.y = (float)pen_y / tex_height;
		info.coords_hi.x = (float)(pen_x + bmp->width) / tex_width;
		info.coords_hi.y = (float)(pen_y + bmp->rows) / tex_height;
		info.offset.x = (float)face->glyph->bitmap_left / tex_width;
		info.offset.y = (float)face->glyph->bitmap_top / tex_height;
		info.advance = (float)(face->glyph->advance.x >> 6) / tex_width;

		font.glyph_table[i] = info;

		pen_x += bmp->width + 1;
	}

	glyph_info new_line;
	new_line.coords_low.x = 0;
	new_line.coords_low.y = 0;
	new_line.coords_hi.x = 0;
	new_line.coords_hi.y = (float)((face->size->metrics.height) >> 6) / tex_height;
	font.glyph_table['\n'] = new_line;

	FT_Done_FreeType(ft);

	std::shared_ptr<Image> font_atlas = std::make_shared<Image>(png_data, tex_width, tex_height, 1, 4, 1, true);
	font_atlas->save_to_disc("atlas.png");

	font.atlas = std::make_shared<Texture2D>(tex_width, tex_height, Texture2D::ColorTextureFormat::RGBA8, 32, -0.75f, 0);
	font.atlas->load_data_with_mipmaps(*font_atlas, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE);

	return font;
}
