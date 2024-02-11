#<fragment shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 frag_uv;

layout(bindless_sampler) uniform sampler2D color_texture;

void main(){
    frag_color = texture(color_texture, frag_uv);
}
