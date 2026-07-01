#pragma once
#include "Texture2D.h"
#include <unordered_map>

#include "UniformBuffer.h"
#include "Buffer.h"

class BindlessTextureBank {
public:

	static constexpr uint32_t invalid_offset = std::numeric_limits<uint32_t>::max();

	uint32_t	add(std::shared_ptr<Texture2D> bindless_texture);
	void		remove(Texture2D& bindless_texture);
	bool		does_exist(Texture2D& bindless_texture);
	uint32_t	get_offset(Texture2D& bindless_texture);

	void		clear();
	
	Buffer&		get_texture_handle_buffer();

private:
	
	struct TextureInfo {
		uint32_t id = 0;
		uint64_t handle = 0;
	};

	uint32_t next_texture_offset = 0;
	uint32_t generate_texture_offset();

	std::unordered_map<uint32_t, TextureInfo> offset_to_info;
	std::unordered_map<uint32_t, uint32_t> id_to_offset;

	bool is_handle_buffer_up_to_date = false;
	std::shared_ptr<UniformBuffer> handle_buffer = nullptr;
};