#include "GL/glew.h"
#include "VertexAttributeBuffer.h"
#include "Debuger.h"

VertexAttributeBuffer::VertexAttributeBuffer()
{
    clear();
    generate_buffer();
}

VertexAttributeBuffer::~VertexAttributeBuffer()
{
    release();
}

void VertexAttributeBuffer::release()
{
    if (buffer_generated) {
        GLCall(glDeleteVertexArrays(1, &id));
    }

    buffer_generated = false;
}

void VertexAttributeBuffer::clear()
{
    vertex_buffers.clear();
    attributes.clear();

    int32_t attribute_slot_count = get_max_attribute_count();
    int32_t buffer_slot_count = get_max_buffer_count();

    vertex_buffers.resize(buffer_slot_count);
    attributes.resize(attribute_slot_count);

    is_validity_checked = false;
}

size_t VertexAttributeBuffer::get_max_vertex_count()
{
    size_t max_vertex_count = 0;
    for (int i = 0; i < attributes.size(); i++) {
        if (!is_attribute_enabled(i)) continue;

        max_vertex_count = std::max(max_vertex_count, get_vertex_count_of_attribute(i));
    }

    return max_vertex_count;
}

size_t VertexAttributeBuffer::get_min_vertex_count()
{
    size_t min_vertex_count = -1;
    for (int i = 0; i < attributes.size(); i++) {
        if (!is_attribute_enabled(i)) continue;

        min_vertex_count = std::min(min_vertex_count, get_vertex_count_of_attribute(i));
    }

    if (min_vertex_count == -1) min_vertex_count = 0;

    return min_vertex_count;
}

int32_t VertexAttributeBuffer::get_max_buffer_count()
{
    int32_t value;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &value));
    return value;
}

int32_t VertexAttributeBuffer::get_max_attribute_count()
{
    int32_t value;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
    return value;
}


buffer_slot_t VertexAttributeBuffer::get_largest_buffer_slot()
{
    size_t max_vertex_count = 0;
    buffer_slot_t max_slot = null_buffer_slot;
    
    for (buffer_slot_t buffer_slot = 0; buffer_slot < vertex_buffers.size(); buffer_slot++) {
        
        size_t vertex_count = get_vertex_count_of_buffer(buffer_slot);

        if (vertex_count >= max_vertex_count) {
            max_slot = buffer_slot;
            max_vertex_count = vertex_count;
        }
    }

    return max_slot;
}

buffer_slot_t VertexAttributeBuffer::get_smallest_buffer_slot()
{
    size_t min_vertex_count = -1;
    buffer_slot_t min_slot = null_buffer_slot;

    for (buffer_slot_t buffer_slot = 0; buffer_slot < vertex_buffers.size(); buffer_slot++) {

        size_t vertex_count = get_vertex_count_of_buffer(buffer_slot);

        if (vertex_count <= min_vertex_count) {
            min_slot = buffer_slot;
            min_vertex_count = vertex_count;
        }
    }
    
    if (min_vertex_count == -1) min_vertex_count = 0;

    return min_slot;
}

attribute_slot_t VertexAttributeBuffer::get_largest_active_attribute_slot()
{
    size_t max_vertex_count = 0;
    attribute_slot_t max_slot = null_attribute_slot;

    for (attribute_slot_t attribute_slot = 0; attribute_slot < attributes.size(); attribute_slot++) {
        if (!is_attribute_enabled(attribute_slot)) continue;

        size_t vertex_count = get_vertex_count_of_attribute(attribute_slot);

        if (vertex_count >= max_vertex_count) {
            max_slot = attribute_slot;
            max_vertex_count = vertex_count;
        }
    }

    return max_slot;
}

