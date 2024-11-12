#include "AttributedVertexBuffer.h"

AttributedVertexBuffer::AttributedVertexBuffer() : 
    _max_attribute_count(get_max_attribute_count())
{
    _vertex_buffers.resize(_max_attribute_count);
    _generate_buffer();
}

AttributedVertexBuffer::~AttributedVertexBuffer()
{
    release();
}

void AttributedVertexBuffer::release()
{
    if (_buffer_generated) {
        GLCall(glDeleteBuffers(1, &id));
    }

    _buffer_generated = false;
}

int32_t AttributedVertexBuffer::get_max_attribute_count()
{
    int32_t value;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
    return value;
}

int32_t AttributedVertexBuffer::get_largest_active_buffer_slot()
{
    size_t largest_buffer_size_by_bytes = 0;
    int32_t largest_slot = -1;
    uint32_t slot;
    for (slot = 0; slot < _max_attribute_count; slot++) {
        if (_vertex_buffers[slot]._slot_enabled && _vertex_buffers[slot]._buffer->get_buffer_size_in_bytes() > largest_buffer_size_by_bytes)
            largest_slot = slot;
    }
    return largest_slot;
}

void AttributedVertexBuffer::bind()
{
    if (!_buffer_generated) {
        std::cout << "[OpenGL Error] released AttributedVertexBuffer tried to bind()" << std::endl;
        ASSERT(false);
    }

    glBindVertexArray(id);
}

void AttributedVertexBuffer::unbind()
{
    GLCall(glBindVertexArray(0));
}

namespace {

    uint32_t _attribute_type_to_GL_type(AttributedVertexBuffer::AttributeType type) {
        switch (type) {
        case AttributedVertexBuffer::AttributeType::f16_fixed                  : return GL_HALF_FLOAT;
        case AttributedVertexBuffer::AttributeType::f16                        : return GL_FIXED;
        case AttributedVertexBuffer::AttributeType::f32                        : return GL_FLOAT;
        case AttributedVertexBuffer::AttributeType::f64                        : return GL_DOUBLE;
        case AttributedVertexBuffer::AttributeType::i8                         : return GL_BYTE;
        case AttributedVertexBuffer::AttributeType::ui8                        : return GL_UNSIGNED_BYTE;
        case AttributedVertexBuffer::AttributeType::i16                        : return GL_SHORT;
        case AttributedVertexBuffer::AttributeType::ui16                       : return GL_UNSIGNED_SHORT;
        case AttributedVertexBuffer::AttributeType::i32                        : return GL_INT;
        case AttributedVertexBuffer::AttributeType::ui32                       : return GL_UNSIGNED_INT;
        case AttributedVertexBuffer::AttributeType::i_2_10_10_10               : return GL_INT_2_10_10_10_REV;
        case AttributedVertexBuffer::AttributeType::ui_2_10_10_10              : return GL_UNSIGNED_INT_2_10_10_10_REV;
        case AttributedVertexBuffer::AttributeType::i8_normalized              : return GL_BYTE;
        case AttributedVertexBuffer::AttributeType::ui8_normalized             : return GL_UNSIGNED_BYTE;
        case AttributedVertexBuffer::AttributeType::i16_normalized             : return GL_SHORT;
        case AttributedVertexBuffer::AttributeType::ui16_normalized            : return GL_UNSIGNED_SHORT;
        case AttributedVertexBuffer::AttributeType::i32_normalized             : return GL_INT;
        case AttributedVertexBuffer::AttributeType::ui32_normalized            : return GL_UNSIGNED_INT;
        case AttributedVertexBuffer::AttributeType::i_2_10_10_10_normalized    : return GL_INT_2_10_10_10_REV;
        case AttributedVertexBuffer::AttributeType::ui_2_10_10_10_normalized   : return GL_UNSIGNED_INT_2_10_10_10_REV;
        case AttributedVertexBuffer::AttributeType::ui_10f_11f_11f             : return GL_UNSIGNED_INT_10F_11F_11F_REV;
        }
    } 

