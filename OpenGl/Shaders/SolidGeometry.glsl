#<geometry shader>

#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 tex_coords;
out vec3 frag_normal;
out vec3 frag_space_coord;

in DATA
{
	vec2 tex_coords;
	vec3 frag_normal;
	vec3 frag_space_coord;
	mat4 projection;
} data_in[];

void main(){
	gl_Position = /* data_in[0].projection * */ gl_in[0].gl_Position;
	tex_coords = data_in[0].tex_coords;
	frag_normal = data_in[0].frag_normal;
	frag_space_coord = data_in[0].frag_space_coord;
	EmitVertex();

	gl_Position = /* data_in[1].projection * */ gl_in[1].gl_Position;
	tex_coords = data_in[1].tex_coords;
	frag_normal = data_in[1].frag_normal;
	frag_space_coord = data_in[1].frag_space_coord;
	EmitVertex();

	gl_Position = /* data_in[2].projection * */ gl_in[2].gl_Position;
	tex_coords = data_in[2].tex_coords;
	frag_normal = data_in[2].frag_normal;
	frag_space_coord = data_in[2].frag_space_coord;
	EmitVertex();
	EndPrimitive();
}