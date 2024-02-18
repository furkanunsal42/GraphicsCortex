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

	void add_texture(int index, const std::shared_ptr<Texture2D>& texture);
	void remove_texture(int index);
	
	void update_uniforms();

	const int _buffer_texture_count = 1024*4;
	int _buffer_usage_offset = 0;
	std::string texture_buffer_name = "textures";
	std::shared_ptr<UniformBuffer> _texture_uniform_buffer = std::make_shared<UniformBuffer>();
	std::unordered_map<int, std::shared_ptr<Texture2D>> _textures;
private:
};