    uint32_t _attribute_type_to_GL_normalized_flag(AttributedVertexBuffer::AttributeType type) {
        switch (type) {
        case AttributedVertexBuffer::AttributeType::f16_fixed                  : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::f16                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::f32                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::f64                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::i8                         : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::ui8                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::i16                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::ui16                       : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::i32                        : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::ui32                       : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::i_2_10_10_10               : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::ui_2_10_10_10              : return GL_FALSE;
        case AttributedVertexBuffer::AttributeType::i8_normalized              : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::ui8_normalized             : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::i16_normalized             : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::ui16_normalized            : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::i32_normalized             : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::ui32_normalized            : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::i_2_10_10_10_normalized    : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::ui_2_10_10_10_normalized   : return GL_TRUE;
        case AttributedVertexBuffer::AttributeType::ui_10f_11f_11f             : return GL_FALSE;
        }
    } 

}

void AttributedVertexBuffer::attach_vertex_buffer(int32_t slot, std::shared_ptr<Buffer> vertex_buffer, size_t stride, size_t offset)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glVertexArrayVertexBuffer(id, slot, vertex_buffer->id, offset, stride));

    _vertex_buffers[slot]._buffer = vertex_buffer;
    _vertex_buffers[slot]._stride = stride;
    _vertex_buffers[slot]._offset = offset;
}

void AttributedVertexBuffer::attach_vertex_buffer(int32_t slot, std::shared_ptr<Buffer> vertex_buffer, AttributeType attribute_type, int32_t element_per_vertex, size_t stride, size_t offset, bool enabled)
{
    attach_vertex_buffer(slot, vertex_buffer, stride, offset);
    set_attribute_format(slot, attribute_type, element_per_vertex, offset);
    if (enabled) enable_attribute(slot);
    else disable_attribute(slot);
}

void AttributedVertexBuffer::set_attribute_format(int32_t slot, AttributeType attribute_type, int32_t element_per_vertex, size_t offset)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glVertexArrayAttribFormat(id, slot, element_per_vertex, _attribute_type_to_GL_type(attribute_type), _attribute_type_to_GL_normalized_flag(attribute_type), offset));

    _vertex_buffers[slot]._attribute_type = attribute_type;
    _vertex_buffers[slot]._element_per_vertex = element_per_vertex;
    _vertex_buffers[slot]._offset = offset;
}

void AttributedVertexBuffer::detach_vertex_buffer(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glVertexArrayVertexBuffer(id, slot, 0, 0, 0));
    
    _buffer_with_structure_info bufferless;
    bufferless._slot_enabled = _vertex_buffers[slot]._slot_enabled;
    _vertex_buffers[slot]._buffer = nullptr;
    _vertex_buffers[slot]._stride = 0;
    _vertex_buffers[slot]._offset = 0;
    _vertex_buffers[slot] = bufferless;
}

std::shared_ptr<Buffer> AttributedVertexBuffer::get_vertex_buffer(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._buffer;
}

AttributedVertexBuffer::AttributeType AttributedVertexBuffer::get_attribute_type(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._attribute_type;
}

int32_t AttributedVertexBuffer::get_attribute_element_per_vertex(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._element_per_vertex;
}

size_t AttributedVertexBuffer::get_attribute_stride(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._stride;
}

size_t AttributedVertexBuffer::get_attribute_offset(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._offset;
}

void AttributedVertexBuffer::enable_attribute(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }
    
    GLCall(glEnableVertexArrayAttrib(id, slot));

    _vertex_buffers[slot]._slot_enabled = true;
}

void AttributedVertexBuffer::disable_attribute(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    GLCall(glDisableVertexArrayAttrib(id, slot));

    _vertex_buffers[slot]._slot_enabled = false;
}

bool AttributedVertexBuffer::is_attribute_enabled(int32_t slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    return _vertex_buffers[slot]._slot_enabled;
}

void AttributedVertexBuffer::enabled_all_attached_attributes()
{
    for (int32_t slot = 0; slot < get_max_attribute_count(); slot++)
        enable_attribute(slot);
}

void AttributedVertexBuffer::disable_all_attributes()
{
    for (int32_t slot = 0; slot < get_max_attribute_count(); slot++)
        disable_attribute(slot);
}

void AttributedVertexBuffer::_generate_buffer()
{
    if (_buffer_generated) return;
    GLCall(glCreateVertexArrays(1, &id));
    _buffer_generated = true;
}

AttributedVertexBuffer::_buffer_with_structure_info::_buffer_with_structure_info(std::shared_ptr<Buffer> buffer, AttributeType attribute_type, int32_t element_per_vertex, size_t stride, size_t offset, bool is_enabled) :
    _buffer(buffer), _attribute_type(attribute_type), _element_per_vertex(element_per_vertex), _stride(stride), _offset(offset), _slot_enabled(is_enabled) {}


