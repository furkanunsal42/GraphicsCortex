#include "Mesh.h"

void Mesh::load_model(const SingleModel& single_model, IndexType type)
{
	clear();

	vab = single_model.create_vertex_attribute_buffer();
	index_buffer = single_model.create_index_buffer(type, 0);
	//index_buffer_type = type;

	SingleMesh mesh_definition;

	mesh_definition.index_count		= index_buffer->get_buffer_size_in_bytes() / get_IndexType_bytes_per_index(type);
	mesh_definition.index_offset	= 0;

	mesh_definition.vertex_count	= vab->get_min_vertex_count();
	mesh_definition.vertex_offset	= 0;

	mesh_definition.index_type = type;
	mesh_definition.primitive = single_model.primitive;

	mesh_t mesh = add_mesh(mesh_definition);
	node_t root = add_node(null_node_name);
	
	get_node(root)->add_submesh(mesh);
}

void Mesh::set_index_buffer(std::shared_ptr<Buffer> index_buffer)
{
	this->index_buffer = index_buffer;
}

void Mesh::set_vertex_attribute_buffer(std::shared_ptr<VertexAttributeBuffer> vertex_attribute_buffer)
{
	vab = vertex_attribute_buffer;
}

std::shared_ptr<Buffer> Mesh::get_index_buffer()
{
	return index_buffer;
}

std::shared_ptr<VertexAttributeBuffer> Mesh::get_vertex_attribute_buffer()
{
	return vab;
}

void Mesh::clear()
{
	next_node_name = root_node_name;
	vab = nullptr;
	index_buffer = nullptr;
	single_meshes.clear();
	name_to_nodes.clear();
	//index_buffer_type = IndexType::i_ui32;
}


bool Mesh::does_mesh_exist(mesh_t submesh_name){
	return submesh_name >= 0 && submesh_name < single_meshes.size();
}

size_t Mesh::get_mesh_count() {
	return single_meshes.size();
}

mesh_t Mesh::add_mesh(SingleMesh mesh_definition)
{
	mesh_definition.owner = this;
	single_meshes.push_back(mesh_definition);
	return single_meshes.size() - 1;
}

mesh_t Mesh::add_mesh(SingleMesh&& mesh_definition)
{
	mesh_definition.owner = this;
	single_meshes.emplace_back(mesh_definition);
	return single_meshes.size() - 1;
}

Mesh::SingleMesh* Mesh::get_mesh(mesh_t submesh_name)
{
	if (!does_mesh_exist(submesh_name))
		return nullptr;

	return &single_meshes[submesh_name];
}

std::span<Mesh::SingleMesh> Mesh::get_meshes()
{
	return std::span<SingleMesh>(single_meshes);
}

void Mesh::clear_meshes()
{
	vab = nullptr;
	index_buffer = nullptr;
	single_meshes.clear();
	//index_buffer_type = IndexType::i_ui32;
	for (std::pair<const node_t, Mesh::Node>& pair : name_to_nodes) {
		Mesh::Node& node = pair.second;
		node.children.clear();
	}
}

//void Mesh::set_index_type(IndexType type)
//{
//	index_buffer_type = type;
//}
//
//IndexType Mesh::get_index_type()
//{
//	return index_buffer_type;
//}

size_t Mesh::get_node_count()
{
	return name_to_nodes.size();
}

bool Mesh::does_node_exist(node_t node_name)
{
	return (name_to_nodes.find(node_name) != name_to_nodes.end());
}

node_t Mesh::add_node(node_t parent)
{
	if (name_to_nodes.find(parent) == name_to_nodes.end() && parent != null_node_name) {
		std::cout << "[Mesh Error] Mesh::add_node() is called with a parent node that doesn't exist" << std::endl;
		ASSERT(false);
	}

	node_t name = generate_node_name();
	name_to_nodes[name] = Node(this, name, parent);

	if (parent != null_node_name)
		name_to_nodes[parent].add_child(name);

	return name;
}

Mesh::Node* Mesh::get_node(node_t node_name)
{
	auto iterator = name_to_nodes.find(node_name);
	if (iterator == name_to_nodes.end())
		return nullptr;

	return &name_to_nodes.at(node_name);
}

Mesh::Node& Mesh::operator[](node_t node_name)
{
	Node* node = get_node(node_name);
	if (node == nullptr) {
		std::cout << "[Mesh Error] Mesh::operator[]() is called with a node name that doesn't exist: " << node_name << std::endl;
		ASSERT(false);
	}

	return *get_node(node_name);
}

void Mesh::clear_nodes()
{
	next_node_name = root_node_name;
	name_to_nodes.clear();
}

node_t Mesh::generate_node_name()
{
	node_t name = next_node_name;
	next_node_name++;
	return name;
}


Mesh::Node::Node(Mesh* owner, uint32_t node_name, node_t parent, glm::mat4 transform) :
	owner(owner), name(node_name), parent(parent), transform(transform)
{
	if (owner == nullptr) {
		std::cout << "[Mesh Error] Mesh::Node is initialized with null referance to it's Mesh" << std::endl;
		ASSERT(false);
	}
}

node_t Mesh::Node::get_name()
{
	return name;
}

node_t Mesh::Node::get_parent()
{
	return parent;
}

void Mesh::Node::set_parent(node_t parent)
{
	this->parent = parent;
}

std::span<node_t> Mesh::Node::get_children()
{
	return std::span<node_t>(children);
}

void Mesh::Node::add_child(node_t child)
{
	if (!owner->does_node_exist(child)) {
		std::cout << "[Mesh Error] Mesh::Node::add_child() is called with a child that doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (std::find(children.begin(), children.end(), child) == children.end()) {
		children.push_back(child);
	}
}

node_t Mesh::Node::create_child()
{
	return owner->add_node(name);
}

std::span<model_t> Mesh::Node::get_submeshes()
{
	return std::span<model_t>(submeshes);
}

void Mesh::Node::add_submesh(mesh_t submesh)
{
	if (!owner->does_mesh_exist(submesh)) {
		std::cout << "[Mesh Error] Mesh::Node::add_submeshes() is called with a submesh that doesn't exist" << std::endl;
		ASSERT(false);
	}

	submeshes.push_back(submesh);
}

glm::mat4 Mesh::Node::get_transform()
{
	return transform;
}

void Mesh::Node::set_transform(glm::mat4 transform)
{
	this->transform = transform;
}

Mesh* Mesh::Node::get_mesh()
{
	return owner;
}

Mesh* Mesh::SingleMesh::get_owner()
{
	return owner;
}
