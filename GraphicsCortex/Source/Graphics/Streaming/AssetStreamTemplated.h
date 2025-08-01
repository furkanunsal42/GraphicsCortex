#pragma once
#include "AssetStream.h"

class StreamingConstraint_Distance : public StreamingConstraint {
	float get_detail_level();
};
class StreamingConstraint_VisibleAABB : public StreamingConstraint {
	float get_detail_level();
};
class StreamingConstraint_ConstantMin : public StreamingConstraint {
	float get_detail_level();
};
class StreamingConstraint_ConstantMax : public StreamingConstraint {
	float get_detail_level();
};

class StreamingAsset<Mesh> {

};

class StreamingAsset<MeshMaterial> {

};

class StreamingAsset<Texture2D> {

};

template<>
StreamingAsset<Mesh> AssetStream::add(
	const std::filesystem::path& asset_path,
	const AssetImportDescription& properties
);

template<>
StreamingAsset<Mesh> AssetStream::add(
	const std::filesystem::path& asset_path
);

template<>
StreamingAsset<MeshMaterial> AssetStream::add(
	const std::filesystem::path& asset_path,
	const AssetImportDescription& properties
);

template<>
StreamingAsset<MeshMaterial> AssetStream::add(
	const std::filesystem::path& asset_path
);

template<>
StreamingAsset<Texture2D> AssetStream::add(
	const std::filesystem::path& asset_path,
	Texture2D::ColorTextureFormat internal_format
);
