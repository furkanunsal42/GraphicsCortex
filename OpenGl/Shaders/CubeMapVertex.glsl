#<vertex shader>

#version 330 core

layout(location = 0) in vec3 position;

out vec3 frag_tex_coords;

uniform mat4 view;
uniform mat4 projection;

void main(){
	vec4 matrix_position = projection * view  * vec4(position, 1.0f);
	gl_Position = vec4(matrix_position.xy, matrix_position.w, matrix_position.w); // to make z always -1.0f, behind everything else

	frag_tex_coords = vec3(position.xy, -position.z); // to counter opengl z direction inconsistancy with cubemaps

}