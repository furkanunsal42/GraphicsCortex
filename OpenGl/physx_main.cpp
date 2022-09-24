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

		physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *physics_foundation, physx::PxTolerancesScale(), true, physics_pvd);

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

	PhysicsObject(const physx::PxGeometry& geometry, unsigned int type = type::DYNAMIC, bool exclusive_shape = false) :
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
	PhysicsObject pyramid(create_geometry::convex_hull(pyramid_index), PhysicsObject::STATIC, true);
	scene.add_actor(pyramid);
	pyramid.set_position(0.0f, 10.0f, 20.0f);
	pyramid.set_type(PhysicsObject::DYNAMIC);
	
	/*
	std::vector<physx::PxVec3> custom_vertex = { physx::PxVec3(0,1,0), physx::PxVec3(-1,0,0), physx::PxVec3(1,0,0), physx::PxVec3(-1,0,1) };
	//std::vector<physx::PxVec3> cutsom_index = { physx::PxVec3(0,1,2), physx::PxVec3(0,2,3), physx::PxVec3(0,3,1), physx::PxVec3(1,2,3) };
	std::vector<unsigned int> cutsom_index = { 0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3 };
	PhysicsObject custom(create_geometry::custom(custom_vertex, cutsom_index), true);
	scene.add_actor(custom);
	custom.set_position(0.0f, 10.0f, 20.0f);
	*/

	auto context = PhysxContext::get();
	physx::PxMaterial* plane_material = context.physics->createMaterial(physx::PxReal(0.5f), physx::PxReal(0.5f), physx::PxReal(0.5f));
	physx::PxRigidStatic* plane_actor = physx::PxCreatePlane(*context.physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 10.0f), *plane_material);
	context.physics_scene->addActor(*plane_actor);

	while (true) {
		scene.simulation_step_start(1 / 1000.0f);
		scene.simulation_step_finish();
	}

}