#include "AssetImporter.h"
#include "DirectoryUtils.h"
#include "BinaryBuffers.h"

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

Image AssetImporter::read_image_cached(const std::string& filename, int desired_channels, bool vertical_flip) {
	std::string native_filename = compute_directory(filename) + compute_filename_typeless(filename) + ".gcimage";
	if (check_file_exist(native_filename)) {
		std::cout << "cached image file exists" << std::endl;
		// todo
	}
	else {
		std::cout << "cached image doesn't exist, caching it" << std::endl;
		Image image(filename, desired_channels, vertical_flip);
		size_t size = image.get_size();
		std::ofstream file(native_filename, std::ios::binary | std::ios::out);
		file << (int)image.get_size() << ' ';
		file << image.get_width() << ' ';
		file << image.get_height() << ' ';
		file << image.get_channels() << ' ';
		file << image.get_vertical_flip() << ' ';
		save_buffer_to_disc(native_filename, image.get_image_data(), size, true);
		file.close();
		return image;
	}
}

//Model AssetImporter::read_model_cached(const std::string& filename, float scale, unsigned int vertex_property_bits) {
//	std::string native_filename = compute_directory(filename) + compute_filename_typeless(filename) + ".gcmodel";
//	if (check_file_exist(native_filename)) {
//		std::cout << "cached model file exists" << std::endl;
//		std::ifstream ifile(native_filename, std::ios::binary | std::ios::in);
//		int image_size;
//		int width, height, channels;
//		bool flip;
//		ifile >> image_size;
//		ifile >> width;
//		ifile >> height;
//		ifile >> channels;
//		ifile >> flip;
//		unsigned char* memory = (unsigned char*)malloc(image_size);
//		read_buffer_from_disc(native_filename, memory, image_size);
//		return Image((unsigned char*)memory, width, height, channels, flip);
//	}
//	else {
//		std::cout << "cached model doesn't exist, caching it" << std::endl;
//		Model model(filename, scale, vertex_property_bits);		
//		std::ofstream file(native_filename, std::ios::binary | std::ios::out);
//		int vertex_data_size = model.vertex_data.size();
//		int index_data_size = model.index_data.size();
//		int attribute_structure_size = model.vertex_attribute_structure.size();
//		file << scale << ' ';
//		file << vertex_property_bits << ' ';
//		file << vertex_data_size << ' ';
//		file << index_data_size << ' ';
//		file << attribute_structure_size << ' ';
//		save_buffer_to_disc(native_filename, &(model.vertex_data[0]), vertex_data_size, true);
//		file << ' ';
//		save_buffer_to_disc(native_filename, &(model.index_data[0]), index_data_size, true);
//		file << ' ';
//		save_buffer_to_disc(native_filename, &(model.vertex_attribute_structure[0]), attribute_structure_size, true);
//		file.close();
//		return model;
//	}
//}

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

