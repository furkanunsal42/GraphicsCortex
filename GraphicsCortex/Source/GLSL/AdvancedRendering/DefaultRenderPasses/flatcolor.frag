#<fragment shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

struct MaterialData {
    uint texture_begin;
    uint texture_count;
};

struct TextureData {
    uvec2 texture_handle;
    uvec2 color;
};

layout(std430, binding = 5) readonly buffer material_buffer { 
    MaterialData materials[]; 
};

layout(std430, binding = 6) readonly buffer texture_buffer { 
    TextureData textures[]; 
};

vec4 fetch_material_color(MaterialData material, uint index){
    
    if (material.texture_count <= index)
        return vec4(0, 0, 0, 0);
    else
        return vec4(
            unpackHalf2x16(textures[material.texture_begin + index].color.x), 
            unpackHalf2x16(textures[material.texture_begin + index].color.y)
        );
}

vec4 fetch_material_texture(MaterialData material, uint index, vec2 uv){

    if (material.texture_count <= index)
        return fetch_material_color(material, index);
    else
        return texture(sampler2D(textures[material.texture_begin + index].texture_handle), uv);

}

// --- Inputs from Vertex Shader ---
in vec3         v_local_pos;
in vec3         v_world_pos;
in flat uint    v_material_id;

// --- Output to Framebuffer ---
out vec4 frag_color;

uniform vec4 color;

void main() {
    //MaterialData material = materials[v_material_id];
    //frag_color = fetch_material_texture(material, 0, vec2(0.5));

    frag_color = color;
}