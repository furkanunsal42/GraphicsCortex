#pragma once

#include <cstdint>
#include <functional>

class CortexScene;
class ISparseSet;

struct Entity2 {

	bool operator==(const Entity2& other) const;
	
	static const Entity2 invalid_entity;

private:

	uint32_t index		: 20 = 0xFFFFF; 
	uint32_t generation : 12 = 0xFFF;

	friend CortexScene;
	friend ISparseSet;
	friend struct std::hash<Entity2>;
};

namespace std {
    template <>
    struct hash<Entity2> {
		size_t operator()(const Entity2& entity) const {
			uint32_t packed_id = (entity.generation << 20) | entity.index;
			return std::hash<uint32_t>()(packed_id);
		}
    };
}