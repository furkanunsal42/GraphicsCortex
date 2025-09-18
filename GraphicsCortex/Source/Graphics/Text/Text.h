#pragma once

#include "ShaderCompiler.h"
#include "Font.h"
#include "Mesh.h"
#include "PrimitiveRenderer.h"

class Text : public UpdatesDefaultUniforms {
public:
	Text(const std::u32string& text = U"");
	//Text(const std::string& text);
	~Text() = default;

	void set_font(font_id font);
	font_id get_font();

	void set_text(const std::u32string& text);
	//void set_text(const std::string& text);

	void set_scale(float scale);
	float get_scale() const;
	void set_max_width(float max_width);
	float get_max_width();
	void set_wait_for_words(bool wait_for_words);
	bool get_wait_for_words();
	void set_color(const glm::vec4& color);
	glm::vec4 get_color();
	glm::vec2 get_size();

	void update_default_uniforms(Program& program);

private:
	std::shared_ptr<Mesh> mesh;

	std::u32string _text;
	float _width;
	float _height;
	float _scale = 1;
	float _text_max_width = 0;
	bool _wait_for_words = true;
	glm::vec4 _color = glm::vec4(0.5, 0.5, 0.5, 1);

	bool _mesh_needs_update = true;
	void _update_mesh();

	friend void primitive_renderer::render(
		Framebuffer& framebuffer,
		Program& program,
		Text& text,
		const RenderParameters& render_parameters,
		size_t instance_count,
		size_t instance_offset
	);

	friend void primitive_renderer::render(
		Program& program,
		Text& text,
		const RenderParameters& render_parameters,
		size_t instance_count,
		size_t instance_offset
	);

	font_id font = 1;

	static std::u32string to_utf32(const std::string& string_utf8);

	//static std::shared_ptr<Program> _default_text_renderer;
};