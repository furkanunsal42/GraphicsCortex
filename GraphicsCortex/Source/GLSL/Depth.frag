#<fragment shader>

#version 330 core 

layout (location = 0) out vec4 frag_color;

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main(){
	float depth = gl_FragCoord.z;
    //depth = linearize_depth(depth, 0.1f, 100.0f);
    frag_color = vec4(vec3(depth), 1);
}
