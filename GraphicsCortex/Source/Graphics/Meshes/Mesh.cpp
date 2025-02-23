#include "Mesh.h"

void Mesh::set_index_buffer(std::shared_ptr<Buffer> index_buffer)
{
	_index_buffer = index_buffer;
}

void Mesh::set_vertex_attribute_buffer(std::shared_ptr<VertexAttributeBuffer> vertex_attribute_buffer)
{
	vab = vertex_attribute_buffer;
}

std::shared_ptr<Buffer> Mesh::get_index_buffer()
{
	return _index_buffer;
}

std::shared_ptr<VertexAttributeBuffer> Mesh::get_vertex_attribute_buffer()
{
	return vab;
}

size_t Mesh::get_submesh_count() {
	return _name_to_singlemesh.size();
}

bool Mesh::does_submesh_exist(uint32_t submesh_name){
	if (submesh_name == null_submesh_name) return false;

	return _name_to_singlemesh.find(submesh_name) != _name_to_singlemesh.end();
}

/*
uint32_t Mesh::insert_submeshes(const Model2& model){
	size_t offset = _vertex_size;
	size_t index_offset = _index_size;
	for (auto& pair : model._name_to_submodel) {
		size_t size = pair.second->read_verticies().size();
		size_t index_size = pair.second->read_indicies().size();
		SubmeshInfo submesh(offset, size, index_offset, index_size, pair.second->get_primitive());
		offset += size;
		index_offset += index_size;

		
	}
}

uint32_t Mesh::insert_submeshes(const Model2::_ProxyNode& submodel){
	
}

uint32_t Mesh::insert_submesh(const SingleModel2& single_model){
	
}

void Mesh::erase_submesh(uint32_t submesh_name){
	
}

void Mesh::erase_submesh(uint32_t submesh_name, size_t submesh_count){
	
}

Mesh::SubmeshInfo& Mesh::get_submesh(uint32_t submesh_name){
	
}

bool Mesh::set_submesh(uint32_t submodel_name, SubmeshInfo submesh){
	

}

Mesh::SubmeshInfo::SubmeshInfo(size_t vertex_buffers_offset, size_t vertex_buffers_size, size_t index_buffer_offset, size_t index_buffer_size, PrimitiveType primitive) :
	vertex_buffers_offset(vertex_buffers_offset), vertex_buffers_size(vertex_buffers_size), index_buffer_offset(index_buffer_offset), index_buffer_size(index_buffer_size), primitive(primitive) {}
	*/