#<geometry shader>

#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 v_position;
out vec2 v_texcoord;
out vec3 v_normal;
out mat3 frag_TBN;

in VSOut
{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
} i[];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	
	// Edges of the triangle
    vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    // Lengths of UV differences
    vec2 deltaUV0 = i[1].texcoord - i[0].texcoord;
    vec2 deltaUV1 = i[2].texcoord - i[0].texcoord;

    // one over the determinant
    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    vec3 tangent = vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vec3 bitangent = vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));

    vec3 T = normalize(vec3(vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(vec4(bitangent, 0.0f)));
    //vec3 N = normalize(-vec3(vec4(cross(edge1, edge0), 0.0f)));
    vec3 N = normalize(i[0].normal);

    mat3 TBN = mat3(T, B, N);
    //TBN is an orthogonal matrix and so its inverse is equal to its transpose
    //TBN = transpose(TBN);
	
	frag_TBN = TBN;
	gl_Position = projection * view * model * vec4(i[0].position, 1.0f);
	v_position = i[0].position;
	v_texcoord = i[0].texcoord;
	v_normal = i[0].normal;
	EmitVertex();

	frag_TBN = TBN;
	gl_Position = projection * view * model * vec4(i[1].position, 1.0f);
	v_position = i[1].position;
	v_texcoord = i[1].texcoord;
	v_normal = i[1].normal;
	EmitVertex();

	frag_TBN = TBN;
	gl_Position = projection * view * model * vec4(i[2].position, 1.0f);
	v_position = i[2].position;
	v_texcoord = i[2].texcoord;
	v_normal = i[2].normal;
	EmitVertex();
	EndPrimitive();
}