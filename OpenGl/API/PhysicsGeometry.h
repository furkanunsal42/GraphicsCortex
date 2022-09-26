#pragma once
#include <PxPhysicsAPI.h>
#include "Config.h"
#include "Debuger.h"

namespace create_geometry {
	physx::PxBoxGeometry box(float half_length_x, float half_length_y, float half_length_z);
	physx::PxSphereGeometry sphere(float radius);
	physx::PxCapsuleGeometry capsule(float radius, float half_height);
	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxConvexMeshGeometry> 
		convex_hull(const T verticies[], unsigned int count);
	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxConvexMeshGeometry> 
		convex_hull(const std::vector<T>& verticies);
	
	physx::PxPlane plane(float nx, float ny, float nz, float distance);
	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxPlane>
		plane(T normal, float distance);

	enum heightfield_array_type {
		ARRAY_OF_ROWS = 0,
		ARRAY_OF_COLUMNS,
		ARRAY_OF_ROWS_INVERTED,
		ARRAY_OF_COLUMNS_INVERTED,
	};

	physx::PxHeightFieldGeometry heightfield(physx::PxHeightFieldSample heightfield_data[], unsigned int column_size, unsigned int row_size, heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS);
	physx::PxHeightFieldGeometry heightfield(std::vector<physx::PxHeightFieldSample> heightfield_data, unsigned int column_size, unsigned int row_size, enum create_geometry::heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS);
	physx::PxHeightFieldGeometry heightfield(std::vector<std::vector<physx::PxHeightFieldSample>> heightfield_data, enum create_geometry::heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS);

	/*
	physx::PxTriangleMeshGeometry custom(const std::vector<physx::PxVec3>& verticies, const std::vector<unsigned int>& indicies);
	*/
}
