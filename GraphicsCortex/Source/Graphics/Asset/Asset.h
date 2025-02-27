#pragma once

#include <filesystem>

#include "SingleModel.h"
#include "Model.h"
#include "Mesh.h"
#include "Materials/ModelMaterial.h"
#include "Materials/MeshMaterial.h"

struct AssetImportDescription {

};

// skeletons
// animation clips
// shaders?

class Asset {
public:

	Asset(const std::filesystem::path& asset_path, const AssetImportDescription& properties = AssetImportDescription());
	~Asset();
	//Asset(Model& model);
	//Asset(SingleModel& single_model);

	SingleModel load_single_model(uint32_t submodel_index);
	Model load_model();
	Mesh load_mesh();
	
	ModelMaterial::SingleMaterial load_single_model_material(uint32_t submodel_index);
	ModelMaterial load_model_material();
	MeshMaterial load_mesh_material();

private:

	void* importer = nullptr;
	const void* scene = nullptr;
	std::filesystem::path filepath;
};