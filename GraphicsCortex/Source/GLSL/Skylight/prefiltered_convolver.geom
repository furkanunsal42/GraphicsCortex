#<geometry shader>

#version 460
#extension GL_EXT_geometry_shader4 : enable

layout(triangles) in;
layout(triangle_strip, max_vertices=3*6) out;

mat4 projection = mat4(
    vec4(1, 0, 0, 0),
    vec4(0, 1, 0, 0),
    vec4(0, 0, -1.0202, -1),
    vec4(0, 0, -0.20202, 0)
);

mat4 views[6] = {
    mat4(
        vec4(0, 0, -1, 0),
        vec4(0, -1, 0, 0),
        vec4(-1, 0, 0, 0),
        vec4(0, 0, 0, 1)
    ),
    mat4(
        vec4(0, 0, 1, 0),
        vec4(0, -1, 0, 0),
        vec4(1, 0, 0, 0),
        vec4(0, 0, 0, 1)
    ),
    mat4(
        vec4(1, 0, -0, 0),
        vec4(0, 0, -1, 0),
        vec4(0, 1, -0, 0),
        vec4(-0, -0, 0, 1)
    ),
    mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(0, -1, 0, 0),
        vec4(0, 0, 0, 1)
    ),
     mat4(
        vec4(1, 0, 0, 0),
        vec4(0, -1, 0, 0),
        vec4(0, 0, -1, 0),
        vec4(0, 0, 0, 1)
    ),
    mat4(
        vec4(-1, 0, 0, 0),
        vec4(0, -1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(0, 0, 0, 1)
    )
};

in vec3 v_local_position[];

out vec3 g_local_position;

void main(void) {
    int i, layer;
    for (layer = 0; layer < 6; layer++) {
        gl_Layer = layer;
        for (i = 0; i < 3; i++) {
            g_local_position = v_local_position[i];
            gl_Position = projection * views[layer] * gl_PositionIn[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
