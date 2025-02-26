#pragma once

#include <filesystem>

#include "Mesh.h"
#include "Model.h"
#include "SingleModel.h"
#include "SingleMaterial.h"

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
	
	//SingleMaterial load_single_material(uint32_t submodel_index);
	//std::vector<SingleMaterial> load_material();

	//std::shared_ptr<Model> store_model_to_disk(const std::filesystem::path& asset_path);
	//std::shared_ptr<Mesh> store_mesh(const std::filesystem::path& asset_path);
	//// store_material(const std::filesystem::path& asset_path);
	//std::shared_ptr<SingleModel> store_as_single_model(const std::filesystem::path& asset_path);
	////store_single_material(const std::filesystem::path& asset_path);

private:

	void* importer = nullptr;
	const void* scene = nullptr;
	std::filesystem::path filepath;
};