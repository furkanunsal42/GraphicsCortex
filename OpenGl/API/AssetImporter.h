#pragma once

#include "Config.h"

#include <string>
#include <unordered_map>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Texture.h"


class AssetImporter {
public:


	static const aiScene* readfile(const std::string& filename, unsigned int flags =	aiProcess_CalcTangentSpace |
																						aiProcess_Triangulate |
																						aiProcess_JoinIdenticalVertices |
																						aiProcess_SortByPType |
																						aiProcess_GenSmoothNormals);
	
	static UnorderedMaterial generate_material(const std::string& filename);
	static Model generate_model(const std::string& filename, float scale = 1.0f, unsigned int vertex_property_bits = Model::ALL);

	static void clear_ram(const std::string& filename = "");
	static void clear_ram_all();

private:

	static Assimp::Importer _assimp_asset_loader;
	static std::unordered_map<std::string, const aiScene*> _imported_assets;
};
