#include "AttributedVertexBuffer.h"

AttributedVertexBuffer::AttributedVertexBuffer() : 
    _max_attribute_count(get_max_attribute_count())
{
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

int AttributedVertexBuffer::get_max_attribute_count()
{
    int value;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
    return value;
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

void AttributedVertexBuffer::attach_vertex_buffer(int slot, std::shared_ptr<Buffer> vertex_buffer, AttributeType attribute_type, int8_t element_per_vertex, size_t stride)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    _vertex_buffers[slot] = _buffer_with_slotinfo(vertex_buffer, attribute_type, element_per_vertex, stride, false);
}

void AttributedVertexBuffer::deattach_vertex_buffer(int slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    auto iterator = _vertex_buffers.find(slot);
    const bool info_exists = iterator != _vertex_buffers.end();

    if (!info_exists) return;
 
    const bool is_enabled = iterator->second._enabled;
    
    if (!is_enabled) 
        _vertex_buffers.erase(slot);
    else 
        iterator->second._buffer = nullptr;
}

std::shared_ptr<Buffer> AttributedVertexBuffer::get_vertex_buffer(int slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    auto iterator = _vertex_buffers.find(slot);
    if (iterator == _vertex_buffers.end()) 
        return nullptr;

    return iterator->second._buffer;
}

void AttributedVertexBuffer::attach_index_buffer(std::shared_ptr<Buffer> index_buffer)
{
    _index_buffer = index_buffer;
}

void AttributedVertexBuffer::deattach_index_buffer()
{
    _index_buffer = nullptr;
}

std::shared_ptr<Buffer> AttributedVertexBuffer::get_index_buffer()
{
    return _index_buffer;
}

void AttributedVertexBuffer::enable_attribute(int slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    _vertex_buffers[slot]._enabled = true;
}

void AttributedVertexBuffer::disable_attribute(int slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    auto iterator = _vertex_buffers.find(slot);
    const bool info_exists = iterator != _vertex_buffers.end();

    if (!info_exists) return;

    const bool is_buffer_attached = iterator->second._buffer != nullptr;

    if (!is_buffer_attached)
        _vertex_buffers.erase(slot);
    else
        iterator->second._buffer = nullptr;

    _vertex_buffers[slot]._enabled = false;
}

bool AttributedVertexBuffer::is_attribute_enabled(int slot)
{
    if (slot < 0 || slot >= _max_attribute_count) {
        std::cout << "[OpenGL Error] AttributedVertexBuffer tried to access slot: " << slot << " but only " << _max_attribute_count << " attributes are supported" << std::endl;
        ASSERT(false);
    }

    auto iterator = _vertex_buffers.find(slot);
    
    if (iterator == _vertex_buffers.end()) return false;
    
    return iterator->second._enabled;
}

void AttributedVertexBuffer::enabled_all_attached_attributes()
{
    for (auto& element: _vertex_buffers) {
        enable_attribute(element.first);
    }
}

void AttributedVertexBuffer::disable_all_attributes()
{
    for (auto& element : _vertex_buffers) {
        disable_attribute(element.first);
    }
}

void AttributedVertexBuffer::_generate_buffer()
{
    if (_buffer_generated) return;
    GLCall(glCreateBuffers(1, &id));
    _buffer_generated = true;
}

AttributedVertexBuffer::_buffer_with_slotinfo::_buffer_with_slotinfo(std::shared_ptr<Buffer> buffer, AttributeType attribute_type, int8_t element_per_vertex, size_t stride, bool is_enabled) :
    _buffer(buffer), _attribute_type(attribute_type), _element_per_vertex(element_per_vertex), _stride(stride), _enabled(is_enabled) {}


