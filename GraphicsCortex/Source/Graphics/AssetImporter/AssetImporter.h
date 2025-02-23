#pragma once

#include <filesystem>

#include "Mesh.h"
#include "Model.h"
#include "SingleModel.h"

struct AssetImportDescription {

};

// skeletons
// animation clips
// shaders?

class Asset {
public:

	Asset(const std::filesystem::path& asset_path, const AssetImportDescription& properties = AssetImportDescription());
	Asset(Model& model);

	std::shared_ptr<Model> load_model();
	std::shared_ptr<Mesh> load_mesh();
	// load_material();
	std::shared_ptr<SingleModel> load_as_single_model();
	// load_single_material();

	//std::shared_ptr<Model> store_model_to_disk(const std::filesystem::path& asset_path);
	//std::shared_ptr<Mesh> store_mesh(const std::filesystem::path& asset_path);
	//// store_material(const std::filesystem::path& asset_path);
	//std::shared_ptr<SingleModel> store_as_single_model(const std::filesystem::path& asset_path);
	////store_single_material(const std::filesystem::path& asset_path);

private:
};