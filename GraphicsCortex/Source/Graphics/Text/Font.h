#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include <vec2.hpp>
#include <Texture2D.h>

#include <iostream>
typedef uint32_t font_id;

class FontBank {
public:
	
	constexpr static font_id not_a_font = 0;

	static FontBank& get();
	
	struct glyph_info {
		glm::vec2 coords_low;	// coords of glyph in the texture atlas
		glm::vec2 coords_hi;
		glm::vec2 offset;		// left & top bearing when rendering
		float advance;			// x advance when rendering
	};

	struct Font {
		std::shared_ptr<Texture2D> atlas = nullptr;
		std::unordered_map<uint32_t, glyph_info> glyph_table;
		int32_t font_size;
		float text_height;
	};

	font_id load_font(const std::filesystem::path& font_file_path, int32_t font_size = 32);
	const Font& get_font(font_id font);
	bool does_font_exist(font_id font);

	void clear();

private:

	FontBank() = default;

	Font _load(const std::filesystem::path& font_file_path, int32_t font_size = 32);

	font_id _next_id = 1;
	std::unordered_map<std::filesystem::path, font_id> _path_to_id;
	std::unordered_map<font_id, Font> _id_to_font;
};