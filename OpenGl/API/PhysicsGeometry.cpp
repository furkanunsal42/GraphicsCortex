#include "PhysicsGeometry.h"
#include "PhysicsContext.h"
#include "Debuger.h"

#include <iostream>
#include <type_traits>
#include <vector>

#include <glm.hpp>

namespace create_geometry {
	physx::PxBoxGeometry box(float half_length_x, float half_length_y, float half_length_z) {
		return physx::PxBoxGeometry(physx::PxReal(half_length_x), physx::PxReal(half_length_y), physx::PxReal(half_length_z));
	}

	physx::PxSphereGeometry sphere(float radius) {
		return physx::PxSphereGeometry(physx::PxReal(radius));
	}

	physx::PxCapsuleGeometry capsule(float radius, float half_height) {
		return physx::PxCapsuleGeometry(physx::PxReal(radius), physx::PxReal(half_height));
	}

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


	physx::PxPlane plane(float nx, float ny, float nz, float distance) {
		return physx::PxPlane(nx, ny, nz, distance);
	}

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

	physx::PxHeightFieldGeometry heightfield(physx::PxHeightFieldSample heightfield_data[], unsigned int column_size, unsigned int row_size, heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS) {
		if (column_size == 0 || row_size == 0) {
			std::cout << "[PhysX Error] create_geometry::heightfield() is called but column_size or row_size was zero.\n";
			ASSERT(false);
		}

		physx::PxHeightFieldSample* heightfield_data_new = (physx::PxHeightFieldSample*)malloc(sizeof(physx::PxHeightFieldSample) * column_size * row_size);

		if (array_type == ARRAY_OF_ROWS) {
			for (int i = 0; i < column_size * row_size; i++) {
				int column = i / row_size;
				int row = i % row_size;

				int i_new = row * column_size + column;
				heightfield_data_new[i_new] = heightfield_data[i];
			}
		}
		else if (array_type == ARRAY_OF_COLUMNS_INVERTED) {
			for (int i = 0; i < column_size * row_size; i++) {
				int column = i / row_size;
				int row = i % row_size;

				int i_new = (column_size - 1 - column) * row_size + column_size - row - 1;
				heightfield_data_new[i_new] = heightfield_data[i];
			}
		}
		else if (array_type == ARRAY_OF_ROWS_INVERTED) {
			for (int i = 0; i < column_size * row_size; i++) {
				int column = i / row_size;
				int row = i % row_size;

				int i_new = (row_size - 1 - row) * column_size + row_size - column - 1;
				heightfield_data_new[i_new] = heightfield_data[i];
			}
		}

		delete[] heightfield_data_new;

		physx::PxHeightFieldDesc mesh_desc;
		//mesh_desc.format = physx::PxHeightFieldFormat::eS16_TM;
		mesh_desc.nbRows = row_size;
		mesh_desc.nbColumns = column_size;
		mesh_desc.samples.stride = sizeof physx::PxHeightFieldSample;

		if (array_type == ARRAY_OF_COLUMNS)
			mesh_desc.samples.data = heightfield_data;
		else
			mesh_desc.samples.data = heightfield_data_new;

		auto context = PhysxContext::get();
		physx::PxHeightField* heightfield_mesh = context.physics_cooking->createHeightField(mesh_desc, context.physics->getPhysicsInsertionCallback());
		/*
		physx::PxDefaultMemoryOutputStream buf;
		context.physics_cooking->cookHeightField(mesh_desc, buf);
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxHeightField* heightfield_mesh = context.physics->createHeightField(input);
		*/
		physx::PxMeshGeometryFlags flags;

		return physx::PxHeightFieldGeometry(heightfield_mesh, flags, physx::PxReal(1.0f), physx::PxReal(1.0f), physx::PxReal(1.0f));
	}

	physx::PxHeightFieldGeometry heightfield(std::vector<physx::PxHeightFieldSample> heightfield_data, unsigned int column_size, unsigned int row_size, heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS) {
		return heightfield(&(heightfield_data[0]), column_size, row_size, array_type);
	}

	physx::PxHeightFieldGeometry heightfield(std::vector<std::vector<physx::PxHeightFieldSample>> heightfield_data, heightfield_array_type array_type = heightfield_array_type::ARRAY_OF_COLUMNS) {
		unsigned int inner_vector_size = heightfield_data[0].size();
		for (std::vector<physx::PxHeightFieldSample> inner_vector : heightfield_data) {
			if (inner_vector.size() != inner_vector_size) {
				std::cout << "[PhysX Error] create_geometry::heightfield() is called but heightfield_data doesn't contain identical sized vectors.\n";
				ASSERT(false);
			}
		}
		unsigned int outer_vector_size = heightfield_data.size();
		physx::PxHeightFieldSample* new_data = new physx::PxHeightFieldSample[outer_vector_size * inner_vector_size];
		for (int i = 0; i < outer_vector_size * inner_vector_size; i++) {
			int outer = i / inner_vector_size;
			int inner = i % inner_vector_size;

			new_data[i] = heightfield_data[outer][inner];
		}

		if (array_type == ARRAY_OF_COLUMNS || array_type == ARRAY_OF_COLUMNS_INVERTED) {
			physx::PxHeightFieldGeometry result = heightfield(&(new_data[0]), inner_vector_size, outer_vector_size, array_type);
			delete[] new_data;
			return result;
		}
		if (array_type == ARRAY_OF_ROWS || array_type == ARRAY_OF_ROWS_INVERTED) {
			physx::PxHeightFieldGeometry result = heightfield(&(new_data[0]), outer_vector_size, inner_vector_size, array_type);
			delete[] new_data;
			return result;
		}
	}

	/*
	physx::PxTriangleMeshGeometry custom(const std::vector<physx::PxVec3>& verticies, const std::vector<unsigned int>& indicies) {
		auto context = PhysxContext::get();

		physx::PxTriangleMeshDesc mesh_desc;
		mesh_desc.points.data = &(verticies[0]);
		mesh_desc.points.stride = sizeof verticies[0];
		mesh_desc.points.count = verticies.size();

		mesh_desc.triangles.data = &(indicies[0]);
		mesh_desc.triangles.stride = 3 * sizeof indicies[0];
		mesh_desc.triangles.count = indicies.size();

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxTriangleMeshCookingResult::Enum result;
		context.physics_cooking->cookTriangleMesh(mesh_desc, buf, &result);
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxTriangleMesh* triangle_mesh = context.physics->createTriangleMesh(input);

		return physx::PxTriangleMeshGeometry(triangle_mesh);
	}
	*/

}
