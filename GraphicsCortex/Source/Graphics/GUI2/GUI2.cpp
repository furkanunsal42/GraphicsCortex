#include "GUI2.h"
#include "PrimitiveRenderer.h"
#include "WindowBoundGlobalResources.h"
#include "GUI2Dynamic.h"
#include "GLMCout.h"

GUI2::MouseEvent GUI2::window_begin(const std::string& idstr, const glm::vec2& initial_position, const glm::vec2& initial_size) {
	
	WindowDesc old_descriptor;
	bool first_call = windows_state.find(idstr) == windows_state.end();
	
	windows_state[idstr].active = true;
	
	if (!first_call)
		old_descriptor = windows_state[idstr].descriptor;
	
	windows_state[idstr].descriptor = WindowDesc();
	
	if (first_call) {
		windows_state[idstr].descriptor.position	= initial_position;
		windows_state[idstr].descriptor.size		= initial_size;
	}
	else {
		windows_state[idstr].descriptor.position	= old_descriptor.position;
		windows_state[idstr].descriptor.size		= old_descriptor.size;
	}

	MouseEvent event = _generate_event_for_aabb(AABB2(windows_state[idstr].descriptor.position, windows_state[idstr].descriptor.position + windows_state[idstr].descriptor.size));
	windows_state[idstr].descriptor.mouse_event = event;
	
	window_stack.push_back(idstr);
	
	return event;
}

GUI2::WindowDesc& GUI2::window_prop() {
	if (window_stack.size() == 0) {
		std::cout << "[GUI Error] window_prop() is called but no window is active" << std::endl;
		ASSERT(false);
	}
	WindowState& state = windows_state.at(window_stack.back());
	return state.descriptor;
}

void GUI2::window_end() {
	window_stack.pop_back();
}

GUI2::MouseEvent GUI2::box_begin(const glm::vec2& position, const glm::vec2& size)
{
	if (box_last != std::nullopt) {
		std::cout << "[GUI Error] GUI2::box_begin() is called within another box definition. nested boxs are not a valid structure" << std::endl;
	}

	box_last = BoxDesc();
	box_last->position = position;
	box_last->size = size;

	auto& window_desc = windows_state[window_stack.back()].descriptor;

	MouseEvent event = _generate_event_for_aabb(AABB2(window_desc.position + position, window_desc.position + position + size));
	box_last->mouse_event = event;

	return event;
}

GUI2::BoxDesc& GUI2::box_prop() {
	if (box_last.has_value() == false) {
		std::cout << "[GUI Error] box_prop() is called but no box was active" << std::endl;
		ASSERT(false);
	}
	return box_last.value();
}

void GUI2::box_end() {
	_publish_last();
}

