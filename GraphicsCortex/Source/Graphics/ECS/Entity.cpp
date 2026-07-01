#include "Entity.h"

const Entity2 Entity2::invalid_entity = Entity2();

bool Entity2::operator==(const Entity2& other) const {
	return index == other.index && generation == other.generation;
}