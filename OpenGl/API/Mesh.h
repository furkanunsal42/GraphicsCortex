#pragma once

#include "Config.h"

#include "Buffer.h"
#include "Model.h"

class Mesh {
public:
	ArrayBuffer array_buffer;
	IndexBuffer index_buffer;

	Mesh(ArrayBuffer& array_buffer, IndexBuffer& index_buffer);
	Mesh(ArrayBuffer&& array_buffer, IndexBuffer&& index_buffer);

	explicit Mesh(const Model& model);

	void bind();
	void unbind();
};