attribute_slot_t VertexAttributeBuffer::get_smallest_active_attribute_slot()
{
    size_t min_vertex_count = -1;
    attribute_slot_t min_slot = null_attribute_slot;

    for (attribute_slot_t attribute_slot = 0; attribute_slot < attributes.size(); attribute_slot++) {
        if (!is_attribute_enabled(attribute_slot)) continue;

        size_t vertex_count = get_vertex_count_of_attribute(attribute_slot);

        if (vertex_count <= min_vertex_count) {
            min_slot = attribute_slot;
            min_vertex_count = vertex_count;
        }
    }

    if (min_vertex_count == -1) min_vertex_count = 0;

    return min_slot;
}

void VertexAttributeBuffer::bind()
{
    if (!is_validity_checked) {
        bool is_valid = check_for_validity();
        is_validity_checked = true;
    }

    if (!buffer_generated) {
        std::cout << "[OpenGL Error] released VertexAttributeBuffer tried to bind()" << std::endl;
        ASSERT(false);
    }

    GLCall(glBindVertexArray(id));
}

void VertexAttributeBuffer::unbind()
{
    GLCall(glBindVertexArray(0));
}

void VertexAttributeBuffer::attach_vertex_buffer(buffer_slot_t buffer_slot, std::shared_ptr<Buffer> vertex_buffer, size_t stride_in_bytes, size_t buffer_offset_in_bytes, uint32_t divisor_for_instancing)
{
    if (buffer_slot < 0 || buffer_slot >= get_max_buffer_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access buffer slot: " << buffer_slot << " but only " << get_max_buffer_count() << " vertex buffers are supported" << std::endl;
        ASSERT(false);
    }
    
    if (buffer_offset_in_bytes >= vertex_buffer->get_buffer_size_in_bytes()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer called attach_vertex_buffer() but given offset (" << buffer_offset_in_bytes << ") is greater than buffer size (" << vertex_buffer->get_buffer_size_in_bytes() << ")" << std::endl;
        ASSERT(false);
    }

    vertex_buffer->force_allocation();

    GLCall(glVertexArrayVertexBuffer(id, buffer_slot, vertex_buffer->id, buffer_offset_in_bytes, stride_in_bytes));
    GLCall(glVertexArrayBindingDivisor(id, buffer_slot, divisor_for_instancing));

    vertex_buffers[buffer_slot] = BufferSlotInfo(vertex_buffer, stride_in_bytes, buffer_offset_in_bytes, divisor_for_instancing);
}

void VertexAttributeBuffer::detach_vertex_buffer(buffer_slot_t buffer_slot)
{
    if (buffer_slot < 0 || buffer_slot >= get_max_buffer_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access buffer slot: " << buffer_slot << " but only " << get_max_buffer_count() << " vertex buffers are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glVertexArrayVertexBuffer(id, buffer_slot, 0, 0, 0));
    GLCall(glVertexArrayBindingDivisor(id, buffer_slot, 0));
    
    vertex_buffers[buffer_slot] = BufferSlotInfo(nullptr, 0, 0, 0);

    is_validity_checked = false;
}

void VertexAttributeBuffer::set_attribute_format(attribute_slot_t attribute_slot, buffer_slot_t buffer_slot, AttributeType attribute_type, uint32_t element_count_per_vertex, size_t relative_offset_in_bytes, bool enabled)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }
    
    if (buffer_slot < 0 || buffer_slot >= get_max_buffer_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access buffer slot: " << buffer_slot << " but only " << get_max_buffer_count() << " vertex buffers are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glVertexArrayAttribFormat(
        id, 
        attribute_slot, 
        element_count_per_vertex, 
        get_attribute_type_to_GL_type(attribute_type), 
        get_attribute_type_to_GL_normalized_flag(attribute_type), 
        relative_offset_in_bytes));

    GLCall(glVertexArrayAttribBinding(id, attribute_slot, buffer_slot));

    if (enabled) enable_attribute(attribute_slot);
    else disable_attribute(attribute_slot);

    attributes[attribute_slot] = AttributeSlotInfo(buffer_slot, attribute_type, element_count_per_vertex, relative_offset_in_bytes, enabled);

    is_validity_checked = false;
}

