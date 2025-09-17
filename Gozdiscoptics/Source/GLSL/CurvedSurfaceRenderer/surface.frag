#<fragment shader>

#version 460 core 

layout (location = 0) out vec4 frag_color;

uniform vec4 color;
in vec3 v_position;

void main(){

	vec3 background_color = vec3(85, 156, 213)/255.0;
	vec3 line_color = vec3(1);

	vec3 line = any(lessThan(mod(v_position.xz, vec2(0.11)) , vec2(0.001))) ? vec3(1) : vec3(85, 156, 213)/255.0;
	frag_color = vec4(line, 1);
	
	//frag_color = vec4(result, 1);
 }

