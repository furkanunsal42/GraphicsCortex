#pragma once
#include "GUI2Widgets.h"

template<typename T>
inline T widget2::StyledWidget::interpolate(T a, T b, float t) {
	t = glm::clamp(t, 0.0f, 1.0f);
	return a * (1 - t) + b * t;
}

template<typename T>
inline T widget2::StyledWidget::get_property(
	T					default_property,
	std::optional<T>	on_hover,
	duration			hover_transition_time,
	std::optional<T>	on_hold,
	duration			hold_transition_time
) {
	T property			= interpolate(default_property, on_hover.value_or(default_property), get_t(hover.begin, hover.end, hover_transition_time));
	if (on_hold.has_value())
		property		= interpolate(property, on_hold.value_or(default_property), get_t(hold.begin, hold.end, hold_transition_time));
	return property;
}
