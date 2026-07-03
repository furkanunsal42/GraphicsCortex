#pragma once

#include "Buffer.h"
#include "DynamicBuffer.h"
#include "ResizableBuffer.h"
#include "VertexAttributeBuffer.h"
#include "ShaderCompiler.h"
#include "ComputeProgram.h"
#include "Tools/VariantedComputeProgram/VariantedComputeProgram.h"
#include "FrameBuffer.h"
#include "RenderParameters.h"
#include "Camera.h"
#include "IndexBufferEnums.h"
#include "Math/AABB.h"
#include <functional>
#include <packing.hpp>

extern std::filesystem::path unified_renderer_shader_parent_path;

class UnifiedRenderer {
public:

    static UnifiedRenderer& get();

    static constexpr uint32_t invalid_id = std::numeric_limits<uint32_t>::max();

    enum RenderOrder { 
        unordered, 
        towards_camera, 
        away_from_camera 
    };
    
    enum AttributeType : uint16_t {
        disabled = 0,
        f32_1, f32_2, f32_3, f32_4,     // Standard Floats (Positions)
        f16_2, f16_4,                   // Half Floats (UVs, HDR Colors)
        unorm8x4, unorm16x2, unorm16x4, // Unsigned Normalized 0.0 to 1.0 (LDR Colors, Bone Weights)
        snorm8x4, snorm16x2, snorm16x4, // Signed Normalized -1.0 to 1.0 (Normals, Tangents)
        snorm10_10_10_2,
        u8_4, u16_4, u32_1              // Pure Integers (Bone Indices, Vertex IDs)
    };

    enum GenericType {
        float_t,
        vec2_t,
        vec3_t,
        vec4_t,
        uint_t,
        uvec2_t,
        uvec4_t,
    };

    struct              RenderPassInfo;
    struct              TextureInfo;
    struct              MaterialInfo;
    struct              MeshInfo;
    struct              ObjectInfo;
    struct              RenderLayerInfo;

    uint32_t            create_render_pass(RenderParameters parameters = RenderParameters(true));
    uint32_t            create_render_pass(Shader shader, RenderParameters parameters = RenderParameters(true));
    void                release_render_pass(uint32_t render_pass_id);
    RenderPassInfo&     get_render_pass(uint32_t render_pass_id);
    void                iterate_render_passes(std::function<void(uint32_t, RenderPassInfo&)> lambda);
    
    uint32_t            create_texture(std::shared_ptr<Texture2D> texture = nullptr);
    uint32_t            create_texture(glm::vec4 flat_color);
    void                release_texture(uint32_t texture_id);
    TextureInfo&        get_texture(uint32_t texture_id);
    void                iterate_textures(std::function<void(uint32_t, TextureInfo&)> lambda);

    uint32_t            create_material();
    void                release_material(uint32_t material_id);
    MaterialInfo&       get_material(uint32_t material_id);
    void                iterate_materials(std::function<void(uint32_t, MaterialInfo&)> lambda);

    uint32_t            create_mesh(PrimitiveType mesh_primitive = triangle);
    void                release_mesh(uint32_t mesh_id);
    MeshInfo&           get_mesh(uint32_t mesh_id);
    void                iterate_meshes(std::function<void(uint32_t, MeshInfo&)> lambda);

    uint32_t            create_object(uint32_t mesh_id = invalid_id, glm::mat4 model_matrix = glm::identity<glm::mat4>());
    void                release_object(uint32_t object_id);
    ObjectInfo&         get_object(uint32_t object_id);
    void                iterate_objects(std::function<void(uint32_t, ObjectInfo&)> lambda);

    uint32_t            create_render_layer();
    void                release_render_layer(uint32_t render_layer_id);
    RenderLayerInfo&    get_render_layer(uint32_t render_layer_id);
    void                iterate_render_layers(std::function<void(uint32_t, RenderLayerInfo&)> lambda);

    void                render(uint32_t render_pass, const Camera& camera, RenderOrder order = unordered);

    struct VertexFormatData {
        uint32_t        stride_in_bytes         = 0;
        uint32_t        active_attributes_mask  = 0;
        uint32_t        padding0                = 0;
        uint32_t        padding1                = 0;
        uint32_t        attributes[16];

        VertexFormatData();

