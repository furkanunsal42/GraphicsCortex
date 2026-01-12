#pragma once

#include "glm.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

#include "Window.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"
#include "Math/AABB.h"
#include "Newsletter.h"

class GUI {
public:

	struct	WindowDesc;
	struct	BoxDesc;
	enum	MouseEvent;
	struct	IOState;

	~GUI();
	GUI();

	MouseEvent		window_begin(const std::string& idstr, const glm::vec2& initial_position, const glm::vec2& initial_size);
	WindowDesc&		window_prop();
	void			window_end();

	MouseEvent		box_begin(const glm::vec2& position, const glm::vec2& size);
	BoxDesc&		box_prop();
	void			box_end();

	void			render();

	const IOState&	get_io_state();

	Window*			get_parent_window();

	struct BoxDesc {

		MouseEvent mouse_event		= None;
		glm::vec2 position			= glm::vec2(0);
		glm::vec2 size				= glm::vec2(128);

		glm::vec4 color				= glm::vec4(1, 1, 1, 1);
		glm::vec4 border_thickness	= glm::vec4(0);
		glm::vec4 border_rounding	= glm::vec4(0);
		glm::vec4 border_color0		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color1		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color2		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color3		= glm::vec4(0, 0, 0, 1);
		glm::vec4 shadow_thickness	= glm::vec4(0);
		glm::vec4 shadow_color		= glm::vec4(0, 0, 0, 1);

		float	  z					= 0;
		glm::vec2 uv00				= glm::vec2(0);
		glm::vec2 uv11				= glm::vec2(1);
		uint64_t texture_handle		= 0;

	};

	struct WindowDesc {

		MouseEvent mouse_event		= MouseEvent::None;
		glm::vec2 position			= glm::vec2(0);
		glm::vec2 size				= glm::vec2(128);

		std::string name			= "CortexGUI Window";
		glm::vec4 color				= glm::vec4(1, 1, 1, 1);
		glm::vec4 border_thickness	= glm::vec4(0);
		glm::vec4 border_rounding	= glm::vec4(0);
		glm::vec4 border_color0		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color1		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color2		= glm::vec4(0, 0, 0, 1);
		glm::vec4 border_color3		= glm::vec4(0, 0, 0, 1);
		glm::vec4 shadow_thickness	= glm::vec4(0);
		glm::vec4 shadow_color		= glm::vec4(0, 0, 0, 1);
		
		bool is_decorated			= false;
		bool is_resizable			= true;
	};

	enum MouseEvent {
		None			= 0b00000001,
		Hover			= 0b00000010,
		LeftPress		= 0b00000100,
		LeftRelease		= 0b00001000,
		LeftHold		= 0b00010000,
		RightPress		= 0b00100000,
		RightRelease	= 0b01000000,
		RightHold		= 0b10000000,
	};

	struct KeyboardEvent {
		KeyboardEvent() = default;
		KeyboardEvent(uint32_t key) { data = key; }
		KeyboardEvent(Window::KeyPressResult key_press_result) { data = key_press_result; }

		std::variant<
			uint32_t,
			Window::KeyPressResult
		> data = 0u;
	};

	struct IOState {
		MouseEvent	mouse_state						= MouseEvent::Hover;
		MouseEvent	mouse_state_prev				= MouseEvent::Hover;
		glm::vec2	mouse_position					= glm::vec2(0);
		glm::vec2	mouse_left_press_begin_position	= glm::vec2(0);
		glm::vec2	mouse_right_press_begin_position= glm::vec2(0);
		std::vector<KeyboardEvent> keyboard_events;
		size_t char_newsletter_event	= Newsletter<void()>::invalid_id;
		size_t key_newsletter_event		= Newsletter<void()>::invalid_id;
	};

private:

	MouseEvent _generate_event_for_aabb(const AABB2& aabb);
	void _publish_last();
	
	struct WindowState {
		bool active = false;
		WindowDesc descriptor;
		std::shared_ptr<Window> window = nullptr;
		std::shared_ptr<ImmediateRenderer> renderer = nullptr;
	};

	IOState io_state;
	Window* parent_window = nullptr;
	bool owning_parent_window = false;
	std::vector<std::string> window_stack;
	std::unordered_map<std::string, WindowState> windows_state;
	std::optional<BoxDesc> box_last = std::nullopt;
};