#<vertex shader>

#version 460 core

// --------------------------------------------------------
// UNIFIED PROGRAMMABLE VERTEX PULLING SHADER
// --------------------------------------------------------

const uint invalid_id = 0xFFFFFFFF;

struct ObjectMaterialData {
    uint object_id;
    uint material_id;
};

struct MeshData {
    uint vertex_begin;
    uint vertex_count;
    uint index_begin;
    uint index_count;
    uint format_id;
    uint _padding0, _padding1, _padding2;
    vec4 aabb_min;
    vec4 aabb_max;
};

struct ObjectData {
    mat4 model_matrix;
    vec4 aabb_min_mesh_id;
    vec4 aabb_max;
};

struct VertexFormatData {
    uint stride_in_bytes;
    uint active_attributes_mask;
    uint padding1;
    uint padding2; 
    uint attributes[16];
};

layout(std430, binding = 0) readonly buffer mesh_buffer            { MeshData meshes[]; };
layout(std430, binding = 1) readonly buffer object_buffer          { ObjectData objects[]; };
layout(std430, binding = 2) readonly buffer object_material_buffer { ObjectMaterialData pass_data[]; };
layout(std430, binding = 3) readonly buffer format_buffer          { VertexFormatData formats[]; };
layout(std430, binding = 4) readonly buffer vertex_buffer          { uint vertices[]; };

uniform mat4 projection_view_matrix;

// ---------------------------------------------------------
// STANDARD FLOATS (Positions, non-normalized data)
// ---------------------------------------------------------
float fetch_f32_1(uint byte_addr) {
    return uintBitsToFloat(vertices[byte_addr / 4]);
}

vec2 fetch_f32_2(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec2(
        uintBitsToFloat(vertices[word_idx]),
        uintBitsToFloat(vertices[word_idx + 1])
    );
}

vec3 fetch_f32_3(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec3(
        uintBitsToFloat(vertices[word_idx]),
        uintBitsToFloat(vertices[word_idx + 1]),
        uintBitsToFloat(vertices[word_idx + 2])
    );
}

vec4 fetch_f32_4(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec4(
        uintBitsToFloat(vertices[word_idx]),
        uintBitsToFloat(vertices[word_idx + 1]),
        uintBitsToFloat(vertices[word_idx + 2]),
        uintBitsToFloat(vertices[word_idx + 3])
    );
}

// ---------------------------------------------------------
// HALF FLOATS (UVs, HDR Colors)
// ---------------------------------------------------------
vec2 fetch_f16_2(uint byte_addr) {
    return unpackHalf2x16(vertices[byte_addr / 4]);
}

vec4 fetch_f16_4(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec4(
        unpackHalf2x16(vertices[word_idx]), 
        unpackHalf2x16(vertices[word_idx + 1])
    );
}

// ---------------------------------------------------------
// NORMALIZED FORMATS (Colors, Normals, Tangents)
// ---------------------------------------------------------
vec4 fetch_unorm8x4(uint byte_addr) {
    return unpackUnorm4x8(vertices[byte_addr / 4]);
}

vec4 fetch_snorm8x4(uint byte_addr) {
    return unpackSnorm4x8(vertices[byte_addr / 4]);
}

vec2 fetch_unorm16x2(uint byte_addr) {
    return unpackUnorm2x16(vertices[byte_addr / 4]);
}
vec4 fetch_unorm16x4(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec4(unpackUnorm2x16(vertices[word_idx]), unpackUnorm2x16(vertices[word_idx + 1]));
}

vec2 fetch_snorm16x2(uint byte_addr) {
    return unpackSnorm2x16(vertices[byte_addr / 4]);
}
vec4 fetch_snorm16x4(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    return vec4(unpackSnorm2x16(vertices[word_idx]), unpackSnorm2x16(vertices[word_idx + 1]));
}

