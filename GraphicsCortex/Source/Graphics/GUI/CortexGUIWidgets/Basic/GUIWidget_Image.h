#pragma once
#include "GUI/CortexGUI.h"

namespace widget {

	class Image : public Widget {
	public:

		Image(){
			color = glm::vec4(1, 1, 1, 1);
		}

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

		Element& get_element(glm::vec2 allocated_size) {
			apply_properties_to_element(element, allocated_size);
			update_texture();
			return element;
		}

	private:

		void update_texture() {
			element.texture() = texture;
			
			if (image_fit == Stretch) {
				element.texcoord_min() = glm::vec2(0);
				element.texcoord_max() = glm::vec2(1);
				return;
			}
			if (image_fit == Contain) {
				glm::vec2 self_size = element.size();
				glm::vec2 texture_size = texture->get_size();

				float self_aspect_ratio		= self_size.x / self_size.y;
				float texture_aspect_ratio	= texture_size.x / texture_size.y;

				glm::vec2 clipped_size;

				if (self_aspect_ratio >= texture_aspect_ratio) {
					clipped_size.y = self_size.y;
					clipped_size.x = clipped_size.y * texture_aspect_ratio;
					element.position().x = (self_size.x - clipped_size.x) / 2;
				}
				else {
					clipped_size.x = self_size.x;
					clipped_size.y = clipped_size.x / texture_aspect_ratio;
					element.position().y = (self_size.y - clipped_size.y) / 2;
				}
				
				element.size() = clipped_size;
				element.texcoord_min() = glm::vec2(0);
				element.texcoord_max() = glm::vec2(1);
				return;
			}
		}
	};
}
