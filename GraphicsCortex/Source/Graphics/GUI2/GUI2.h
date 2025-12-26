#pragma once

#include "glm.hpp"
#include <vector>
#include <string>
#include <unordered_map>

#include "Window.h"
#include "Tools/ImmediateRendering/ImmediateRenderer.h"
#include "Math/AABB.h"

class GUI2 {
public:

	struct	WindowDesc;
	struct	BoxDesc;
	enum	MouseEvent;
	struct	IOState;

	MouseEvent		window_begin(const std::string& idstr, const glm::vec2& initial_position, const glm::vec2& initial_size);
	WindowDesc&		window_prop();
	void			window_end();

	MouseEvent		box_begin(const glm::vec2& position, const glm::vec2& size);
	BoxDesc&		box_prop();
	void			box_end();

	void			render();

	const IOState&	get_io_state();

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

		glm::vec2 uv00				= glm::vec2(0);
		glm::vec2 uv11				= glm::vec2(1);
		uint32_t texture_id			= -1;
	};

	struct WindowDesc {

		MouseEvent mouse_event		= MouseEvent::None;
		glm::vec2 position			= glm::vec2(0);
		glm::vec2 size				= glm::vec2(128);

		std::string name			= "CortexGUI2 Window";
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

	enum class MouseState {
		Hover			= 0b00000000,
		LeftPress		= 0b00000001,
		LeftRelease		= 0b00000010,
		LeftHold		= 0b00000100,
		RightPress		= 0b00001000,
		RightRelease	= 0b00010000,
		RightHold		= 0b00100000,
	};

	struct IOState {
		MouseState mouse_state						= MouseState::Hover;
		MouseState mouse_state_prev					= MouseState::Hover;
		glm::vec2 mouse_position					= glm::vec2(0);
		glm::vec2 mouse_left_press_begin_position	= glm::vec2(0);
		glm::vec2 mouse_right_press_begin_position	= glm::vec2(0);
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
	std::shared_ptr<Window> parent_window = nullptr;
	std::vector<std::string> window_stack;
	std::unordered_map<std::string, WindowState> windows_state;
	std::optional<BoxDesc> box_last = std::nullopt;
};