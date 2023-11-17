#pragma once

#include "Config.h"

#include "Buffer.h"
#include "Model.h"

class Mesh {
public:
	std::shared_ptr<ArrayBuffer> array_buffer;
	std::shared_ptr<IndexBuffer> index_buffer;

	Mesh();
	Mesh(std::shared_ptr<ArrayBuffer> array_buffer, std::shared_ptr<IndexBuffer> index_buffer);

	explicit Mesh(const Model& model);

	void load_model(const Model& model);

	void bind();
	void unbind();
};