void GUI2::render() {

	Window* previous_window = active_window;
	glm::vec4 previous_viewport = primitive_renderer::get_viewport_position_size();

	//glm::vec4 previous_viewport = previous_window != nullptr ? 
	//							  primitive_renderer::get_viewport_position_size() : 
	//							  glm::ivec4(0);
	//
	//if (active_window == nullptr) {
	//	WindowDescription desc;
	//	desc.w_name = "CortexGUI2 Parent Window";
	//	desc.w_resolution = glm::ivec2(0);
	//	parent_window = std::make_shared<Window>(desc);
	//	parent_window->context_make_current();
	//}

	for (auto iterator = windows_state.begin(); iterator != windows_state.end(); ) {
		if (iterator->second.active == false)
			iterator = windows_state.erase(iterator);
		else
			++iterator;
	}

	if (parent_window != nullptr)
		parent_window->handle_events();

	bool now_holding_left = false;
	bool now_holding_right = false;

	for (std::pair<const std::string, WindowState>& info : windows_state) {

		const std::string& idstr = info.first;
		WindowState& state = windows_state[idstr];
		WindowDesc& desc = state.descriptor;

		if (state.active == false)
			continue;

		std::function<void()> render_func = [&]() {

			state.window->context_make_current();
			primitive_renderer::clear(desc.color.x, desc.color.y, desc.color.z, desc.color.a);

			RenderParameters params(true);
			params.blend = true;
			params.depth_test = true;
			params.cull_face = true;
			params.cull_face_direction = RenderParameters::CullDirection::FRONT;

			glm::vec2 window_resolution = state.window->get_window_resolution();
			
			Camera camera;
			camera.view_matrix = glm::mat4(1);
			camera.projection_matrix = glm::ortho(0.0f, window_resolution.x, window_resolution.y, 0.0f, -1024.0f, 1024.0f);

			if (state.renderer != nullptr) {
				primitive_renderer::set_viewport_position(glm::vec2(0));
				primitive_renderer::set_viewport_size(window_resolution);
				state.renderer->render_without_clear(camera, params);
			}

			state.window->swap_buffers();

			};

		if (state.window == nullptr) {
			WindowDescription description;
			description.w_name						= desc.name;
			description.w_resolution				= desc.size;
			description.w_resizable					= desc.is_resizable;
			description.w_scale_framebuffer_size	= false;
			description.w_scale_window_size			= false;
			description.w_decorated					= desc.is_decorated;

			//description.context_shared				= previous_window != nullptr	? 
			//										  previous_window->get_handle() :
			//										  parent_window->get_handle();

			state.window = std::make_shared<Window>(description);
			state.window->set_window_position(desc.position);
			state.window->newsletters->on_window_refresh_events.subscribe(render_func);

		}

		state.window->set_window_name(desc.name);
		state.window->set_window_decorated(desc.is_decorated);
		state.window->set_window_resizable(desc.is_resizable);
		if (!desc.is_resizable) {
			state.window->set_window_position(desc.position);
			state.window->set_window_resolution(desc.size);
		}
		
		state.window->handle_events();
		
		if (desc.is_resizable) {
			desc.position = state.window->get_window_position();
			desc.size = state.window->get_window_resolution();
		}
		
		Window::PressAction left	= state.window->get_mouse_button(Window::MouseButton::LEFT);
		Window::PressAction right	= state.window->get_mouse_button(Window::MouseButton::RIGHT);
		now_holding_left			= now_holding_left	|| left  == Window::PressAction::PRESS;
		now_holding_right			= now_holding_right	|| right == Window::PressAction::PRESS;

		render_func();
		if (state.renderer != nullptr)
			state.renderer->clear();
	}

	window_stack.clear();

	if (previous_window != nullptr) {
		
		//Window* effective_parent_window = previous_window != nullptr ? previous_window : parent_window.get();
		Window* effective_parent_window = previous_window;

		io_state.mouse_position =	effective_parent_window->get_cursor_position() + 
									glm::dvec2(effective_parent_window->get_window_position());
		
		//Window::PressAction left	= effective_parent_window->get_mouse_button(Window::MouseButton::LEFT);
		//Window::PressAction right	= effective_parent_window->get_mouse_button(Window::MouseButton::RIGHT);
		//bool now_holding_left		= left  == Window::PressAction::PRESS;
		//bool now_holding_right		= right == Window::PressAction::PRESS;
		
		MouseState mouse_state_prev = io_state.mouse_state;
		bool were_holding_left   = ((int32_t)mouse_state_prev & (int32_t)MouseState::LeftHold)  != 0;
		bool were_holding_right  = ((int32_t)mouse_state_prev & (int32_t)MouseState::RightHold) != 0;
		bool were_pressing_left  = ((int32_t)mouse_state_prev & (int32_t)MouseState::LeftPress)  != 0;
		bool were_pressing_right = ((int32_t)mouse_state_prev & (int32_t)MouseState::RightPress) != 0;
		
		io_state.mouse_state = MouseState::Hover;
		
		if (now_holding_left  &&  were_holding_left)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::LeftHold);
		if (now_holding_left  &&  were_pressing_left)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::LeftHold);
		if (!now_holding_left &&  were_holding_left)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::LeftRelease);
		if (now_holding_left  && !were_holding_left)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::LeftPress);
		
		if (now_holding_right  &&  were_holding_right)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::RightHold);
		if (now_holding_right  &&  were_pressing_right)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::RightHold);
		if (!now_holding_right &&  were_holding_right)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::RightRelease);
		if (now_holding_right  && !were_holding_right)	io_state.mouse_state = MouseState((int32_t)io_state.mouse_state | (int32_t)MouseState::RightPress);

		if ((int32_t)io_state.mouse_state & (int32_t)MouseState::LeftPress)  io_state.mouse_left_press_begin_position  = io_state.mouse_position;
		if ((int32_t)io_state.mouse_state & (int32_t)MouseState::RightPress) io_state.mouse_right_press_begin_position = io_state.mouse_position;

		if (previous_window != nullptr) {
			previous_window->context_make_current();
			primitive_renderer::set_viewport(previous_viewport);
		}
		//if (parent_window != nullptr)
		//	Window::detech_context();
	}
}

const GUI2::IOState& GUI2::get_io_state()
{
	return io_state;
}

GUI2::MouseEvent GUI2::_generate_event_for_aabb(const AABB2& aabb) {
	
	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::LeftHold) != 0) return MouseEvent::LeftHold;
	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::LeftPress) != 0) return MouseEvent::LeftPress;
	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::LeftRelease) != 0) return MouseEvent::LeftRelease;

	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::RightHold) != 0) return MouseEvent::RightHold;
	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::RightPress) != 0) return MouseEvent::RightPress;
	if (aabb.does_contain(io_state.mouse_position) && ((int32_t)io_state.mouse_state & (int32_t)MouseState::RightRelease) != 0) return MouseEvent::RightRelease;

	if (aabb.does_contain(io_state.mouse_position)) return MouseEvent::Hover;

	return None;
}

void GUI2::_publish_last() {

	if (window_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2::_publish_last() is called but no window is active" << std::endl;
		ASSERT(false);
	}

	if (box_last.has_value()) {

		BoxDesc& desc = box_last.value();

		if (windows_state.at(window_stack.back()).renderer == nullptr)
			windows_state.at(window_stack.back()).renderer = std::make_shared<ImmediateRenderer>();

		ImmediateRenderer& renderer = *windows_state.at(window_stack.back()).renderer;

		renderer.set_fill_color(desc.color);
		renderer.draw_rectangle(
			desc.position,
			desc.position + desc.size
		);

		box_last = std::nullopt;
	}
}