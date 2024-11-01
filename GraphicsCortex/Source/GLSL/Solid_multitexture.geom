#<geometry shader>

#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec2 tex_coords;
out vec3 frag_normal;
out vec3 frag_space_coord;
out mat3 frag_TBN;
out float map_index_color;
out float map_index_specular;
out float map_index_normal;
in DATA
{
	vec2 tex_coords;
	vec3 frag_normal;
	vec3 frag_space_coord;
	mat4 projection;
	mat4 model;
	mat4 view;
	float map_index_color;
	float map_index_specular;
	float map_index_normal;
} data_in[];

void main(){
	
	// Edges of the triangle
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // Lengths of UV differences
    vec2 deltaUV0 = data_in[1].tex_coords - data_in[0].tex_coords;
    vec2 deltaUV1 = data_in[2].tex_coords - data_in[0].tex_coords;

    // one over the determinant
    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(vec4(bitangent, 0.0f)));
    //vec3 N = normalize(-vec3(vec4(cross(edge1, edge0), 0.0f)));
    vec3 N = normalize(data_in[0].frag_normal);

    mat3 TBN = mat3(T, B, N);
    // TBN is an orthogonal matrix and so its inverse is equal to its transpose
    //TBN = transpose(TBN);
	
	frag_TBN = TBN;
	gl_Position = data_in[0].projection * data_in[0].view * gl_in[0].gl_Position;
	tex_coords = data_in[0].tex_coords;
	frag_normal = data_in[0].frag_normal;
	frag_space_coord = data_in[0].frag_space_coord;
	map_index_color = data_in[0].map_index_color;
	map_index_specular = data_in[0].map_index_color;
	map_index_normal = data_in[0].map_index_color;
	EmitVertex();

	frag_TBN = TBN;
	gl_Position = data_in[0].projection * data_in[0].view * gl_in[1].gl_Position;
	tex_coords = data_in[1].tex_coords;
	frag_normal = data_in[1].frag_normal;
	frag_space_coord = data_in[1].frag_space_coord;
	map_index_color = data_in[1].map_index_color;
	map_index_specular = data_in[1].map_index_color;
	map_index_normal = data_in[1].map_index_color;
	EmitVertex();

	frag_TBN = TBN;
	gl_Position = data_in[0].projection * data_in[0].view * gl_in[2].gl_Position;
	tex_coords = data_in[2].tex_coords;
	frag_normal = data_in[2].frag_normal;
	frag_space_coord = data_in[2].frag_space_coord;
	map_index_color = data_in[2].map_index_color;
	map_index_specular = data_in[2].map_index_color;
	map_index_normal = data_in[2].map_index_color;
	EmitVertex();
	EndPrimitive();
}