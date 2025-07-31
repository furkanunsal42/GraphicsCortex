#include "BindlessTextureBank.h"
#include "Debuger.h"
#include "WindowBoundGlobalResources.h"

#include <iostream>

BindlessTextureBank& BindlessTextureBank::get()
{
    if (active_global_resources->BindlessTextureBank == nullptr)
        active_global_resources->BindlessTextureBank = std::unique_ptr<BindlessTextureBank>(new BindlessTextureBank());

    return *active_global_resources->BindlessTextureBank;
}

void BindlessTextureBank::add_texture(Texture2D& bindless_texture)
{
    if (is_texture_present(bindless_texture))
        return;

    TextureInfo info;
    info.id = bindless_texture.id;
    info.handle = bindless_texture.texture_handle;
    uint32_t offset = generate_texture_offset();
    
    offset_to_info[offset] = info;
    id_to_offset[bindless_texture.id] = offset;
}

void BindlessTextureBank::remove_texture(Texture2D& bindless_texture)
{
    if (!is_texture_present(bindless_texture))
        return;

    uint32_t offset = id_to_offset.at(bindless_texture.id);
    id_to_offset.erase(bindless_texture.id);
    offset_to_info.erase(offset);
}

uint32_t BindlessTextureBank::get_texture_offset(Texture2D& bindless_texture)
{
    if (!is_texture_present(bindless_texture)) {
        std::cout << "[BindlessTextureBank Error] BindlessTextureBank::get_texture_offset() is called but given texture is not present" << std::endl;
        ASSERT(false);
    }

    return id_to_offset.at(bindless_texture.id);
}

bool BindlessTextureBank::is_texture_present(Texture2D& bindless_texture)
{
    return id_to_offset.find(bindless_texture.id) != id_to_offset.end();
}

void BindlessTextureBank::clear()
{
    id_to_offset.clear();
    offset_to_info.clear();
}

uint32_t BindlessTextureBank::generate_texture_offset()
{
    uint32_t generated_offset = next_texture_offset;
    next_texture_offset++;
    return generated_offset;
}
