#include "Graphic.h"

#include "Default_Assets.h"
#include "PhysicsGeometry.h"

#include <iostream>

Graphic::Graphic() :
	model_matrix(glm::mat4(1.0f)), vertex_buffer(ArrayBuffer()), index_buffer(IndexBuffer()), physics_representation(PhysicsObject(create_geometry::box(0.5f, 0.5f, 0.5f))) {}

Graphic::Graphic(const ArrayBuffer& buffer, const IndexBuffer& indicies, Material& mat, Program& program) : 
	model_matrix(glm::mat4(1.0f)), vertex_buffer(buffer), index_buffer(indicies), material(&mat), renderer(&program), physics_representation(PhysicsObject(create_geometry::box(0.5f, 0.5f, 0.5f))) {}

Graphic::Graphic(const std::vector<float>& verticies, int data_dim = 2) :
	physics_representation(PhysicsObject(create_geometry::box(0.5f, 0.5f, 0.5f)))
{	// legacy
	std::vector<unsigned int> triangles;
	for (unsigned int i = 1; i < verticies.size() / data_dim - 1; i++) {

		triangles.push_back(0);
		triangles.push_back(i);
		triangles.push_back(i + 1);
	}
	for (auto data : verticies)
		std::cout << data << " ";
	std::cout << "\n";
	for (auto data : triangles)
		std::cout << data << " ";

	// data dim is no longer passed in constructer
	ArrayBuffer array_buffer(verticies/*, data dim*/);
	IndexBuffer index_buffer(triangles, 3);

	this->vertex_buffer = array_buffer;
	this->index_buffer = index_buffer;
}

Graphic::Graphic(Material& material, Program& renderer):
	renderer(&renderer), material(&material), model_matrix(glm::mat4(1.0f)), physics_representation(PhysicsObject(create_geometry::box(0.5f, 0.5f, 0.5f))) {}

void Graphic::draw(bool show_warnings, bool _ignore_default_uniforms) {
	bool material_exist = true;
	bool renderer_exist = true;
	if (material == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] material is not specified for Graphic.draw()" << std::endl;
		material_exist = false;
	}
	if (renderer == nullptr) {
		if (show_warnings)
			std::cout << "[Opengl Warning] renderer is not specified for Graphic.draw()" << std::endl;
		renderer_exist = false;
	}

	if (renderer_exist)
		renderer->bind();
	vertex_buffer.bind();
	index_buffer.bind();
	
	if (material_exist)
 		material->bind();

	
	GLCall(glDrawElements(mode, index_buffer.data_count, GL_UNSIGNED_INT, nullptr));
}


void Graphic::update_matrix() {
	if (_last_updated_position == position && _last_updated_rotation == rotation)
		return;
	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x * 180 / physx::PxPi), glm::vec3(1.0f, 0.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y * 180 / physx::PxPi), glm::vec3(0.0f, 1.0f, 0.0f));
	model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z * 180 / physx::PxPi), glm::vec3(0.0f, 0.0f, 1.0f));
	model_matrix = glm::translate(model_matrix, (glm::vec3)(glm::vec4(position.x, position.y, position.z, 0) * model_matrix));
	_last_updated_position = position;
	_last_updated_rotation = rotation;
}

Assimp::Importer Graphic::asset_loader;

void Graphic::load_model(const std::string& file_path, bool use_for_physics) {
	const aiScene* imported_scene = asset_loader.ReadFile(file_path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals);

	std::cout << asset_loader.GetErrorString();

	int vertex_count = 0;
	int index_count = 0;
	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		vertex_count += imported_scene->mMeshes[i]->mNumVertices;
		index_count += imported_scene->mMeshes[i]->mNumFaces;
	}
	index_count *= 3; // if all faces are triangles!

	std::vector<float> vertex_data;
	std::vector<unsigned int> indicies;
	
	std::vector<physx::PxVec3> verticies_physics;

	unsigned int prefix_indicies_sum = 0;

	for (int i = 0; i < imported_scene->mNumMeshes; i++) {
		for (int j = 0; j < imported_scene->mMeshes[i]->mNumVertices; j++) {

			aiVector3D vertex = imported_scene->mMeshes[i]->mVertices[j];
			vertex_data.push_back((float)vertex.x);
			vertex_data.push_back((float)vertex.y);
			vertex_data.push_back((float)vertex.z);

			aiVector3D texcoords = imported_scene->mMeshes[i]->mTextureCoords[0][j];
			vertex_data.push_back(texcoords.x);
			vertex_data.push_back(texcoords.y);

			aiVector3D normal = imported_scene->mMeshes[i]->mNormals[j];
			normal.Normalize();
			vertex_data.push_back((float)normal.x);
			vertex_data.push_back((float)normal.y);
			vertex_data.push_back((float)normal.z);
		
			if (use_for_physics)
				verticies_physics.push_back(physx::PxVec3((float)vertex.x, (float)vertex.y, (float)vertex.z));
		}
		for (int j = 0; j < imported_scene->mMeshes[i]->mNumFaces; j++) {
			const aiFace& Face = imported_scene->mMeshes[i]->mFaces[j];
			
			indicies.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[0]);
			indicies.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[1]);
			indicies.push_back(prefix_indicies_sum + (unsigned int)Face.mIndices[2]);

		}
		prefix_indicies_sum += imported_scene->mMeshes[i]->mNumVertices;
	}
	vertex_buffer.vertex_attribute_structure.clear();
	vertex_buffer.initialize_buffer(vertex_data);
	vertex_buffer.push_attribute(3);	// position
	vertex_buffer.push_attribute(2);	// texture
	vertex_buffer.push_attribute(3);	// normals

	index_buffer.initialize_buffer(indicies, 3);

	if (use_for_physics) {
		physics_representation = PhysicsObject(create_geometry::convex_hull(verticies_physics, 30));
	}
}

void Graphic::clear_mesh() {
	vertex_buffer = ArrayBuffer();
	index_buffer = IndexBuffer();
}

void Graphic::sync_with_physics() {
	rotation.x = physics_representation.get_rotation().x;
	rotation.y = physics_representation.get_rotation().y;
	rotation.z = physics_representation.get_rotation().z;
	position.x = physics_representation.get_position().x;
	position.y = physics_representation.get_position().y;
	position.z = physics_representation.get_position().z;
}

glm::vec3 Graphic::get_position() {
	return position;
}
glm::vec3 Graphic::get_rotation() {
	return rotation;
}

void Graphic::set_position(glm::vec3 position) {
	this->position = position;
	physics_representation.set_position(position);
}

void Graphic::set_rotation(glm::vec3 rotation) {
	this->rotation = rotation;
	physics_representation.set_rotation(rotation);
}

void Graphic::update_uniform_queue(bool init) {
	_uniform_update_queue.update_uniforms();
}