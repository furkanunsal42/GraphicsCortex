#<fragment shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 frag_uv;

uniform uvec2 texture_slot;

layout(location = 0, bindless_sampler) uniform sampler2D Texture0;

void main(){
    frag_color = texture(Texture0, frag_uv);
}
