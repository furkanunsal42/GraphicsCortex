#pragma once

#include "Config.h"

#include "Buffer.h"
#include "Model.h"

class Mesh {
public:
	struct SingleMesh {
		std::shared_ptr<ArrayBuffer> array_buffer = nullptr;
		std::shared_ptr<IndexBuffer> index_buffer = nullptr;
	};
	std::vector<SingleMesh> submeshes;

	Mesh();
	Mesh(std::shared_ptr<ArrayBuffer> array_buffer, std::shared_ptr<IndexBuffer> index_buffer);

	explicit Mesh(const Model& model);

	void load_model(const Model& model);

	void bind(int submesh_index);
	void unbind();
};