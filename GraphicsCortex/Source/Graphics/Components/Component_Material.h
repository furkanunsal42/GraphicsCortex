#pragma once
#include "Component.h"

#include <unordered_map>
#include <string>

#include "Texture2D.h"
#include "ShaderCompiler.h"

class MaterialComponent : public Component {
public:

	void set_texture(const std::string& name, std::shared_ptr<Texture2D> texture) {};
	void remove_texture(const std::string& name) {};
	
	MaterialComponent(std::shared_ptr<Program> program) { _program = program; };

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
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> _textures;
	std::shared_ptr<Program> _program;
};