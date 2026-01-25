#pragma once
#include "GUIWidgets.h"

template<typename T>
widget::StyleProperty<T>::StyleProperty<T>(T value) : 
	value(value) { }

template<typename T>
widget::StyleProperty<T>::operator T() 
{ 
	return value; 
}
template<typename T>
widget::StyleProperty<T>& widget::StyleProperty<T>::transition(T default_value) {
	value = value;
	return *this;
}


template<typename T>
widget::StyleProperty<T>& widget::StyleProperty<T>::transition(const IOEvent& event, T transition_value, duration transition_duration) {
	float t = glm::clamp(get_t(event, transition_duration), 0.0f, 1.0f);
	value = value * (1 - t) + transition_value * t;
	return *this;
}

template<typename T>
float widget::StyleProperty<T>::get_t(const IOEvent& event, duration transition_duration) {

	if (event.begin_time == GUIDynamic::invalid_time && event.end_time == GUIDynamic::invalid_time)
		return 0;

	if (transition_duration.count() == 0)
		return event.end_time > event.begin_time ? 1 : 0;

	auto now = std::chrono::system_clock::now();

	duration time_passed = std::min(now - event.begin_time, transition_duration);
	duration time_passed_hovering = std::min(event.end_time - event.begin_time, transition_duration);
	duration time_passed_recovering = std::min(now - event.end_time, transition_duration);

	bool happening =
		(event.begin_time != GUIDynamic::invalid_time && event.end_time == GUIDynamic::invalid_time) ||
		(event.begin_time != GUIDynamic::invalid_time && event.end_time != GUIDynamic::invalid_time && event.begin_time > event.end_time);

	bool recovering =
		event.begin_time != GUIDynamic::invalid_time &&
		event.end_time != GUIDynamic::invalid_time &&
		event.begin_time < event.end_time &&
		now - event.end_time < time_passed_hovering;

	duration relevent_duration = happening ? time_passed : time_passed_hovering - time_passed_recovering;

	return 1 - (transition_duration - relevent_duration).count() / (float)transition_duration.count();

}