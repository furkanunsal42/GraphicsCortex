#pragma once
#include "Config.h"

#include <string>
#include <chrono>
#include "CortexMath.h"

extern bool is_glew_initialized;
extern bool is_glfw_initialized;
extern int fps_counter_batch;
extern double frame_time_ms;
extern double frame_rate_fps;
extern std::chrono::time_point<std::chrono::system_clock> frame_begin_time;

struct GLFWwindow;

class Frame {
public:
	enum CallbackLevel {
		NOTIFICATION = 4, 
		LOW			 = 3,
		MEDIUM		 = 2,
		HIGH		 = 1,
		DISABLED     = 0,
	};

	int multisample;
	int window_width;
	int window_height;
	std::string window_name;
	int swapinterval;
	bool depth_test, blend, face_culling, initialize_imgui;
	GLFWwindow* window;

	Frame() = delete;
	Frame(const Frame& other) = delete;
	Frame(int width, int height, const std::string& name = "GraphicsCortex", int msaa = 0, int swapinterval = 1, bool depth_test = true, bool blend = false, bool face_culling = true, CallbackLevel min_debug_callback_level = CallbackLevel::LOW, bool initialize_gui = true);
	~Frame();

	//GLFWwindow* create_window(int width, int height, std::string name, int msaa = 0, int swapinterval = 1, bool depth_test = true, bool blend = false, bool face_culling = true);
	void clear_window(float red = 0, float green = 0, float blue = 0, float alpha = 1);
	void display_performance(int batch_size = NULL);
	void close_frame();
	double get_interval_ms();
	bool is_running();
	double handle_window();
	void set_viewport(int width, int height);
	void set_viewport(glm::vec2 size);

	void resize(int width, int height);
	void set_visibility(bool value);
	enum CursorType {
		Arrow = 0,
		IBeam,
		Crosshair,
		Hand,
		HResize,
		VResize,
		Disabled,
	};
	
	enum CursorState {
		Hover = 0,
		LeftPressed = 1,
		LeftReleased = 2,
		RightPressed = 4,
		RightReleased = 8,
	};

	Vec2<int> get_cursor_position();
	void set_cursor_type(Frame::CursorType cursor_type = Frame::CursorType::Arrow);
	CursorType get_cursor_type();
	bool get_mouse_state(Frame::CursorState state);
	double get_time_sec();

	enum Key {
		SPACE              = 32	,
		APOSTROPHE         = 39 , /* ' */
		COMMA              = 44 , /* , */
		MINUS              = 45 , /* - */
		PERIOD             = 46 , /* . */
		SLASH              = 47 , /* / */
		NUM0               = 48	,
		NUM1               = 49	,
		NUM2               = 50	,
		NUM3               = 51	,
		NUM4               = 52	,
		NUM5               = 53	,
		NUM6               = 54	,
		NUM7               = 55	,
		NUM8               = 56	,
		NUM9			   = 57	,
		SEMICOLON          = 59 , /* ; */
		EQUAL              = 61 , /* = */
		A                  = 65	,
		B                  = 66	,
		C                  = 67	,
		D                  = 68	,
		E                  = 69	,
		F                  = 70	,
		G                  = 71	,
		H                  = 72	,
		I                  = 73	,
		J                  = 74	,
		K                  = 75	,
		L                  = 76	,
		M                  = 77	,
		N                  = 78	,
		O                  = 79	,
		P                  = 80	,
		Q                  = 81	,
		R                  = 82	,
		S                  = 83	,
		T                  = 84	,
		U                  = 85	,
		V                  = 86	,
		W                  = 87	,
		X                  = 88	,
		Y                  = 89	,
		Z                  = 90	,
		LEFT_BRACKET       = 91 , /* [ */
		BACKSLASH          = 92 , /* \ */
		RIGHT_BRACKET      = 93 , /* ] */
		GRAVE_ACCENT       = 96 , /* ` */
		WORLD_1            = 161, /* non-US #1 */
		WORLD_2            = 162, /* non-US #2 */

		ESCAPE             = 256,
		ENTER              = 257,
		TAB                = 258,
		BACKSPACE          = 259,
		INSERT             = 260,
		DELETE             = 261,
		RIGHT              = 262,
		LEFT               = 263,
		DOWN               = 264,
		UP                 = 265,
		PAGE_UP            = 266,
		PAGE_DOWN          = 267,
		HOME               = 268,
		END                = 269,
		CAPS_LOCK          = 280,
		SCROLL_LOCK        = 281,
		NUM_LOCK           = 282,
		PRINT_SCREEN       = 283,
		PAUSE              = 284,
		F1                 = 290,
		F2                 = 291,
		F3                 = 292,
		F4                 = 293,
		F5                 = 294,
		F6                 = 295,
		F7                 = 296,
		F8                 = 297,
		F9                 = 298,
		F10                = 299,
		F11                = 300,
		F12                = 301,
		F13                = 302,
		F14                = 303,
		F15                = 304,
		F16                = 305,
		F17                = 306,
		F18                = 307,
		F19                = 308,
		F20                = 309,
		F21                = 310,
		F22                = 311,
		F23                = 312,
		F24                = 313,
		F25                = 314,
		KP_0               = 320,
		KP_1               = 321,
		KP_2               = 322,
		KP_3               = 323,
		KP_4               = 324,
		KP_5               = 325,
		KP_6               = 326,
		KP_7               = 327,
		KP_8               = 328,
		KP_9               = 329,
		KP_DECIMAL         = 330,
		KP_DIVIDE          = 331,
		KP_MULTIPLY        = 332,
		KP_SUBTRACT        = 333,
		KP_ADD             = 334,
		KP_ENTER           = 335,
		KP_EQUAL           = 336,
		LEFT_SHIFT         = 340,
		LEFT_CONTROL       = 341,
		LEFT_ALT           = 342,
		LEFT_SUPER         = 343,
		RIGHT_SHIFT        = 344,
		RIGHT_CONTROL      = 345,
		RIGHT_ALT          = 346,
		RIGHT_SUPER        = 347,
		MENU               = 348,
	};

	bool get_key_press(Frame::Key key);
	
	double get_scroll_position_x();
	double get_scroll_position_y();

	double scroll_position_x = 0;
	double scroll_position_y = 0;

private:
	bool visible = true;
	CursorState _cursor_state;
	CursorType _current_cursor_type;
	void _window_resize_callback(GLFWwindow* window, int width, int height);
};