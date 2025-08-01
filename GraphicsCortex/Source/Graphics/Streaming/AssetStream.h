#pragma once

#include "Mesh.h"
#include "Texture2D.h"
#include "Material.h"
#include "Asset/Asset.h"
#include "Camera.h"

class StreamingConstraint {
	virtual float get_detail_level();
};

class StreamingAssetBase {
public:

	StreamingAssetBase(StreamingAssetBase& other) = delete;
	StreamingAssetBase(StreamingAssetBase&& other) = delete;
	StreamingAssetBase& operator=(StreamingAssetBase& other) = delete;
	StreamingAssetBase& operator=(StreamingAssetBase&& other) = delete;

	virtual void update();

	void clear_constraints();
	int32_t add_constraint(StreamingConstraint contraint);
	template<typename constraint_t>
	constraint_t get_constraint(int32_t constraint_id);
	void remove_constraint(int32_t constraint_id);

private:

	void _check_constraints_and_update();

	struct ConstraintData {
		StreamingConstraint constraint;
		size_t constraint_type_id;
	};
	std::map<int32_t, ConstraintData> constraints;
};

template<typename asset_t>
class StreamingAsset : public StreamingAssetBase {
public:
	
	void update();
	float get_loaded_detail_level();
	std::shared_ptr<asset_t> get_asset();
	bool is_loaded();
};

class AssetStream {
public:

	AssetStream(AssetStream& other) = delete;
	AssetStream(AssetStream&& other) = delete;
	AssetStream& operator=(AssetStream& other) = delete;
	AssetStream& operator=(AssetStream&& other) = delete;

	AssetStream& get();

	void update();

	void set_memory_limit(size_t memory_limit_in_bytes);
	void set_loading_thread_count(size_t memory_limit_in_bytes);
	size_t get_memory_limit();
	int32_t get_loading_thread_count();

	int32_t register_point();
	void update_point(int32_t point_id, const glm::vec3& point);
	void delete_point(int32_t point_id);

	int32_t register_camera();
	void update_camera(int32_t camera_id, const Camera& camera);
	void delete_camera(int32_t camera_id);

	template<typename resource_t, typename... arguments_t>
	std::shared_ptr<StreamingAsset<resource_t>> add(arguments_t...);

private:
	AssetStream(int32_t loading_threads = 4, size_t memory_limit_in_bytes = 1024 * 1024 * 2);

	enum ThreadState {
		Free,
		Loading,
		Loaded,
	};

	size_t target_memory_limit_in_bytes = 0;
	int32_t target_thread_count = 0;

	std::vector<ThreadState> thread_states;
	std::map<int32_t, glm::vec3> points;
	std::map<int32_t, Camera> cameras;

	std::vector<std::weak_ptr<StreamingAssetBase>> streaming_assets;
};

#include "AssetStreamTemplated.h"