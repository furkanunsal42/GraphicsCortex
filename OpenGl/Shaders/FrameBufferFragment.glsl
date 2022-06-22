#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;
in vec2 frag_tex_coords;
uniform sampler2D texture_slot;
uniform int edge_detect;

float offset = 1/500.0f;
int kernel[] = {
	1, 0, 1,
	0,-4, 0,
	1, 0, 1,
};

void main(){
	if (edge_detect){
		float x, y;
		for (int i = 0; i < 9; i++){
			x = ((i % 3) - 1) * offset;
			y = ((i / 3) - 1) * offset;
			frag_color += (kernel[i] * texture(texture_slot, vec2(frag_tex_coords.x + x, frag_tex_coords.y + y)));
		}
	}
	else{
		frag_color = texture(texture_slot, frag_tex_coords);
	}
}