vec4 fetch_snorm10_10_10_2(uint byte_addr) {
    uint val = vertices[byte_addr / 4];
    // bitwise sign-extension trick for 10-bit signed integers
    return vec4(
        float(int(val << 22) >> 22) / 511.0,
        float(int(val << 12) >> 22) / 511.0,
        float(int(val << 2) >> 22)  / 511.0,
        float(int(val) >> 30)       / 1.0
    );
}

// ---------------------------------------------------------
// PURE INTEGERS (Bone IDs, Vertex IDs, Material Slots)
// ---------------------------------------------------------
uvec4 fetch_u8_4(uint byte_addr) {
    uint val = vertices[byte_addr / 4];
    return uvec4(val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF, val >> 24);
}

uvec4 fetch_u16_4(uint byte_addr) {
    uint word_idx = byte_addr / 4;
    uint v0 = vertices[word_idx];
    uint v1 = vertices[word_idx + 1];
    return uvec4(v0 & 0xFFFF, v0 >> 16, v1 & 0xFFFF, v1 >> 16);
}

uint fetch_u32_1(uint byte_addr) {
    return vertices[byte_addr / 4];
}

// ---------------------------------------------------------
// GENERICS
// ---------------------------------------------------------
vec4 fetch_vec4(uint addr, uint type) {

    uint w = addr / 4; // Convert byte address to 32-bit word index

    switch (type) {
        // --- Floats ---
        case 1:  return vec4(uintBitsToFloat(vertices[w]), 0.0, 0.0, 1.0);
        case 2:  return vec4(uintBitsToFloat(vertices[w]), uintBitsToFloat(vertices[w+1]), 0.0, 1.0);
        case 3:  return vec4(uintBitsToFloat(vertices[w]), uintBitsToFloat(vertices[w+1]), uintBitsToFloat(vertices[w+2]), 1.0);
        case 4:  return vec4(uintBitsToFloat(vertices[w]), uintBitsToFloat(vertices[w+1]), uintBitsToFloat(vertices[w+2]), uintBitsToFloat(vertices[w+3]));
        
        // --- Half Floats ---
        case 5:  return vec4(unpackHalf2x16(vertices[w]), 0.0, 1.0);
        case 6:  return vec4(unpackHalf2x16(vertices[w]), unpackHalf2x16(vertices[w+1]));
        
        // --- Unsigned Normalized (0.0 to 1.0) ---
        case 7:  return unpackUnorm4x8(vertices[w]);
        case 8:  return vec4(unpackUnorm2x16(vertices[w]), 0.0, 1.0);
        case 9:  return vec4(unpackUnorm2x16(vertices[w]), unpackUnorm2x16(vertices[w+1]));
        
        // --- Signed Normalized (-1.0 to 1.0) ---
        case 10: return unpackSnorm4x8(vertices[w]);
        case 11: return vec4(unpackSnorm2x16(vertices[w]), 0.0, 1.0);
        case 12: return vec4(unpackSnorm2x16(vertices[w]), unpackSnorm2x16(vertices[w+1]));
        case 13: {
            uint val = vertices[w];
            return vec4(
                float(int(val << 22) >> 22) / 511.0,
                float(int(val << 12) >> 22) / 511.0,
                float(int(val << 2)  >> 22) / 511.0,
                float(int(val) >> 30)       / 1.0
            );
        }
        
        // Fallback for Disabled or pure Int formats being misread as Floats
        default: return vec4(0.0, 0.0, 0.0, 1.0); 
    }
}

uvec4 fetch_uvec4(uint addr, uint type) {
    uint w = addr / 4;
    
    switch (type) {
        // --- Pure Unsigned Integers ---
        case 14: {
            uint val = vertices[w];
            return uvec4(val & 0xFF, (val >> 8) & 0xFF, (val >> 16) & 0xFF, val >> 24);
        }
        case 15: 
        {
            uint v0 = vertices[w];
            uint v1 = vertices[w+1];
            return uvec4(v0 & 0xFFFF, v0 >> 16, v1 & 0xFFFF, v1 >> 16);
        }
        case 16: return uvec4(vertices[w], 0, 0, 1);
        
        default: return uvec4(0, 0, 0, 1);
    }
}