VertexAttributeBuffer::BufferSlotInfo& VertexAttributeBuffer::get_buffer_slot(buffer_slot_t buffer_slot)
{
    if (buffer_slot < 0 || buffer_slot >= get_max_buffer_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access buffer slot: " << buffer_slot << " but only " << get_max_buffer_count() << " vertex buffers are supported" << std::endl;
        ASSERT(false);
    }

    return vertex_buffers[buffer_slot];
}

VertexAttributeBuffer::AttributeSlotInfo& VertexAttributeBuffer::get_attribute_slot(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return attributes[attribute_slot];
}

void VertexAttributeBuffer::enable_attribute(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glEnableVertexArrayAttrib(id, attribute_slot));

    attributes[attribute_slot].slot_enabled = true;
}

void VertexAttributeBuffer::disable_attribute(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glDisableVertexArrayAttrib(id, attribute_slot));

    attributes[attribute_slot].slot_enabled = false;
}

bool VertexAttributeBuffer::is_attribute_enabled(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return attributes[attribute_slot].slot_enabled;
}

void VertexAttributeBuffer::enabled_all_attached_attributes()
{
    for (int32_t slot = 0; slot < get_max_attribute_count(); slot++)
        enable_attribute(slot);
}

void VertexAttributeBuffer::disable_all_attributes()
{
    for (int32_t slot = 0; slot < get_max_attribute_count(); slot++)
        disable_attribute(slot);
}

uint32_t VertexAttributeBuffer::get_vertex_buffer_count()
{
    uint32_t count = 0;
    for (auto& buffer_slot : vertex_buffers)
        if (buffer_slot.buffer != nullptr)
            count++;
    return count;
}

uint32_t VertexAttributeBuffer::get_active_attribute_count()
{
    uint32_t count = 0;
    for (auto& attribute_slot : attributes)
        if (attribute_slot.slot_enabled)
            count++;
    return count;
}

size_t VertexAttributeBuffer::get_vertex_count_of_buffer(buffer_slot_t buffer_slot)
{
    if (buffer_slot < 0 || buffer_slot >= get_max_buffer_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access buffer slot: " << buffer_slot << " but only " << get_max_buffer_count() << " vertex buffers are supported" << std::endl;
        ASSERT(false);
    }

    BufferSlotInfo& slot = get_buffer_slot(buffer_slot);

    std::shared_ptr<Buffer>& buffer = slot.buffer;
    if (buffer == nullptr) return 0;
    if (slot.stride_in_bytes == 0) return 0;

    size_t vertex_count = (buffer->get_buffer_size_in_bytes() - slot.offset_in_bytes) / slot.stride_in_bytes;
    
    return vertex_count;
}

size_t VertexAttributeBuffer::get_element_count_of_attribute(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    AttributeSlotInfo& attribute = get_attribute_slot(attribute_slot);
    if (attribute.buffer_slot == null_buffer_slot) return 0;

    BufferSlotInfo& slot = get_buffer_slot(attribute.buffer_slot);

    std::shared_ptr<Buffer>& buffer = slot.buffer;
    if (buffer == nullptr) return 0;
    if (slot.stride_in_bytes == 0) return 0;

    size_t vertex_count = (buffer->get_buffer_size_in_bytes() - slot.offset_in_bytes) / slot.stride_in_bytes;
    
    if (attribute.element_count_per_vertex == 0) return 0;
    size_t element_count = vertex_count * attribute.element_count_per_vertex;

    return element_count;
}