        void            set_attribute(int32_t location, AttributeType type, uint16_t byte_offset);
        bool            operator==(const VertexFormatData& other) const;
        friend          std::hash<UnifiedRenderer::VertexFormatData>;
    };
    struct MeshData {
        uint32_t        vertex_begin        = 0;
        uint32_t        vertex_count        = 0;
        uint32_t        index_begin         = 0;
        uint32_t        index_count         = 0;
        uint32_t        format_id           = invalid_id;
        uint32_t        _padding0, _padding1, _padding2 = 0;
        glm::vec4       aabb_min            = glm::vec4(glm::vec3(+std::numeric_limits<float>::max()), -1);
        glm::vec4       aabb_max            = glm::vec4(glm::vec3(-std::numeric_limits<float>::max()), -1);
    };
    struct ObjectData {
        glm::mat4       model_matrix        = glm::identity<glm::mat4>();
        glm::vec4       aabb_min_mesh_id    = glm::vec4(glm::vec3(+std::numeric_limits<float>::max()), glm::uintBitsToFloat(invalid_id));
        glm::vec4       aabb_max            = glm::vec4(glm::vec3(-std::numeric_limits<float>::max()), -1);
    };
    struct TextureData {
        glm::uvec2      handle              = glm::uvec2(0);
        glm::uvec2      color               = glm::uvec2(glm::packHalf2x16(glm::vec2(1)));
    };
    struct MaterialData {
        uint32_t        texture_id_begin    = invalid_id;
        uint32_t        texture_count       = 0;
    };
    struct ObjectMaterialData {
        uint32_t        object_id           = 0;
        uint32_t        material_id         = 0;
    };

    struct VertexFormatHasher {
        size_t          operator()(const VertexFormatData& format) const;
    };

    struct MeshInfo {
    public:

        PrimitiveType       get_primitive();
        void                set_primitive(PrimitiveType primitive);

        void                allocate_vertices(size_t size_in_bytes);
        void                allocate_indices(size_t size_in_bytes);

        size_t              get_index_begin();
        size_t              get_index_size_in_bytes();
        size_t              get_vertex_begin();
        size_t              get_vertex_size_in_bytes();

        void                load_vertices(void* data, size_t size_in_bytes, size_t offset_in_bytes = 0);
        void                load_indices(void* data, size_t size_in_bytes, size_t offset_in_bytes = 0);
        void                get_vertices(void* data, size_t size_in_bytes, size_t offset_in_bytes = 0);
        void                get_indices(void* data, size_t size_in_bytes, size_t offset_in_bytes = 0);
        void                clear_vertices(uint8_t value, size_t size_in_bytes, size_t offset_in_bytes = 0);
        void                clear_indices(uint8_t value, size_t size_in_bytes, size_t offset_in_bytes = 0);
        AABB3               get_aabb();
        void                invalidate_aabb();

        void                set_vertex_stride(uint32_t stride_in_bytes);
        uint32_t            get_vertex_stride();
        void                push_attribute(int32_t location, AttributeType type);
        void                set_attribute(int32_t location, AttributeType type, uint16_t byte_offset);
        AttributeType       get_attributte_type(int32_t location);
        uint16_t            get_attribute_offset(int32_t location);

    private:
        VertexFormatData    local_format;
        PrimitiveType       primitive               = triangle;
        void*               index_begin_v           = DynamicBuffer::invalid_address;
        size_t              index_size_in_bytes     = 0;
        void*               vertex_begin_v          = DynamicBuffer::invalid_address;
        size_t              vertex_size_in_bytes    = 0;
        uint32_t            format_id               = invalid_id;
        uint32_t            self_id                 = invalid_id;
        bool                is_format_up_to_date    = false;
        bool                is_mesh_up_to_date      = false;
        bool                is_aabb_up_to_date      = false;
        UnifiedRenderer*    owner                   = nullptr;
        friend UnifiedRenderer;
    };

    struct TextureInfo {
        
        using texture_t     = std::shared_ptr<Texture2D>;

        void                set_texture(texture_t texture);
        texture_t           get_texture();
        void                set_color(glm::vec4 color);
        glm::vec4           get_color();

    private:
        glm::vec4           color                       = glm::vec4(1);
        texture_t           texture                     = nullptr;
        bool                is_up_to_date               = false;
        UnifiedRenderer*    owner                       = nullptr;
        friend UnifiedRenderer;
    };

    struct MaterialInfo {
        void                allocate(size_t texture_count);
        void                set_texture(uint32_t index, uint32_t texture_id);
        uint32_t            get_texture(uint32_t index);
        size_t              size();
    private:
        std::vector<uint32_t> textures;
        void*               textures_address_v          = DynamicBuffer::invalid_address;
        size_t              texture_count               = 0; // represents the count allocated on the gpu, use textures.size() for the data on the cpu
        bool                is_up_to_date               = false;
        UnifiedRenderer*    owner                       = nullptr;
        friend UnifiedRenderer;
    };

    struct ObjectInfo {
        void                set_model_matrix(glm::mat4 model_matrix);
        glm::mat4           get_model_matrix();
        void                set_mesh(uint32_t mesh);
        uint32_t            get_mesh();

    private:
        glm::mat4           model_matrix                = glm::identity<glm::mat4>();
        uint32_t            mesh                        = invalid_id;
        bool                is_transform_up_to_date     = false;
        bool                is_mesh_up_to_date          = false;
        UnifiedRenderer*    owner                       = nullptr;
        friend UnifiedRenderer;
    };

    struct RenderLayerInfo {

        void                object_add(uint32_t object_id, uint32_t material_id = invalid_id);
        void                object_remove(uint32_t object_id);
        bool                object_does_exist(uint32_t object_id);
        uint32_t            object_get_material(uint32_t object_id);
        void                object_set_material(uint32_t object_id, uint32_t new_material);
        size_t              object_count();

