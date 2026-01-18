#pragma once
#include "GUIWidgets.h"

namespace widget2 {

	template<>
	inline void DefaultStyle::apply<Window>(Window& widget) {
		
		widget.padding			= glm::vec4(10);
		widget.target_size		= glm::vec2(GUIDynamic::fit);
		widget.min_size			= glm::vec2(GUIDynamic::fit);
		widget.max_size			= glm::vec2(10240);
		widget.color			= glm::vec4(0.98f, 0.98f, 0.98f, 1);
		widget.border_thickness	= glm::vec4(0);
		widget.border_rounding	= glm::vec4(0);
		widget.border_color0	= glm::vec4(1);
		widget.border_color1	= glm::vec4(1);
		widget.border_color2	= glm::vec4(1);
		widget.border_color3	= glm::vec4(1);
		widget.shadow_thickness	= glm::vec4(0);
		widget.shadow_color		= glm::vec4(1);

	}

	template<>
	inline void DefaultStyle::apply<Box>(Box& widget) {
		widget.margin			= glm::vec4(0);
		widget.target_size		= glm::vec2(128);	
		widget.min_size			= glm::vec2(GUIDynamic::fit);	
		widget.max_size			= glm::vec2(10240);	
		widget.color			= glm::vec4(1, 1, 1, 1);
		widget.border_thickness	= glm::vec4(0);			
		widget.border_rounding	= glm::vec4(0);		
		widget.border_color0	= glm::vec4(0, 0, 0, 1);		
		widget.border_color1	= glm::vec4(0, 0, 0, 1);		
		widget.border_color2	= glm::vec4(0, 0, 0, 1);		
		widget.border_color3	= glm::vec4(0, 0, 0, 1);		
		widget.shadow_thickness	= glm::vec4(0);			
		widget.shadow_color		= glm::vec4(0, 0, 0, 1);	
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<Grid>(Grid& widget) {
		widget.margin		= glm::vec4(0);	
		widget.padding		= glm::vec4(0);
		widget.target_size	= glm::vec2(GUIDynamic::fit);
		widget.min_size		= glm::vec2(GUIDynamic::fit);	
		widget.max_size		= glm::vec2(10240);
		widget.pass_through_events = true;
	}

	template<>
	inline void DefaultStyle::apply<Stack>(Stack& widget) {
		widget.margin		= glm::vec4(0);
		widget.padding		= glm::vec4(0);
		widget.target_size	= glm::vec2(GUIDynamic::fit);
		widget.min_size		= glm::vec2(GUIDynamic::fit);	
		widget.max_size		= glm::vec2(10240);
		widget.spacing		= 10;
		widget.is_vertical	= true;
		widget.pass_through_events = true;
	}

	template<>
	inline void DefaultStyle::apply<Image>(Image& widget) {
		apply<Box>(widget);
		widget.uv00			= glm::vec2(0);
		widget.uv11			= glm::vec2(1);
		widget.type			= Image::Fit;

		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<Label>(Label& widget) {
		apply<Grid>(widget);
		widget.font			= 1;
		widget.text_height	= 12;
		widget.text_color	= glm::vec4(0, 0, 0, 1);
		widget.target_size	= glm::vec2(GUIDynamic::fit);

		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<TextInput>(TextInput& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);

		widget.placeholder_text_color			= glm::vec4(0.3, 0.3, 0.3, 1);
		widget.selected_text_color				= glm::vec4(1, 1, 1, 1);
		widget.selected_background_color		= glm::vec4(0.23, 0.48, 0.72, 1);
		widget.text_color						= glm::vec4(0.2, 0.2, 0.2, 1);
		widget.text_cursor_color				= glm::vec4(0, 0, 0, 1);
		widget.text_cursor_timer_blink_period	= std::chrono::milliseconds(500);

		widget.target_size						= glm::vec2(200, 26);
		widget.padding							= glm::vec4(0);

		widget.label.target_size				= glm::vec2(GUIDynamic::fit);
		widget.label.margin						= glm::vec4(4, GUIDynamic::avail, 4, GUIDynamic::avail);
		widget.label.text_color					= glm::vec4(0.2, 0.2, 0.2, 1);

		widget.background.color					= glm::vec4(1, 1, 1, 1);
		widget.background.border_thickness 		= glm::vec4(2);
		widget.background.border_color0 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color1 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color2 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color3 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.target_size 			= glm::vec2(GUIDynamic::avail, GUIDynamic::avail);
		widget.background.margin 				= glm::vec4(0);

		widget.label.text_color.transition(widget.focus, glm::vec4(0, 0, 0, 1));
		widget.background.border_color0.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color1.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color2.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color3.transition(widget.focus, glm::vec4(0.50, 0.50, 0.56, 1));
		
		widget.background.pass_through_events = true;
		widget.label.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<Slider>(Slider& widget) {
		apply<Grid>(widget);
		apply(widget.head);
		apply(widget.background);
		apply(widget.filled_bar);

		widget.target_size						= glm::vec2(200, 16);

		widget.head.target_size					= glm::vec2(widget.target_size.value.y, widget.target_size.value.y);
		widget.head.color 						= glm::vec4(0.92, 0.92, 0.92, 1);
		widget.head.border_rounding 			= glm::vec4(11);
		widget.head.color.transition(widget.hover, glm::vec4(0.94, 0.94, 0.94, 1), std::chrono::milliseconds(100));
		widget.head.color.transition(widget.hold, glm::vec4(0.82, 0.82, 0.82, 1), std::chrono::milliseconds(100));

		widget.background.target_size			= glm::vec2(GUIDynamic::avail, 4);
		widget.background.color 				= glm::vec4(0.90, 0.90, 0.90, 1);
		widget.background.margin 				= glm::vec4(0, GUIDynamic::avail, 0, GUIDynamic::avail);

		widget.filled_bar.target_size			= glm::vec2(GUIDynamic::avail, 4);
		widget.filled_bar.color					= glm::vec4(0.0, 0.47, 0.84, 1);
		widget.filled_bar.margin				= glm::vec4(0, GUIDynamic::avail, 0, GUIDynamic::avail);

		widget.background.pass_through_events = true;
		widget.filled_bar.pass_through_events = true;
		widget.head.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<DragFloat>(DragFloat& widget) {
		apply<TextInput>(widget);

		widget.target_size = glm::vec2(80, 26);

		widget.background.border_color0.transition(widget.hold, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color1.transition(widget.hold, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color2.transition(widget.hold, glm::vec4(0.50, 0.50, 0.56, 1));
		widget.background.border_color3.transition(widget.hold, glm::vec4(0.50, 0.50, 0.56, 1));

		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<Button>(Button& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);

		widget.target_size					= glm::vec2(100, 26);
		//widget.padding						= glm::vec4(43, 13, 43, 13);
		//widget.target_size					= glm::vec2(GUIDynamic::fit);

		widget.background.target_size		= glm::vec2(GUIDynamic::avail);
		widget.background.color				= glm::vec4(0.93, 0.93, 0.93, 1);
		widget.background.border_thickness 	= glm::vec4(1);
		widget.background.border_color0 	= glm::vec4(0.0, 0.47, 0.84, 1);
		widget.background.border_color1 	= glm::vec4(0.0, 0.47, 0.84, 1);
		widget.background.border_color2 	= glm::vec4(0.0, 0.47, 0.84, 1);
		widget.background.border_color3 	= glm::vec4(0.0, 0.47, 0.84, 1);

		widget.label.margin					= glm::vec4(GUIDynamic::avail);
		widget.label.text_color				= glm::vec4(0.2, 0.2, 0.2, 1);

		widget.background.border_color0.	transition(widget.hover, glm::vec4(0.0, 0.47, 0.84, 1), std::chrono::milliseconds(250));
		widget.background.border_color1.	transition(widget.hover, glm::vec4(0.0, 0.47, 0.84, 1), std::chrono::milliseconds(250));
		widget.background.border_color2.	transition(widget.hover, glm::vec4(0.0, 0.47, 0.84, 1), std::chrono::milliseconds(250));
		widget.background.border_color3.	transition(widget.hover, glm::vec4(0.0, 0.47, 0.84, 1), std::chrono::milliseconds(250));
		widget.background.color.			transition(widget.hover, glm::vec4(0.9, 0.95, 0.98, 1), std::chrono::milliseconds(250));
		widget.background.border_thickness.	transition(widget.hover, glm::vec4(1), std::chrono::milliseconds(250));

		widget.background.color.transition(widget.hold, glm::vec4(0.8, 0.89, 0.97, 1), std::chrono::milliseconds(50));
		
		widget.background.pass_through_events = true;
		widget.label.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<ImageButton>(ImageButton& widget) {
		apply<Grid>(widget);
		apply(widget.image);
		apply(widget.background);

		widget.target_size						= glm::vec2(26);
		
		widget.image.margin						= glm::vec4(2);
		widget.image.target_size				= glm::vec2(GUIDynamic::avail);
		widget.image.type						= Image::Fit;
		
		widget.background.target_size			= glm::vec2(GUIDynamic::avail);
		widget.background.color					= glm::vec4(0, 0, 0, 0);
		widget.background.border_color0			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color1			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color2			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color3			= glm::vec4(0.45f, 0.44f, 0.42f, 1);

		widget.background.border_thickness		= glm::vec4(0);
		widget.background.border_thickness.transition(widget.hover, glm::vec4(1), std::chrono::milliseconds(15));
		widget.background.border_thickness.transition(widget.hold, glm::vec4(2), std::chrono::milliseconds(80));

		widget.background.border_color0.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color1.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color2.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color3.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		
		widget.background.pass_through_events = true;
		widget.image.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<CheckBox>(CheckBox& widget) {
		apply<Grid>(widget);
		apply(widget.image);
		apply(widget.background);

		widget.target_size						= glm::vec2(26);
		
		widget.image.margin						= glm::vec4(4);
		widget.image.target_size				= glm::vec2(GUIDynamic::avail);
		widget.image.type						= Image::Fit;
		widget.image.color						= glm::vec4(1, 1, 1, 0);

		widget.background.target_size			= glm::vec2(GUIDynamic::avail);
		widget.background.color					= glm::vec4(0, 0, 0, 0);
		widget.background.border_color0			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color1			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color2			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_color3			= glm::vec4(0.45f, 0.44f, 0.42f, 1);
		widget.background.border_thickness		= glm::vec4(1);

		widget.background.border_thickness.	transition(widget.hover, glm::vec4(1), std::chrono::milliseconds(15));
		widget.background.border_thickness.	transition(widget.hold, glm::vec4(2), std::chrono::milliseconds(80));
		widget.background.color.			transition(widget.check, glm::vec4(0.0, 0.47, 0.84, 1), std::chrono::milliseconds(120));
		widget.image.color.					transition(widget.check, glm::vec4(1, 1, 1, 1), std::chrono::milliseconds(120));


		widget.background.border_color0.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color1.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color2.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));
		widget.background.border_color3.transition(widget.hold, glm::vec4(0.45f, 0.59f, 0.65f, 1), std::chrono::milliseconds(40));

		widget.background.pass_through_events = true;
		widget.image.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<ComboBox>(ComboBox& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);
		apply(widget.dropdown);
		apply(widget.dropdown_stack);

		widget.target_size						= glm::vec2(200, 26);
		widget.padding							= glm::vec4(0);

		widget.label.target_size				= glm::vec2(GUIDynamic::fit);
		widget.label.margin						= glm::vec4(8, GUIDynamic::avail, 8, GUIDynamic::avail);
		widget.label.text_color					= glm::vec4(0.2, 0.2, 0.2, 1);

		widget.background.color					= glm::vec4(0.95, 0.95, 0.95, 1);
		widget.background.border_thickness 		= glm::vec4(2);
		widget.background.border_color0 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color1 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color2 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.border_color3 		= glm::vec4(0.68, 0.71, 0.75, 1);
		widget.background.target_size 			= glm::vec2(GUIDynamic::avail, GUIDynamic::avail);
		widget.background.margin 				= glm::vec4(0);

		widget.background.color.transition(widget.hover, glm::vec4(0.81f, 0.85f, 0.9f, 1), std::chrono::milliseconds(120));
		widget.background.border_color0.transition(widget.hover, glm::vec4(0.68, 0.71, 0.75, 1));
		widget.background.border_color1.transition(widget.hover, glm::vec4(0.68, 0.71, 0.75, 1));
		widget.background.border_color2.transition(widget.hover, glm::vec4(0.68, 0.71, 0.75, 1));
		widget.background.border_color3.transition(widget.hover, glm::vec4(0.68, 0.71, 0.75, 1));

		widget.background.color.transition(widget.hold, glm::vec4(0.76f, 0.8f, 0.85f, 1), std::chrono::milliseconds(60));

		widget.dropdown.color					= glm::vec4(1, 1, 1, 1);
		widget.dropdown.padding					= glm::vec4(0);

		widget.dropdown_stack.target_size		= glm::vec2(GUIDynamic::fit);
		widget.dropdown_stack.spacing			= 0;

		widget.background.pass_through_events = true;
		widget.label.pass_through_events = true;
		widget.dropdown_stack.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<ComboBoxItem>(ComboBoxItem& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);

		widget.target_size		= glm::vec2(200, 26);
		
		widget.background.target_size = glm::vec2(GUIDynamic::avail);
		widget.background.color = glm::vec4(0.96, 0.96, 0.96, 1);
		widget.background.color.transition(widget.hover, glm::vec4(0.88, 0.88, 0.88, 1));

		widget.background.border_thickness = glm::vec4(0);
		widget.background.border_thickness.transition(widget.hover, glm::vec4(1));

		widget.background.border_color0 = glm::vec4(0.34, 0.34, 0.34, 1);
		widget.background.border_color1 = glm::vec4(0.34, 0.34, 0.34, 1);
		widget.background.border_color2 = glm::vec4(0.34, 0.34, 0.34, 1);
		widget.background.border_color3 = glm::vec4(0.34, 0.34, 0.34, 1);

		widget.label.margin = glm::vec4(8, GUIDynamic::avail, GUIDynamic::avail, GUIDynamic::avail);

		widget.label.pass_through_events = true;
		widget.background.pass_through_events = true;
		widget.pass_through_events = true;
	}

	template<>
	inline void DefaultStyle::apply<Menu>(Menu& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.label);

		widget.target_size				= glm::vec2(60, 26);
		widget.background.target_size	= glm::vec2(GUIDynamic::avail);
		widget.label.margin				= glm::vec4(GUIDynamic::avail);

		//widget.background.border_thickness	= glm::vec4(0);
		//widget.background.border_color0		= glm::vec4(0.46, 0.46, 0.46, 1);
		//widget.background.border_color1		= glm::vec4(0.46, 0.46, 0.46, 1);
		//widget.background.border_color2		= glm::vec4(0.46, 0.46, 0.46, 1);
		//widget.background.border_color3		= glm::vec4(0.46, 0.46, 0.46, 1);

		widget.background.color.transition(widget.hover, glm::vec4(0.81f, 0.85f, 0.9f, 1));

		widget.background.pass_through_events = true;
		widget.label.pass_through_events = true;
		widget.pass_through_events = false;
	}

	template<>
	inline void DefaultStyle::apply<WindowControls>(WindowControls& widget) {
		apply<Stack>(widget);
		apply(widget.minimize_button);
		apply(widget.restore_button);
		apply(widget.close_button);

		widget.target_size				= glm::vec2(GUIDynamic::fit);
		widget.is_vertical				= false;
		widget.spacing					= 0;
		widget.padding					= glm::vec4(0);

		widget.minimize_button.target_size = glm::vec2(40, 24);
		widget.restore_button.target_size = glm::vec2(40, 24);
		widget.close_button.target_size = glm::vec2(40, 24);

		widget.minimize_button.background.color = glm::vec4(1, 1, 1, 1);
		widget.minimize_button.background.color.transition(widget.minimize_button.hover, glm::vec4(0.8, 0.8, 0.8, 1));
		widget.minimize_button.background.color.transition(widget.minimize_button.hold, glm::vec4(0.75, 0.75, 0.75, 1));

		widget.restore_button.background.color = glm::vec4(1, 1, 1, 1);
		widget.restore_button.background.color.transition(widget.restore_button.hover, glm::vec4(0.8, 0.8, 0.8, 1));
		widget.restore_button.background.color.transition(widget.restore_button.hold, glm::vec4(0.75, 0.75, 0.75, 1));

		widget.close_button.background.color = glm::vec4(1, 1, 1, 1);
		widget.close_button.background.color.transition(widget.close_button.hover, glm::vec4(0.91f, 0.05f, 0.11f, 1));
		widget.close_button.background.color.transition(widget.close_button.hold, glm::vec4(0.91f, 0.05f, 0.11f, 1));
	}

	template<>
	inline void DefaultStyle::apply<MenuBar>(MenuBar& widget) {
		apply<Grid>(widget);
		apply(widget.background);
		apply(widget.manu_stack);
		apply(widget.window_controls);

		widget.target_size					= glm::vec2(GUIDynamic::avail, 26);
		widget.background.target_size		= glm::vec2(GUIDynamic::avail);
	
		widget.background.color				= glm::vec4(0.90, 0.90, 0.90, 1);

		widget.background.pass_through_events = true;
		widget.manu_stack.pass_through_events = true;
		widget.window_controls.pass_through_events = false;
		widget.pass_through_events = false;
	}

}