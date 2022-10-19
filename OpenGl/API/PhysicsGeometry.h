#pragma once
#include <PxPhysicsAPI.h>
#include "PhysicsContext.h"
#include "Config.h"
#include "Debuger.h"

#include "glm.hpp"

#include <vector>
#include <type_traits>

namespace create_geometry {
	physx::PxBoxGeometry box(float half_length_x, float half_length_y, float half_length_z);
	physx::PxSphereGeometry sphere(float radius);
	physx::PxCapsuleGeometry capsule(float radius, float half_height);
	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxConvexMeshGeometry>
		convex_hull(const T verticies[], unsigned int count) {

		physx::PxConvexMeshDesc mesh_desc;
		mesh_desc.points.data = verticies;
		mesh_desc.points.stride = sizeof verticies[0];
		mesh_desc.points.count = count;
		mesh_desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		auto context = PhysxContext::get();
		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		context.physics_cooking->cookConvexMesh(mesh_desc, buf, &result);
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convex_mesh = context.physics->createConvexMesh(input);

		return physx::PxConvexMeshGeometry(convex_mesh);
	}
	
	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxConvexMeshGeometry>
		convex_hull(const std::vector<T>& verticies) {
		return convex_hull(&(verticies[0]), verticies.size());
	}
	
	physx::PxPlane plane(float nx, float ny, float nz, float distance);

	template<typename T>
	std::enable_if_t<std::is_same<T, physx::PxVec3>::value || std::is_same<T, glm::vec3>::value, physx::PxPlane>
		plane(T normal, float distance) {
		return plane(normal.x, normal.y, normal.z, distance);
	}


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