size_t VertexAttributeBuffer::get_vertex_count_of_attribute(attribute_slot_t attribute_slot)
{
    if (attribute_slot < 0 || attribute_slot >= get_max_attribute_count()) {
        std::cout << "[OpenGL Error] VertexAttributeBuffer tried to access attribute slot: " << attribute_slot << " but only " << get_max_attribute_count() << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    AttributeSlotInfo& attribute = get_attribute_slot(attribute_slot);
    if (attribute.buffer_slot == null_buffer_slot) return 0;
    
    BufferSlotInfo& slot = get_buffer_slot(attribute.buffer_slot);
    std::shared_ptr<Buffer>& buffer = slot.buffer;
    
    if (buffer == nullptr) return 0;
    if (slot.stride_in_bytes == 0) return 0;
    
    size_t vertex_count = (buffer->get_buffer_size_in_bytes() - slot.offset_in_bytes) / slot.stride_in_bytes;
    
    return vertex_count;
}

bool VertexAttributeBuffer::check_for_validity()
{
    bool is_valid = true;
    
    for (attribute_slot_t attribute_slot = 0; attribute_slot < attributes.size(); attribute_slot++) {
        AttributeSlotInfo& attribute = attributes[attribute_slot];

        if (!attribute.slot_enabled) continue;

        if (attribute.element_count_per_vertex < 1 || attribute.element_count_per_vertex > 4) {
            std::cout << "[OpenGL Error] VertexAttributeBuffer validity check failed because attribute[" << attribute_slot << "] has element_count_per_vertex = " << attribute.element_count_per_vertex << " but it has to be between 1 and 4" << std::endl;
            ASSERT(false);
            is_valid = false;
            break;
        }

        buffer_slot_t buffer_slot = attribute.buffer_slot;
        if (buffer_slot == null_buffer_slot) {
            std::cout << "[OpenGL Error] VertexAttributeBuffer validity check failed because "
                         "attribute[" << attribute_slot << "] has buffer_slot = " << attribute.buffer_slot << " but "
                         "it has to be between 0 and " << attributes.size() << std::endl;
            ASSERT(false); 
            is_valid = false;
            break;
        }

        BufferSlotInfo& vertex_buffer = vertex_buffers[buffer_slot];

        // ? TEST
        //if (attribute.relative_offset_in_bytes >= vertex_buffer.stride_in_bytes) {
        //    is_valid = false;
        //    break;
        //}

        if (vertex_buffer.buffer == nullptr) {
            std::cout << "[OpenGL Error] VertexAttributeBuffer validity check failed because "
                         "vertex_buffer[" << buffer_slot << "] has a nullptr buffer" << std::endl;
            ASSERT(false);
            is_valid = false;
            break;
        }

        if (vertex_buffer.stride_in_bytes == 0) {
            std::cout << "[OpenGL Error] VertexAttributeBuffer validity check failed because "
                         "vertex_buffer[" << buffer_slot << "] has stride_in_bytes = " << vertex_buffer.stride_in_bytes << " but "
                         "it has to be greater than 0" << std::endl;
            ASSERT(false);
            is_valid = false;
            break;
        }

        if (vertex_buffer.offset_in_bytes >= vertex_buffer.buffer->get_buffer_size_in_bytes()) {
            std::cout << "[OpenGL Error] VertexAttributeBuffer validity check failed because "
                         "vertex_buffer[" << buffer_slot << "] has offset_in_bytes = " << vertex_buffer.offset_in_bytes << " but "
                         "it has to be less than it's buffer size (" << vertex_buffer.buffer->get_buffer_size_in_bytes() << ")" << std::endl;
            ASSERT(false);
            is_valid = false;
            break;
        }
    }
    return is_valid;
}

void VertexAttributeBuffer::generate_buffer()
{
    if (buffer_generated) return;
    GLCall(glCreateVertexArrays(1, &id));
    buffer_generated = true;
}


uint32_t VertexAttributeBuffer::get_attribute_type_to_GL_type(VertexAttributeBuffer::AttributeType type) {
    switch (type) {
    case VertexAttributeBuffer::AttributeType::a_f16_fixed: return GL_HALF_FLOAT;
    case VertexAttributeBuffer::AttributeType::a_f16: return GL_FIXED;
    case VertexAttributeBuffer::AttributeType::a_f32: return GL_FLOAT;
    case VertexAttributeBuffer::AttributeType::a_f64: return GL_DOUBLE;
    case VertexAttributeBuffer::AttributeType::a_i8: return GL_BYTE;
    case VertexAttributeBuffer::AttributeType::a_ui8: return GL_UNSIGNED_BYTE;
    case VertexAttributeBuffer::AttributeType::a_i16: return GL_SHORT;
    case VertexAttributeBuffer::AttributeType::a_ui16: return GL_UNSIGNED_SHORT;
    case VertexAttributeBuffer::AttributeType::a_i32: return GL_INT;
    case VertexAttributeBuffer::AttributeType::a_ui32: return GL_UNSIGNED_INT;
    case VertexAttributeBuffer::AttributeType::a_i_2_10_10_10: return GL_INT_2_10_10_10_REV;
    case VertexAttributeBuffer::AttributeType::a_ui_2_10_10_10: return GL_UNSIGNED_INT_2_10_10_10_REV;
    case VertexAttributeBuffer::AttributeType::a_i8_normalized: return GL_BYTE;
    case VertexAttributeBuffer::AttributeType::a_ui8_normalized: return GL_UNSIGNED_BYTE;
    case VertexAttributeBuffer::AttributeType::a_i16_normalized: return GL_SHORT;
    case VertexAttributeBuffer::AttributeType::a_ui16_normalized: return GL_UNSIGNED_SHORT;
    case VertexAttributeBuffer::AttributeType::a_i32_normalized: return GL_INT;
    case VertexAttributeBuffer::AttributeType::a_ui32_normalized: return GL_UNSIGNED_INT;
    case VertexAttributeBuffer::AttributeType::a_i_2_10_10_10_normalized: return GL_INT_2_10_10_10_REV;
    case VertexAttributeBuffer::AttributeType::a_ui_2_10_10_10_normalized: return GL_UNSIGNED_INT_2_10_10_10_REV;
    case VertexAttributeBuffer::AttributeType::a_ui_10f_11f_11f: return GL_UNSIGNED_INT_10F_11F_11F_REV;
    }
}

uint32_t VertexAttributeBuffer::get_attribute_type_to_GL_normalized_flag(VertexAttributeBuffer::AttributeType type) {
    switch (type) {
    case VertexAttributeBuffer::AttributeType::a_f16_fixed: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_f16: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_f32: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_f64: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_i8: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_ui8: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_i16: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_ui16: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_i32: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_ui32: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_i_2_10_10_10: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_ui_2_10_10_10: return GL_FALSE;
    case VertexAttributeBuffer::AttributeType::a_i8_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_ui8_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_i16_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_ui16_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_i32_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_ui32_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_i_2_10_10_10_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_ui_2_10_10_10_normalized: return GL_TRUE;
    case VertexAttributeBuffer::AttributeType::a_ui_10f_11f_11f: return GL_FALSE;
    }
}

VertexAttributeBuffer::BufferSlotInfo::BufferSlotInfo(std::shared_ptr<Buffer> buffer, size_t stride_in_bytes, size_t offset_in_bytes, size_t divisor) :
    buffer(buffer), stride_in_bytes(stride_in_bytes), offset_in_bytes(offset_in_bytes), divisor(divisor)
{
}

VertexAttributeBuffer::AttributeSlotInfo::AttributeSlotInfo(buffer_slot_t buffer_slot, AttributeType type, int32_t element_count_per_vertex, size_t relative_offset_in_bytes, bool slot_enabled) :
    buffer_slot(buffer_slot), type(type), element_count_per_vertex(element_count_per_vertex), relative_offset_in_bytes(relative_offset_in_bytes), slot_enabled(slot_enabled)
{
}
