#pragma once

#include "CortexMath.h"
#include "Frame.h"
#include "ShaderCompiler.h"
#include "Text.h"
#include "SharedPtr_Graphic.h"

#include <functional>
#include <chrono>
#include <optional>
#include <iostream>

unsigned int compute_gui_id(std::string filename, int line);
#define gui_id compute_gui_id(__FILE__, __COUNTER__)

class Layout {
public:
	Layout(uint32_t type, const AABB2& aabb);
	Layout(uint32_t type = Layout::Horizional, const Vec2<float>& position = {0, 0}, const Vec2<float>& window_size = {0, 0});

	Vec2<float> window_size;
	Vec2<float> position;
	Vec2<float> _raw_position;

	enum LayoutType {
		Horizional,
		Vertical,
	};

	LayoutType type;

	void add_widget(const Vec2<float>& size);
	Vec2<float> get_widget_position();
	Vec2<float> get_centered_widget_position(unsigned int widget_index, const Vec2<float>& object_size);

	Vec2<float> get_position();
	Vec2<float> get_raw_position();
	void clear();
	void update_position_by_style(const Vec4<float>& margin, const Vec4<float>& padding);

	std::vector<Vec2<float>> widget_positions;
};

namespace {
	typedef double Time;
}

namespace gui {
	
	typedef vec3f color;
	typedef vec2f displacement;
	typedef vec2f rotation_euler;
	typedef vec4f corner_rounding;
	typedef vec4f padding;
	typedef vec4f margin;
	typedef vec4f border_thickness;
	typedef vec3f border_color;

	color colorcode(uint32_t hexcode);

}

class _interpolation{
public:

	_interpolation() = default;
		
	enum type{
		Linear,
		Polynomial
	};

	type interpolation_type = _interpolation::Linear;
	float power = 1.0f;
	
	template<typename T>
	T interpolate(T default_value, T target_value, Time max_time, Time current_time) {
		switch (interpolation_type) {
		case Linear:
			return _linear_interpolation(default_value, target_value, max_time, current_time);
			break;
		case Polynomial:
			return _polynomial_interpolation(default_value, target_value, max_time, current_time);
			break;
		}
	}

private:
	template<typename T>
	T _linear_interpolation(T default_value, T target_value, Time max_time, Time current_time) {
		return default_value + (target_value - default_value) / max_time * (std::min(current_time, max_time));
	}
	template<typename T>
	T _polynomial_interpolation(T default_value, T target_value, Time max_time, Time current_time) {
		return _linear_interpolation(default_value, target_value, std::pow(max_time, power), std::pow(current_time, power));
	}
};

namespace Interpolation {
	_interpolation linear();
	_interpolation polynomial(float power = 1.0f);
}

struct _widget_info;

struct Persentage {
public:
	enum RespectedAttribute {
		SIZE_X,
		SIZE_Y,
		PADDED_SIZE_X,
		PADDED_SIZE_Y,
		MAX_SIZE_DIM,
		MIN_SIZE_DIM,
		POSITION_X,
		POSITION_Y,
		PADDED_POSITION_X,
		PADDED_POSITION_Y,
		PARENT_WIDTH,
		PARENT_HEIGHT,
		AVAILABLE_WIDTH,
		AVAILABLE_HEIGHT,
	};

	static std::string RespectedAttribute_to_string(const RespectedAttribute& attribute);


	Persentage(float value = 0, RespectedAttribute attribute_type = MIN_SIZE_DIM);

	float get_value(const _widget_info& info) const;

	RespectedAttribute attribute_type;
	float value;
};


template<typename T>
struct StyleAttribute {
public:

	StyleAttribute() :
		use_persentage(false), value(T()) {}

	bool use_value = false;
	bool use_persentage = false;
	union {
		T value;
		Persentage persentage;
	};

	void operator=(const T& other) {
		value = other;
		use_value = true;
		use_persentage = false;
	}

	void operator=(const Persentage& other) {
		persentage = other;
		use_persentage = true;
		use_value = false;
	}

	void disable(){
		use_value = use_persentage = false;
	}

	bool exist() const {
		return use_value || use_persentage;
	}