    private:

        std::vector<ObjectMaterialData>         objects;            // for fast iteration
        std::unordered_map<uint32_t, size_t>    object_id_to_index; // for O(1) access

        bool                is_up_to_date   = false;
        UnifiedRenderer*    owner           = nullptr;
        friend UnifiedRenderer;
    };

    struct RenderPassInfo {
        
        using program_t             = std::shared_ptr<Program>;
        using varianted_program_t   = std::shared_ptr<VariantedProgram>;

        void                set_render_layer(uint32_t render_layer_id);
        uint32_t            get_render_layer();

        uint32_t            get_required_attributes_mask();
        void                require_attribute(int32_t location, GenericType generic_type, std::string attribute_name, bool flat = false);
        bool                is_mesh_compatible(uint32_t mesh_mask);

        void                set_parameters(RenderParameters new_parameters);
        RenderParameters    get_parameters();

        bool                is_shader_set();
        void                set_shader(Shader shader);
        program_t           get_program();


    private:
        struct AttributeInfo {
            std::string variable_name;
            GenericType generic_type;
            bool is_flat = false;
        };
        std::array<AttributeInfo, 16> attribute_infos;
        uint32_t            required_attributes_mask = 0;
        uint32_t            render_layer        = invalid_id;
        varianted_program_t varianted_program   = nullptr;
        RenderParameters    parameters          = RenderParameters(true);
        bool                is_up_to_date       = false;
        UnifiedRenderer*    owner               = nullptr;
        friend UnifiedRenderer;
    };

// private: for testing purposes
    
    struct DrawElementsIndirectCommand {
        uint32_t    count;
        uint32_t    instanceCount;
        uint32_t    firstIndex;
        int32_t     baseVertex;
        uint32_t    baseInstance;
    };

    UnifiedRenderer() = default;

    struct HandleGenerator {
    public:
        uint32_t    next_id_to_generate = 0;
        uint32_t    generate_id();
        void        free_id(uint32_t old_id);

    private:
        std::vector<uint32_t>           free_ids;
        std::unordered_set<uint32_t>    free_id_set; // to prevent multiple free-ing
    };

    HandleGenerator         render_pass_handle_generator;
    HandleGenerator         render_layer_handle_generator;
    HandleGenerator         texture_handle_generator;
    HandleGenerator         vertex_format_handle_generator;
    HandleGenerator         mesh_handle_generator;
    HandleGenerator         object_handle_generator;
    HandleGenerator         material_handle_generator;

    struct TrackedDynamicBuffer {
        DynamicBuffer       buffer;
        size_t              last_updated_epoch = 0;
    };

    TrackedDynamicBuffer    unified_vertex_buffer;    // holds every mesh, each may have different attributes
    TrackedDynamicBuffer    unified_index_buffer;     // holds every mesh, each may be of different primitive type
    ResizableBuffer         vertex_format_buffer;     // holds vertex attribute 
    ResizableBuffer         mesh_buffer;              // holds vertex/index begin and size, attribute format, aabb of each mesh
    ResizableBuffer         object_buffer;            // holds mesh, aabb (redudent for faster access), transform

    TrackedDynamicBuffer    texture_buffer;           // holds handle for every bindless texture
    ResizableBuffer         material_buffer;          // holds array of texture handles and colors

    // each render_pass holds its object and material pairs and indirect command buffer.
    std::unordered_map<uint32_t, ResizableBuffer>   object_material_buffer;
    std::unordered_map<uint32_t, ResizableBuffer>   command_buffer;

    std::unordered_map<uint32_t, MeshInfo>          meshes;
    std::unordered_map<uint32_t, ObjectInfo>        objects;
    std::unordered_map<uint32_t, TextureInfo>       textures;
    std::unordered_map<uint32_t, MaterialInfo>      materials;
    std::unordered_map<uint32_t, RenderLayerInfo>   layers;
    std::unordered_map<uint32_t, RenderPassInfo>    passes;

    std::unordered_map<VertexFormatData, uint32_t, VertexFormatHasher>  format_to_id;

    bool a_mesh_modified            = true;
    bool a_texture_modified         = true;
    bool a_material_modified        = true;
    bool a_transform_modified       = true;
    bool an_object_modified         = true;
    bool a_render_layer_modified    = true;
    bool a_render_pass_modified     = true;

    void update_parent_object_matricies();
    void update_meshes();
    void update_materials();
    void update_objects();
    void update_render_passes();
    void update_structures();

    void compile_shaders();
    bool are_shaders_compiled = false;

    void compute_mesh_aabbs();
    void generate_command_buffer(uint32_t render_pass, const glm::mat4& projection_view_matrix, RenderOrder order = unordered);

    ResizableBuffer         mesh_job_buffer;
    std::shared_ptr<Buffer> draw_command_counter;
    VertexAttributeBuffer   blank_vab;

    std::shared_ptr<ComputeProgram> cp_compute_aabb;
    std::shared_ptr<ComputeProgram> cp_cull_and_generate_draw_commands;

};