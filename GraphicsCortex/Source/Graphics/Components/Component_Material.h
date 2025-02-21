#pragma once
#include "Component/Component.h"

#include <unordered_map>
#include <string>

#include "Texture2D.h"
#include "ShaderCompiler.h"

class MaterialComponent : public Component {
public:

	enum TexturePurpose {
		GeneralTexture,
		NormalTexture,
	};

	MaterialComponent(std::shared_ptr<Program> program);
	
	void set_texture(const std::string& name, std::shared_ptr<Texture2D> texture, TexturePurpose texture_purpose = GeneralTexture);
	std::shared_ptr<Texture2D> get_texture2d(const std::string& name);
	TexturePurpose get_texture2d_purpose(const std::string& name);
	void remove_texture(const std::string& name);


	std::shared_ptr<Program> get_program();
	
	/*
	void on_activated();
	void on_deactivated();
	void on_initialized();
	void on_destructed();

	void on_begin();
	void on_update();

	void on_fixed_update();
	void on_pre_render();
	void on_post_render();

	void on_pre_physics();
	void on_post_physics();
	*/


private:

	struct _texture_with_purpose {
	public:
		_texture_with_purpose(std::shared_ptr<Texture2D> texture = nullptr, TexturePurpose purpose = GeneralTexture) {
			_texture = texture;
			_purpose = purpose;
		}

		std::shared_ptr<Texture2D> _texture;
		TexturePurpose _purpose;
	};

	std::unordered_map<std::string, _texture_with_purpose> _textures;
	std::shared_ptr<Program> _program;
};