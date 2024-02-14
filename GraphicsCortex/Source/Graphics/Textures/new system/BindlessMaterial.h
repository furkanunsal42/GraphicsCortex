#pragma once
#include "Texture2D.h"
#include "ShaderCompiler.h"
#include <unordered_map>

class BindlessMaterial {
public:
	std::shared_ptr<Program> program = nullptr;

	BindlessMaterial();
	BindlessMaterial(const BindlessMaterial& other) = delete;

	BindlessMaterial(const std::shared_ptr<Program>& program);
	bool operator==(const BindlessMaterial& other);

	void set_program(const std::shared_ptr<Program>& program);
	void set_program_without_texture_reset(const std::shared_ptr<Program>& program);

	void add_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture);
	void remove_texture(const std::string& name);
	
	void update_uniforms();
private:

	std::unordered_map<unsigned int, std::shared_ptr<Texture2D>> _textures;
};