#pragma once
#include "GUI/CortexGUIControl.h"

namespace gui_controls {

	class Image : public GUIControl {
	public:

		enum FitStyle {
			Absolute,
			Scretch,
			Contain,
		};

		FitStyle image_fit = Contain;

		std::shared_ptr<Texture2D> texture = nullptr;

		Widget& get_widget() {
			apply_properties_to_widget(widget);
			widget.texture() = texture;
			return widget;
		}

	private:
	};
}
