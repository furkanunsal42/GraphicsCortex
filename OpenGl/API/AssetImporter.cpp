#include "AssetImporter.h"

Assimp::Importer AssetImporter::_assimp_asset_loader;
std::unordered_map<std::string, const aiScene*> AssetImporter::_imported_assets;

const aiScene* AssetImporter::readfile(const std::string& filename, unsigned int flags) {
	_assimp_asset_loader.ReadFile(filename, flags);
	const aiScene* imported_scene = _assimp_asset_loader.GetOrphanedScene();

	std::string load_error = _assimp_asset_loader.GetErrorString();
	if (load_error != "")
		std::cout << "[Assimp Loading Error]: " << load_error << '\n';

	_imported_assets.insert(std::pair<std::string, const aiScene*>(filename, imported_scene));
	return imported_scene;
}

namespace {

	std::string compute_directory(const std::string& file_name) { // "C:\folder\folder\file" -> "C:\folder\folder\"
		std::string dir = file_name;
		for (int i = dir.size() - 1; i >= 0; i--) {
			if (dir[i] != '/' && dir[i] != '\\')
				dir.pop_back();
			else
				break;
		}
		return dir;
	}

	std::string compute_filename(const std::string& file_name) { // "C:\folder\folder\file" -> "file"
		std::string name = "";
		for (int i = file_name.size() - 1; i >= 0; i--) {
			if (file_name[i] != '/' && file_name[i] != '\\')
				name = file_name[i] + name;
			else
				break;
		}
		return name;
	}

	std::vector<char> compute_enabled_bits(unsigned int binary_bits, int bit_count) {
		ASSERT(bit_count <= 32);
		std::vector<char> bits(32);
		int mask = 1;
		for (int i = 0; i < bit_count; i++) {
			if ((binary_bits & mask) == mask)
				bits[i] = true;
			else
				bits[i] = false;
			mask *= 2;
		}
		return bits;
	}

}

#define MAP_TYPE_COUNT 3
UnorderedMaterial AssetImporter::generate_material(const std::string& filename) {
	const aiScene* imported_scene;

	if (_imported_assets.find(filename) == _imported_assets.end()) {
		imported_scene = readfile(filename);
	}
	else {
		imported_scene = _imported_assets[filename];
	}
	
	std::vector<std::string> image_paths;
	std::string dir = compute_directory(filename);

	unsigned int map_types[MAP_TYPE_COUNT] = { aiTextureType_DIFFUSE, aiTextureType_METALNESS, aiTextureType_NORMALS };
	for (int i = 0; i < imported_scene->mNumMeshes; i++) {

		int map_indicies[MAP_TYPE_COUNT] = { -1, -1, -1 };
		int material_index = imported_scene->mMeshes[i]->mMaterialIndex;

		for (int j = 0; j < MAP_TYPE_COUNT; j++) {

			aiString image_name;

			if (imported_scene->mMaterials[material_index]->GetTexture((aiTextureType)map_types[j], 0, &image_name) != AI_SUCCESS) {
				map_indicies[j] = -1; // texture not found
				continue;
			}

			std::string path = dir + "textures/" + compute_filename(std::string(image_name.C_Str())); // compute texture's filename under "texture/" directory

			bool image_exists = false;
			for (int k = 0; k < image_paths.size(); k++) {
				if (image_paths[k] == path) {
					image_exists = true;
					map_indicies[j] = k;
					break;
				}
			}
			if (!image_exists) {
				map_indicies[j] = image_paths.size();
				image_paths.push_back(path);
			}
		}
	}

	UnorderedMaterial material(image_paths.size());
	for (int i = 0; i < image_paths.size(); i++) {
		material.set_texture(image_paths[i], 4, i);
	}
	return material;
}

Model AssetImporter::generate_model(const std::string& filename, float scale, unsigned int vertex_property_bits) {
	
	const aiScene* imported_scene;

	if (_imported_assets.find(filename) == _imported_assets.end()) {
		imported_scene = readfile(filename);
	}
	else {
		imported_scene = _imported_assets[filename];
	}

	return Model(imported_scene, scale, vertex_property_bits);
}

void AssetImporter::clear_ram(const std::string& filename) {
	
	std::unordered_map<std::string, const aiScene*>::iterator it = _imported_assets.find(filename);
	if (it == _imported_assets.end()) {
		return;
	}
	
	const aiScene* imported_scene = it->second;
	_imported_assets.erase(it);
	delete imported_scene;
}

void AssetImporter::clear_ram_all() {
	for (std::pair<std::string, const aiScene*> pair : _imported_assets) {
		delete pair.second;
	}
	_imported_assets.clear();
}

