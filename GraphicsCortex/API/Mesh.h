#pragma once

#include "Config.h"

#include "Buffer.h"
#include "Model.h"
#include "SharedPtr_Buffers.h"

class Mesh {
public:
	std::shared_ptr<ArrayBuffer> array_buffer;
	std::shared_ptr<IndexBuffer> index_buffer;

	Mesh();
	Mesh(ArrayBuffer_s array_buffer, IndexBuffer_s index_buffer);

	explicit Mesh(const Model& model);

	void load_model(const Model& model);

	void bind();
	void unbind();
};