	T get_value(const _widget_info& info) const {
		if (use_value)
			return value;
		else if (use_persentage) {
			return T(persentage.get_value(info));
		}
		else {
			return T();	// default case 
		}
	}
};


class StaticStyle {
public:
	enum Stacking {
		Stack,
		Center,
		Inverse,
	};

	enum TextAlligning {
		CenterXY,
		Default,
		CenterX,
		CenterY,
	};

	// StyleAttribute<vec3f> color_attrib;	// work in progress
	StyleAttribute<vec3f> text_color;
	StyleAttribute<float> text_size;
	StyleAttribute<vec3f> color;
	StyleAttribute<vec2f> displacement;
	StyleAttribute<vec2f> rotation_euler;	// not implemented 
	StyleAttribute<vec4f> corner_rounding; // corner indicies follows traditional coordinate system partition	// 1 | 0
																													// --|-- 
																													// 2 | 3

	StyleAttribute<vec4f> padding;
	StyleAttribute<vec4f> margin;
	StyleAttribute<vec4f> border_thickness;
	StyleAttribute<vec3f> border_color;
	std::optional<Frame::CursorType> cursor_type;
	std::optional<Stacking> stacking_type;
	std::optional<TextAlligning> text_allign_type;

	virtual void clear();
};

class Style : public StaticStyle {
public:
	
	std::optional<Time> text_color_change;
	std::optional<Time> text_size_change;
	std::optional<Time> color_change;
	std::optional<Time> displacement_change;
	std::optional<Time> rotation_change;
	std::optional<Time> corner_rounding_change;
	std::optional<Time> padding_change;
	std::optional<Time> margin_change;
	std::optional<Time> border_thickness_change;
	std::optional<Time> border_color_change;

	std::optional<_interpolation> text_color_interpolation;
	std::optional<_interpolation> text_size_interpolation;
	std::optional<_interpolation> color_interpolation;
	std::optional<_interpolation> displacement_interpolation;
	std::optional<_interpolation> rotation_interpolation;
	std::optional<_interpolation> corner_rounding_interpolation;
	std::optional<_interpolation> padding_interpolation;
	std::optional<_interpolation> margin_interpolation;
	std::optional<_interpolation> border_thickness_interpolation;
	std::optional<_interpolation> border_color_interpolation;

	StaticStyle on_hover;
	StaticStyle on_active;

	void clear();

private:
};

class FunctionalStyle : public StaticStyle {

	void on_hover(std::function<Style(Time)> hover_function);
	void on_active(std::function<Style(Time)> active_function);
	void on_passive(std::function<Style(Time)> passive_function);

	void clear_hover();
	void clear_active();
	void clear_passive();

	

private:
	std::function<Style(Time)> _on_hover;
	std::function<Style(Time)> _on_active;
	std::function<Style(Time)> _on_passive;
};


// object based structure

struct _widget_info {
public:
	vec2f size;
	vec2f position;

	// widget is the most relevant widget to mouse
	bool is_active_hovering = false;
	bool is_hovering = false;
	//Time last_update;

	Time _current_text_color_time = 0;
	Time _current_text_size_time = 0;
	Time _current_color_time = 0;
	Time _current_displacement_time = 0;
	Time _current_rotation_time = 0;
	Time _current_corner_rounding_time = 0;
	Time _current_padding_time = 0;
	Time _current_margin_time = 0;
	Time _current_border_thickness_time = 0;
	Time _current_border_color_time = 0;

	void increment_time(Time deltatime);

	bool properly_initialized = false;
};

