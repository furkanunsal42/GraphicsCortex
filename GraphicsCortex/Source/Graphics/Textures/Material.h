#pragma once

#include <unordered_map>
#include "ShaderCompiler.h"

#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Texture2DArray.h"
#include "TextureCubeMap.h"

class Material {
public:
	Material(const Material& other) = delete;

	std::shared_ptr<Program> program = nullptr;

	Material(const std::shared_ptr<Program>& program);
	Material();

	void set_program(const std::shared_ptr<Program>& program);
	void set_program_without_texture_reset(const std::shared_ptr<Program>& program);
	void reset_textures();

	void add_texture1d(const std::string& name, const std::shared_ptr<Texture1D>& texture);
	void add_texture2d(const std::string& name, const std::shared_ptr<Texture2D>& texture);
	void add_texture3d(const std::string& name, const std::shared_ptr<Texture3D>& texture);
	void add_texture2d_array(const std::string& name, const std::shared_ptr<Texture2DArray>& texture);
	void add_texture_cubemap(const std::string& name, const std::shared_ptr<TextureCubeMap>& texture);

	void remove_texture1d(const std::string& name);
	void remove_texture2d(const std::string& name);
	void remove_texture3d(const std::string& name);
	void remove_texture2d_array(const std::string& name);
	void remove_texture_cubemap(const std::string& name);
	
	std::shared_ptr<Texture1D> get_texture1d(const std::string& name);
	std::shared_ptr<Texture2D> get_texture2d(const std::string& name);
	std::shared_ptr<Texture3D> get_texture3d(const std::string& name);
	std::shared_ptr<Texture2DArray> get_texture2d_array(const std::string& name);
	std::shared_ptr<TextureCubeMap> get_texture_cubemap(const std::string& name);

	void update_uniforms();

private:
	std::unordered_map<std::string, std::shared_ptr<Texture1D>> texture_1d_map;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> texture_2d_map;
	std::unordered_map<std::string, std::shared_ptr<Texture3D>> texture_3d_map;
	std::unordered_map<std::string, std::shared_ptr<Texture2DArray>> texture_2d_array_map;
	std::unordered_map<std::string, std::shared_ptr<TextureCubeMap>> texture_cubemap_map;
};

