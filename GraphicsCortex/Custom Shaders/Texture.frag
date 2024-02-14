#<fragment shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;

in float tex_index_color;
in vec2 frag_uv;

layout(std140) uniform textures{
    sampler2D bindless_textures[1024*4];
};

void main(){
    frag_color = texture(bindless_textures[int(round(tex_index_color))], frag_uv);
}