/*
class Gui;

class Box {
public:

	Box(Gui& gui, Style style, AABB2 aabb, uint32_t id = 0);
	Box(Gui& gui, Style style, AABB2 aabb, Program_s custom_renderer, uint32_t id = 0);
	
	void set_position(Vec2<float> position);
	void set_size(Vec2<float> size);

	bool is_mouse_hover();

	void render();

	Style overwrite_style;

	vec2f get_original_size();
	vec2f get_original_position();

	bool hovering();
	bool click_released();
	bool click_pressed();
	bool click_holding();

	vec2 get_current_size();
	vec2 get_current_position();
	StaticStyle get_current_style();

	std::unordered_map<std::string, float> data;

	void initialize_data(const std::string& key, int value);

	void set_text(std::u32string& string);
	std::u32string& get_text();

	uint32_t _id = 0;
private:

	Style _style;
	Gui& _gui_ref;
	AABB2 _aabb;				//aabb size will be padded and displaced
	vec2 _original_size;		//original size will not be padded		
	vec2 _original_position;	//original size will not be displaced
	Graphic_s _graphic_representation;

	vec2 _current_position;			// after interpolation
	vec2 _current_size;				// after interpolation
	StaticStyle _current_style;		// after interpolation

	std::u32string _text_string = U"";
	std::shared_ptr<Text> _text;

	_widget_info _info;

	friend Gui;
};

class Gui {	// similiar function with UI class, new implementation 
public:
	Gui(Frame& frame);
	void new_frame(Time frame_time);
	Camera camera;
	Vec2<int> window_size;
	bool _hover_happened;
	Frame::CursorType _dominant_cursor_style = Frame::Arrow;

	void render(Time delatime);
	
	Box& box(AABB2 aabb, Style style, std::u32string text = U"", bool draw = true);
	Box& box(vec2 position, vec2 size, Style style, std::u32string text = U"", bool draw = true);

	void layout(vec2 position, vec2 min_size, Style style, std::u32string text = U"", Layout::LayoutType layout_type = Layout::Vertical, bool draw = true);
	Box& layout_end();

	Box& content(vec2 size, Style style, std::u32string text = U"", bool draw = true, bool force_insert = false);

	Frame& get_frame_referance();

	void change_layout_z(float z_displacement);
private:

	Time _frame_time_ms;

	std::vector<Box> widget_table;
	//std::vector<Text> font_table;
	std::vector<std::function<void(Time)>> render_queue;

	std::vector<Layout> layout_table;
	std::vector<vec2> layout_min_size_table;
	std::vector<Style> layout_styles_table;
	std::vector<std::u32string> layout_strings_table;
	std::vector<uint8_t> layout_draw_flags_table;

	uint32_t _widget_next_id;
	Frame& _frame_ref;

	static void _initialize();
	static bool _initialized;

	static Program_s default_gui_renderer;
	static unsigned int _default_gui_uniform_screen_position;
	static unsigned int _default_gui_uniform_projection;
	static unsigned int _default_gui_uniform_rect_color;
	static unsigned int _default_gui_uniform_rect_size;
	static unsigned int _default_gui_uniform_corner_rounding;
	static unsigned int _default_gui_uniform_border_color;
	static unsigned int _default_gui_uniform_border_thickness;
	static unsigned int _default_gui_uniform_z_index;

	float _current_z_index = 0;
	float _z_index_buff = 0;

	std::shared_ptr<Font> _font = std::make_shared<Font>("Fonts\\Roboto-Regular.ttf", 16);

	friend Box;
};

class CustomWidget {
public:
	virtual void render() {}
};
*/

// new implementation of gui system
class Gui2 {
public:

	Gui2(Frame& frame);
	void new_frame(Time frame_time);
	_widget_info& box(const std::string& name, vec2 position, vec2 size, Style style, std::u32string text);

	_widget_info& layout(const std::string& name, vec2 position, vec2 min_size, Style style, Layout::LayoutType layout_type = Layout::Vertical);
	void layout_end();

	_widget_info& layout_content(const std::string& name, vec2 min_size, Style style, Layout::LayoutType layout_type = Layout::Vertical);
	void layout_content_end();

	_widget_info& content(const std::string& name, vec2 size, Style style, std::u32string text);

	Style override_style;
	float z_index = 0; 

private:
	friend Frame;
	vec4f get_padding_by_id(unsigned int id, const Style& override_style, const Style& style);
	vec4f get_margin_by_id(unsigned int id, const Style& override_style, const Style& style);
	Style::Stacking get_stacking_type_by_id(unsigned int id, const Style& override_style, const Style& style);

	_widget_info& box(unsigned int id, vec2 position, vec2 size, Style style, std::u32string text, Style override_style, float z_index);

	std::unordered_map<std::string, unsigned int> name_id_table;

