#<fragment shader>
#version 460 core 

layout (location = 0) out vec4 frag_color;

in vec3 v_position;
in vec2 v_texcoord;

uniform sampler2D atlas;
//uniform vec2 screen_resolution;
uniform vec4 color;

void main(){
    //vec2 fragment_size = (1.0f/screen_resolution);
    frag_color = texture(atlas, v_texcoord/*+(vec2(0.5, 0.5) * fragment_size)*/) * color;
}