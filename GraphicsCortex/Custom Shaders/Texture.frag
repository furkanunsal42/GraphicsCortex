#<fragment shader>

#version 460 core
#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 frag_color;
in vec2 frag_uv;

layout(bindless_sampler) uniform sampler2D textures[1];

void main(){
    frag_color = texture(textures[0], frag_uv);
}