	Frame& frame_ref;
	static std::shared_ptr<Program> gui_program;
	static std::shared_ptr<Font> font;
	Camera camera;
	
	Time frame_time;
	std::unordered_map<unsigned int, _widget_info> widget_info_table;
	std::unordered_map<unsigned int, std::unique_ptr<Graphic>> widget_graphic_table;
	std::unordered_map<unsigned int, std::unique_ptr<Text>> widget_text_table;

	// id - z_index
	std::vector<std::pair<unsigned int, float>> hoverings;
	std::vector<Frame::CursorType> hoverings_cursortypes;

	struct layout_info {
		layout_info(unsigned int id, vec2 min_size, Style style, Style override_style, Layout::LayoutType layout_type, float z_index) {
			this->id = id;
			this->min_size = min_size;
			this->style = style;
			this->override_style = override_style;
			this->layout_type = layout_type;
			this->layout.type = layout_type;
			this->z_index = z_index;
		}

		layout_info() { ; }

		unsigned int id;
		vec2 min_size;
		Style style;
		Style override_style;
		Layout::LayoutType layout_type;
		Layout layout;
		float z_index;
	};

	struct content_info {
		content_info(unsigned int id, vec2 size, Style style, Style override_style, std::u32string text, float z_index) {
			this->id = id;
			this->size = size;
			this->style = style;
			this->override_style = override_style;
			this->text = text;
			this->z_index = z_index;
		}

		unsigned int id;
		vec2 size;
		Style style;
		Style override_style;
		std::u32string text;
		float z_index;
	};

	// layouts and contents are hold in a tree
	// layout node definition
	struct layout_node {
		layout_node(unsigned int id, vec2 min_size, Style style, Style override_style, Layout::LayoutType layout_type, float z_index) {
			self_info = layout_info(id, min_size, style, override_style, layout_type, z_index);
		}
		layout_info self_info;
		std::vector<content_info> contents;
		std::vector<std::shared_ptr<layout_node>> childs;
		
		enum child_type {
			content,
			layout,
		};
		// type of childs in creation order, necessery to work out which vector to pop while rendering
		std::vector<child_type> child_type_order;
	};

	vec2 position;
	std::shared_ptr<layout_node> current_layout;

	// keeps track of which layout is pushed through layout_content() call first
	std::vector<std::weak_ptr<layout_node>> layout_stack;

	std::vector<std::shared_ptr<layout_node>> get_layouts_in_ascending_order();
	std::vector<std::shared_ptr<layout_node>> get_layouts_in_descending_order();
};

/*
// css-like layout structure

struct WidgetInfo {
public:
	WidgetInfo(bool was_hovered = false, bool was_active = false, Time hover_duration = 0, Time active_duration = 0, Time last_update = -1) : 
		was_hovered(was_hovered), was_active(was_active), hover_duration(hover_duration), active_duration(active_duration), last_update(last_update) {}

	bool was_hovered;
	bool was_active;
	Time hover_duration;
	Time active_duration;
	Time last_update;
};

class Ui {	// legacy
public:
	Ui(Frame& frame);
	void new_frame();
	void begin(Layout::LayoutType type = Layout::Horizional);
	bool box(const std::string& id, const Vec2<float>& size, const Style& style);
	void end(const Style& style);

private:
	
	Time last_update_ms;
	Time frame_time;
	
	//std::unordered_map<std::string, Time> animation_state;
	std::vector<std::function<void(bool& hover_out, AABB2& aabb_out, StaticStyle& style_to_use_out)>> _position_calculation_calls;
	std::vector<std::function<void(bool hover, AABB2 aabb, StaticStyle style_to_use)>> _render_calls;
	std::string _focused_id;
	std::string _hovered_id;
	std::vector<Layout> layouts;
	glm::mat4 projection_matrix;
	Program program;
	Frame& frame;

	Vec2<int> window_size;

	std::unordered_map<std::string, WidgetInfo> widget_info_table;

	void _update_matrix(int screen_width, int screen_height);
	Frame::CursorState _cursor_state;
	bool _cursor_state_just_changed = false;
	bool _hovered = false;
};
*/