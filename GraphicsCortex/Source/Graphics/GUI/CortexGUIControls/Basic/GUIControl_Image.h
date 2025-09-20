#pragma once
#include "GUI/CortexGUIControl.h"

namespace gui_controls {

	class Image : public GUIControl {
	public:

		enum FitStyle {
			Stretch,
			Contain,
		};

		FitStyle image_fit = Contain;

		void load_image(const std::filesystem::path& image_path) {
			texture = std::make_shared<Texture2D>(image_path.string(), Texture2D::ColorTextureFormat::RGBA8, Texture2D::ColorFormat::RGBA, Texture2D::Type::UNSIGNED_BYTE, 1, 0, 0);
			texture->wait_async_load();
		}
		
		std::shared_ptr<Texture2D> texture = nullptr;

		Widget& get_widget() {
			apply_properties_to_widget(widget);
			update_texture();
			return widget;
		}

	private:

		void update_texture() {
			widget.texture() = texture;
			
			if (image_fit == Stretch) {
				widget.texcoord_min() = glm::vec2(0);
				widget.texcoord_max() = glm::vec2(1);
				return;
			}
			//if (image_fit == Absolute) {
			//	widget.texcoord_min() = glm::vec2(0);
			//	widget.texcoord_max() = glm::vec2(1) * glm::vec2(widget.size()) / glm::vec2(texture->get_size());
			//	return;
			//}
			if (image_fit == Contain) {
				glm::vec2 self_size = widget.size();
				glm::vec2 texture_size = texture->get_size();

				float self_aspect_ratio		= self_size.x / self_size.y;
				float texture_aspect_ratio	= texture_size.x / texture_size.y;

				glm::vec2 clipped_size;

				if (self_aspect_ratio >= texture_aspect_ratio) {
					clipped_size.y = self_size.y;
					clipped_size.x = clipped_size.y * texture_aspect_ratio;
					widget.position().x = (self_size.x - clipped_size.x) / 2;
				}
				else {
					clipped_size.x = self_size.x;
					clipped_size.y = clipped_size.x / texture_aspect_ratio;
					widget.position().y = (self_size.y - clipped_size.y) / 2;
				}
				
				widget.size() = clipped_size;
				widget.texcoord_min() = glm::vec2(0);
				widget.texcoord_max() = glm::vec2(1);
				return;
			}
		}
	};
}
