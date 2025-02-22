#include "GL/glew.h"
#include "IndexBufferEnums.h"

uint32_t get_IndexType_bytes_per_index(IndexType type) {
	switch (type) {
	case IndexType::i_ui8:  return 1;
	case IndexType::i_ui16: return 2;
	case IndexType::i_ui32: return 4;
	}
	return 4;
}

uint32_t get_PrimitiveType_index_count(PrimitiveType primitive)
{
	switch (primitive) {
	case PrimitiveType::point :						return 1;
	case PrimitiveType::line :						return 2;
	case PrimitiveType::line_strip :				return 2;	
	case PrimitiveType::line_loop :					return 2;
	case PrimitiveType::triangle :					return 3;
	case PrimitiveType::triangle_strip :			return 3;		
	case PrimitiveType::triangle_fan :				return 3;	
	case PrimitiveType::line_strip_adjacency :		return 2;			
	case PrimitiveType::line_adjacency :			return 2;		
	case PrimitiveType::triangle_strip_adjacency :	return 3;				
	case PrimitiveType::triangle_adjacency :		return 3;			
	case PrimitiveType::patches :					return 0;	// ? unknown
	}
	return 3;
}

uint32_t IndexType_to_GL(IndexType type)
{
	switch (type) {
	case IndexType::i_ui8:  return GL_UNSIGNED_BYTE;
	case IndexType::i_ui16: return GL_UNSIGNED_SHORT;
	case IndexType::i_ui32: return GL_UNSIGNED_INT;
	}
	return 0;
}

uint32_t PrimitiveType_to_GL(PrimitiveType primitive)
{
	switch (primitive) {
	case point:				return GL_POINTS;
	case line:				return GL_LINES;
	case line_strip:		return GL_LINE_STRIP;
	case line_loop:			return GL_LINE_LOOP;
	case triangle:			return GL_TRIANGLES;
	case triangle_strip:	return GL_TRIANGLE_STRIP;
	case triangle_fan:		return GL_TRIANGLE_FAN;

	case line_strip_adjacency:		return GL_LINE_STRIP_ADJACENCY;
	case line_adjacency:			return GL_LINES_ADJACENCY;
	case triangle_strip_adjacency:	return GL_TRIANGLE_STRIP_ADJACENCY;
	case triangle_adjacency:		return GL_TRIANGLES_ADJACENCY;
	case patches:					return GL_PATCHES;
	}
	return 0;
}
