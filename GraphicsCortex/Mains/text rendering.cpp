#include "GraphicsCortex.h"

#include "stb_image_write.h"

struct glyph_info {
	float x0, y0, x1, y1;	// coords of glyph in the texture atlas
	float x_off, y_off;   // left & top bearing when rendering
	float advance;        // x advance when rendering
};

class Font {
public:
	std::unordered_map<uint32_t, glyph_info> glyphs;

	Font(const std::string& filename, int font_size = 16) {
		FT_Library ft;
		FT_Face    face;

		FT_Init_FreeType(&ft);
		FT_New_Face(ft, filename.c_str(), 0, &face);
		FT_Set_Char_Size(face, 0, font_size << 6, 96, 96);

		// quick and dirty max texture size estimate

		int glyph_amount = 256;
		int tex_height = font_size * std::sqrt(glyph_amount);
		tex_height = std::pow(2, std::ceil(std::log2(tex_height)));
		int tex_width = tex_height;

		// render glyphs to atlas

		unsigned char* png_data = (unsigned char*)calloc(tex_width * tex_height * 4, 1);

		int pen_x = 0, pen_y = 0;

		for (int i = 0; i < glyph_amount; ++i) {
			FT_Load_Char(face, i, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
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
					//png_data[((tex_height - 1 - y) * tex_width + x) * 4 + 3] = 0xff;
				}
			}

			// this is stuff you'd need when rendering individual glyphs out of the atlas
			glyph_info info;
			info.x0 = (float)pen_x / tex_width;
			info.y0 = (float)pen_y / tex_height;
			info.x1 = (float)(pen_x + bmp->width) / tex_width;
			info.y1 = (float)(pen_y + bmp->rows) / tex_height;

			info.x_off = (float)face->glyph->bitmap_left;
			info.y_off = (float)face->glyph->bitmap_top;
			info.advance = (float)(face->glyph->advance.x >> 6) / tex_width;

			glyphs[i] = info;

			pen_x += bmp->width + 1;
		}

		FT_Done_FreeType(ft);

		Image font_atlas(png_data, tex_width, tex_height, 4, true);
		font_atlas.save_to_disc("atlas.png");

		_font_atlas.load_image(font_atlas);

		Shader shader("Custom Shaders\\Texture.vert", "Custom Shaders\\Texture.frag");
		Program_s renderer(shader.vertex_shader, shader.fragment_shader);
		graphics_representation->load_program(renderer);

	}

	void generate_text_graphic(const std::string& text, Scene& scene) {

		std::vector<float> verticies;
		std::vector<unsigned int> indicies;

		float pen_x = 0;
		float pen_y = 0;

		int char_count = 0;

		for (const char& character : text) {
			const glyph_info& character_info = glyphs[character];
			
			std::cout << character_info.x_off << ", " << character_info.y_off << std::endl;

			float new_verticies[] = {
				0 + pen_x - character_info.x_off,	0 - character_info.y_off + pen_y,	0, character_info.x0, 1 - character_info.y1,
				1 + pen_x - character_info.x_off,	0 - character_info.y_off + pen_y,	0, character_info.x1, 1 - character_info.y1,
				0 + pen_x - character_info.x_off,	1 - character_info.y_off + pen_y,	0, character_info.x0, 1 - character_info.y0,
				1 + pen_x - character_info.x_off,	1 - character_info.y_off + pen_y,	0, character_info.x1, 1 - character_info.y0,
			};

			unsigned int new_indicies[] = {
				0 + char_count*4, 1 + char_count*4, 3 + char_count*4,
				0 + char_count*4, 3 + char_count*4, 2 + char_count*4,
			};

			for (float value : new_verticies)
				verticies.push_back(value);

			for (unsigned int value : new_indicies)
				indicies.push_back(value);

			pen_x += 1 + character_info.advance;
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
		graphics_representation->set_uniform_all(default_program::basic_uniform_queue(scene, graphics_representation));
		graphics_representation->set_uniform("texture_slot", 0);

	}
	Graphic_s graphics_representation;
	Texture _font_atlas;

};

int main() {

	Frame frame(1920, 1080, "GraphicsCortex", 0, 0, true, true, false, false);
	Scene scene(frame);

	Font font("Fonts\\Roboto-Thin.ttf", 100);
	font._font_atlas.texture_slot = 0;
	font.generate_text_graphic("Text in OpenGL?? Nani??", scene);

	while (frame.is_running()) {
		double frametime = frame.handle_window();
		frame.clear_window();
		frame.display_performance();

		scene.camera.handle_movements(frame.window, frametime);

		font._font_atlas.bind();
		scene.render();

		font.graphics_representation->update_matrix();
		font.graphics_representation->update_uniforms();
		font.graphics_representation->draw(false);
		//scene.render(false);
	}
}