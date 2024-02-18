#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec2 frag_uv;

uniform sampler2D texture_slot;
uniform vec2 screen_resolution = vec2(1920, 1080);

uniform vec4 text_color = vec4(1);

void main(){
    vec2 fragment_size = (1.0f/screen_resolution);
    //vec2 fragment_size = vec2(0);
    frag_color = texture(texture_slot, frag_uv+(vec2(0.5, 0.5) * fragment_size)) * text_color;
}