ivec4 fetch_ivec4(uint addr, uint type) {
    return ivec4(fetch_uvec4(addr, type));
}

#define custom_output_variables 

custom_output_variables

#define PULL_ATTRIBUTE_0 { v_local_pos = vec3(fetch_vec4(addr, type)); v_world_pos = (obj.model_matrix * vec4(v_local_pos, 1.0)).xyz; gl_Position = projection_view_matrix * obj.model_matrix * vec4(v_local_pos, 1.0); }
#define PULL_ATTRIBUTE_1 { ; }
#define PULL_ATTRIBUTE_2 { ; }
#define PULL_ATTRIBUTE_3 { ; }
#define PULL_ATTRIBUTE_4 { ; }
#define PULL_ATTRIBUTE_5 { ; }
#define PULL_ATTRIBUTE_6 { ; }
#define PULL_ATTRIBUTE_7 { ; }
#define PULL_ATTRIBUTE_8 { ; }
#define PULL_ATTRIBUTE_9 { ; }
#define PULL_ATTRIBUTE_10 { ; }
#define PULL_ATTRIBUTE_11 { ; }
#define PULL_ATTRIBUTE_12 { ; }
#define PULL_ATTRIBUTE_13 { ; }
#define PULL_ATTRIBUTE_14 { ; }
#define PULL_ATTRIBUTE_15 { ; }

// --- Outputs to Fragment Shader ---

out vec3 v_world_pos;
out vec3 v_local_pos;
out flat uint v_material_id;

void main() {
    
    uint pass_idx = gl_BaseInstance; 
    
    ObjectMaterialData obj_mat = pass_data[pass_idx];
    
    uint obj_id     = obj_mat.object_id;
    v_material_id   = obj_mat.material_id;

    ObjectData obj  = objects[obj_id];
    uint mesh_id    = floatBitsToUint(obj.aabb_min_mesh_id.w);
    
    MeshData mesh           = meshes[mesh_id];
    VertexFormatData format = formats[mesh.format_id];
    
    uint base_vertex_addr = mesh.vertex_begin + (gl_VertexID * format.stride_in_bytes);

    // PROGRAMMATIC PULLING

    for (int i = 0; i < 16; ++i) {
        if ((format.active_attributes_mask & (1 << i)) != 0) {
            uint attr = format.attributes[i];
            uint type = attr >> 16;
            uint offset = attr & 0xFFFF;
            uint addr = base_vertex_addr + offset;

            if      (i == 0)    { PULL_ATTRIBUTE_0 }
            else if (i == 1)    { PULL_ATTRIBUTE_1 }
            else if (i == 2)    { PULL_ATTRIBUTE_2 }
            else if (i == 3)    { PULL_ATTRIBUTE_3 }
            else if (i == 4)    { PULL_ATTRIBUTE_4 }
            else if (i == 5)    { PULL_ATTRIBUTE_5 }
            else if (i == 6)    { PULL_ATTRIBUTE_6 }
            else if (i == 7)    { PULL_ATTRIBUTE_7 }
            else if (i == 8)    { PULL_ATTRIBUTE_8 }
            else if (i == 9)    { PULL_ATTRIBUTE_9 }
            else if (i == 10)   { PULL_ATTRIBUTE_10 }
            else if (i == 11)   { PULL_ATTRIBUTE_11 }
            else if (i == 12)   { PULL_ATTRIBUTE_12 }
            else if (i == 13)   { PULL_ATTRIBUTE_13 }
            else if (i == 14)   { PULL_ATTRIBUTE_14 }
            else if (i == 15)   { PULL_ATTRIBUTE_15 }
        }
    }
}