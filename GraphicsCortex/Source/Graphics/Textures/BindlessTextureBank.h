#pragma once
#include "Texture2D.h"
#include <unordered_map>

class BindlessTextureBank {
public:

	static BindlessTextureBank& get();

	void add_texture(Texture2D& bindless_texture);
	void remove_texture(Texture2D& bindless_texture);
	uint32_t get_texture_offset(Texture2D& bindless_texture);
	bool is_texture_present(Texture2D& bindless_texture);
	
	void clear();

private:
	
	BindlessTextureBank();

	struct TextureInfo {
		uint32_t id = 0;
		uint64_t handle = 0;
	};

	uint32_t next_texture_offset = 0;
	uint32_t generate_texture_offset();

	std::unordered_map<uint32_t, TextureInfo> offset_to_info;
	std::unordered_map<uint32_t, uint32_t> id_to_offset;
};