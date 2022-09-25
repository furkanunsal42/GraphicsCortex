#include "API/GraphicsCortex.h"

#include <PxPhysicsAPI.h>

#define PVD_HOST "127.0.0.1"
#define PHYSX_THREAD_SIZE 1

#include <thread>

class PhysxContext {
public:
	physx::PxDefaultAllocator physics_allocator;
	physx::PxDefaultErrorCallback physics_error_callback;
	physx::PxPhysics* physics;
	physx::PxFoundation* physics_foundation;
	physx::PxSceneDesc* scene_desc;
	physx::PxScene* physics_scene;
	physx::PxPvd* physics_pvd;
	physx::PxPvdTransport* physics_transport;
	physx::PxCooking* physics_cooking;

	static PhysxContext& get() {
		static PhysxContext instance;
		return instance;
	}

private:
	PhysxContext() {
		physics_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physics_allocator, physics_error_callback);

		physics_pvd = PxCreatePvd(*physics_foundation);
		physics_transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
		physics_pvd->connect(*physics_transport, physx::PxPvdInstrumentationFlag::eALL);

		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *physics_foundation, physx::PxTolerancesScale(), true, physics_pvd);

		scene_desc = new physx::PxSceneDesc(physics->getTolerancesScale());
		scene_desc->gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

		if (PHYSX_THREAD_SIZE <= 0)
			scene_desc->cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
		else
			scene_desc->cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(PHYSX_THREAD_SIZE);

		scene_desc->filterShader = physx::PxDefaultSimulationFilterShader;

		physics_scene = physics->createScene(*scene_desc);

		physics_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *physics_foundation, physx::PxCookingParams(physics->getTolerancesScale()));
	};
};

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

	physx::PxHeightFieldGeometry heightfield(physx::PxHeightFieldSample heightfield_data[], unsigned int column_size, unsigned int row_size, heightfield_array_type array_type = ARRAY_OF_COLUMNS) {
		if (column_size == 0 || row_size == 0){
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

	physx::PxHeightFieldGeometry heightfield(std::vector<physx::PxHeightFieldSample> heightfield_data, unsigned int column_size, unsigned int row_size, heightfield_array_type array_type = ARRAY_OF_COLUMNS) {
		return heightfield(&(heightfield_data[0]), column_size, row_size, array_type);
	}

	physx::PxHeightFieldGeometry heightfield(std::vector<std::vector<physx::PxHeightFieldSample>> heightfield_data, heightfield_array_type array_type = ARRAY_OF_COLUMNS) {
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

		if (array_type == ARRAY_OF_COLUMNS || array_type == ARRAY_OF_COLUMNS_INVERTED){
			physx::PxHeightFieldGeometry result = heightfield(&(new_data[0]), inner_vector_size, outer_vector_size, array_type);
			delete[] new_data;
			return result;
		}
		if (array_type == ARRAY_OF_ROWS || array_type == ARRAY_OF_ROWS_INVERTED){
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

class PhysicsObject {
public:
	physx::PxMaterial* material;
	physx::PxShape* shape;
	physx::PxTransform transform;
	physx::PxRigidActor* actor;
	bool exclusive_shape = false;
	
	unsigned int type;

	enum type {
		DYNAMIC = 0,
		KINEMATIC,
		STATIC,
	};

	PhysicsObject(const physx::PxGeometry& geometry, enum type type = type::DYNAMIC, bool exclusive_shape = false) :
		position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape), type(type)
	{

		auto context = PhysxContext::get();
		material = context.physics->createMaterial(0.5f, 0.5f, 0.5f);
		update_transform();
		if (type == type::DYNAMIC || type == type::KINEMATIC)
			actor = context.physics->createRigidDynamic(transform);
		if (type == type::STATIC)
			actor = context.physics->createRigidStatic(transform);

		create_shape(geometry, *material, exclusive_shape);
	}

	PhysicsObject(const physx::PxPlane& plane, bool exclusive_shape = false) :
		position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape), type(type::STATIC), shape(nullptr)
	{
		auto context = PhysxContext::get();
		material = context.physics->createMaterial(0.5f, 0.5f, 0.5f);
		actor = physx::PxCreatePlane(*context.physics, plane, *material);
	}

	void set_type(unsigned int new_type) {
		if (type == type::STATIC) {
			if (type != new_type)
				std::cout << "PhysicsObject.set_type(unsigned int new_type) is called but staic type cannot be changed.\n";
			return;
		}
		if (new_type == type::DYNAMIC)
			((physx::PxRigidBody*)actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
		if (new_type == type::KINEMATIC)
			((physx::PxRigidBody*)actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
	}

	void create_shape(const physx::PxGeometry& geometry, const physx::PxMaterial& material, bool exclusive_shape = false) {
		auto context = PhysxContext::get();
		shape = context.physics->createShape(geometry, material, exclusive_shape);
		actor->attachShape(*shape);
	}

	void set_position(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;

		update_transform();
	}

	template<typename T>
	std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, void>
		set_position(T rotation_vector) {
		set_position(rotation_vector.x, rotation_vector.y, rotation_vector.z);
	}

	void set_rotation(float x, float y, float z) {
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;

		update_transform();
	}
	
	template<typename T>
	std::enable_if_t<std::is_same<T, glm::vec3>::value || std::is_same<T, physx::PxVec3>::value, void>
	set_rotatoin(T rotation_vector) {
		set_rotation(rotation_vector.x, rotation_vector.y, rotation_vector.z);
	}


	void update_transform() {
		glm::quat rotatoion_quat_glm(glm::vec3(rotation.x, rotation.y, rotation.z));
		physx::PxQuat rotation_quat(rotatoion_quat_glm.x, rotatoion_quat_glm.y, rotatoion_quat_glm.z, rotatoion_quat_glm.w);
		transform = physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), rotation_quat);

		if (actor != nullptr)
			actor->setGlobalPose(transform);
	}

private:
	glm::vec3 position;
	glm::vec3 rotation;
};

class PhysicsScene{
public:

	std::vector<std::reference_wrapper<PhysicsObject>> actors;
	
	void add_actor(PhysicsObject& actor) {
		auto context = PhysxContext::get();
		context.physics_scene->addActor(*actor.actor);
		actors.push_back(actor);
	}
	
	void simulate_step(long double delta_time) {
		simulation_step_start(delta_time);
		simulation_step_finish();
	}

	void simulation_step_start(long double delta_time) {
		auto context = PhysxContext::get();
		context.physics_scene->simulate(delta_time);
	}

	void simulation_step_finish() {
		auto context = PhysxContext::get();
		context.physics_scene->fetchResults(true);
	}
};

int main() {
	PhysicsScene scene;

	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(box);
	box.set_position(0.5f, 10.0f, 0.0f);

	PhysicsObject sphere(create_geometry::sphere(1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(sphere);
	sphere.set_position(0.0f, 10.0f, 10.0f);

	PhysicsObject capsule(create_geometry::capsule(1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(capsule);
	sphere.set_position(0.0f, 10.0f, 0.0f);
	sphere.set_rotation(physx::PxHalfPi, 10.0f, 0.0f);

	std::vector<physx::PxVec3> pyramid_index = { physx::PxVec3(0,1,0), physx::PxVec3(1,0,0), physx::PxVec3(-1,0,0), physx::PxVec3(0,0,1), physx::PxVec3(0,0,-1) };
	PhysicsObject pyramid(create_geometry::convex_hull(pyramid_index), PhysicsObject::DYNAMIC, true);
	scene.add_actor(pyramid);
	pyramid.set_position(0.0f, 10.0f, 20.0f);

	PhysicsObject plane(create_geometry::plane(0.0f, 1.0f, 0.0f, 10.0f), true);
	scene.add_actor(plane);

	std::vector<std::vector<physx::PxHeightFieldSample>> samples;
	for (int i = 0; i < 5; i++){
		std::vector<physx::PxHeightFieldSample> sample_inner;
		for (int j = 0; j < 5; j++) {
			physx::PxHeightFieldSample sample;
			if ( j < 2)
				sample.height = 2;
			else 
				sample.height = 0;
			sample_inner.push_back(sample);
		}
		samples.push_back(sample_inner);
	}
	PhysicsObject heightfield(create_geometry::heightfield(samples), PhysicsObject::STATIC, true);
	scene.add_actor(heightfield);




	while (true) {
		scene.simulation_step_start(1 / 1000.0f);
		scene.simulation_step_finish();
	}

}