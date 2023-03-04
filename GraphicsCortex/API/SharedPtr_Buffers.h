#pragma once
#include "SharedPtr.h"
#include "Buffer.h"

class ArrayBuffer_s {
public:

	_SHARED_POINTER_DEFINITIONS(ArrayBuffer);

	ArrayBuffer_s() :
		obj(std::make_shared<ArrayBuffer>()) {}

	ArrayBuffer_s(const ArrayBuffer& a) :
		obj(std::make_shared<ArrayBuffer>(a)) {}

	ArrayBuffer_s(float verticies[], int data_count) :
		obj(std::make_shared<ArrayBuffer>(verticies, data_count)) {}

	ArrayBuffer_s(std::vector<float> verticies) :
		obj(std::make_shared<ArrayBuffer>(verticies)) {}

};

class IndexBuffer_s {
public:

	_SHARED_POINTER_DEFINITIONS(IndexBuffer);

	IndexBuffer_s() :
		obj(std::make_shared<IndexBuffer>()) {}

	IndexBuffer_s(const IndexBuffer& i) :
		obj(std::make_shared<IndexBuffer>(i)) {}

	IndexBuffer_s(unsigned int verticies[], int vertex_dim, int data_count) :
		obj(std::make_shared<IndexBuffer>(verticies, vertex_dim, data_count)) {}

	IndexBuffer_s(std::vector<unsigned int> verticies, int vertex_dim) :
		obj(std::make_shared<IndexBuffer>(verticies, vertex_dim)) {}

};