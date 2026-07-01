#pragma once

#include <filesystem>

#include "SingleModel.h"
#include "Model.h"
#include "Mesh.h"
#include "Materials/ModelMaterial.h"
#include "Materials/MeshMaterial.h"

#include "ECS/Prefab.h"

struct AssetImportDescription {

};

// skeletons
// animation clips
// shaders?

class Asset {
public:

	Asset(const std::filesystem::path& asset_path, const AssetImportDescription& properties = AssetImportDescription());
	~Asset();

	Prefab			load_prefab();
	
	SingleModel		load_single_model(uint32_t submodel_index);
	Model			load_model();
	Mesh			load_mesh();
	
	ModelMaterial::SingleMaterial load_single_model_material(uint32_t submodel_index);	// TODO sync with load_model_material()
	ModelMaterial	load_model_material();
	MeshMaterial	load_mesh_material();

private:

	void* importer		= nullptr;
	const void* scene	= nullptr;
	std::filesystem::path filepath;
};