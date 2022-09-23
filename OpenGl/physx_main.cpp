#include "API/GraphicsCortex.h"

#include <PxPhysicsAPI.h>

#define PVD_HOST "127.0.0.1"
#define PHYSX_THREAD_SIZE 1

#include <thread>

physx::PxDefaultAllocator physics_allocator;
physx::PxDefaultErrorCallback physics_error_callback;
physx::PxPhysics* physics;
physx::PxFoundation* physics_foundation;
physx::PxSceneDesc* scene_desc;
physx::PxScene* physics_scene;
physx::PxPvd* physics_pvd;
physx::PxPvdTransport* physics_transport;
physx::PxCooking* physics_cooking;

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
	
	physx::PxConvexMeshGeometry convex_hull(const std::vector<physx::PxVec3>& verticies) {
		
		// verticies check

		physx::PxConvexMeshDesc mesh_desc;
		physx::PxBoundedData points;
		points.data = &(verticies[0]);
		points.stride = sizeof verticies[0];
		points.count = 5;
		mesh_desc.points = points;
		mesh_desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		physics_cooking->cookConvexMesh(mesh_desc, buf, &result);
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convex_mesh = physics->createConvexMesh(input);
		
		return physx::PxConvexMeshGeometry(convex_mesh);
	}

	physx::PxConvexMeshGeometry convex_hull(const std::vector<glm::vec3>& verticies) {

		// verticies check

		physx::PxConvexMeshDesc mesh_desc;
		physx::PxBoundedData points;
		points.data = &(verticies[0]);
		points.stride = sizeof verticies[0];
		points.count = 5;
		mesh_desc.points = points;
		mesh_desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		physx::PxDefaultMemoryOutputStream buf;
		physx::PxConvexMeshCookingResult::Enum result;
		physics_cooking->cookConvexMesh(mesh_desc, buf, &result);
		physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
		physx::PxConvexMesh* convex_mesh = physics->createConvexMesh(input);

		return physx::PxConvexMeshGeometry(convex_mesh);
	}
	


}

class PhysicsObject {
public:
	physx::PxMaterial* material;
	physx::PxShape* shape;
	physx::PxTransform transform;
	physx::PxRigidDynamic* dynamic_actor;
	bool exclusive_shape = false;

	glm::vec3 position;
	glm::vec3 rotation;

	PhysicsObject(const physx::PxGeometry& geometry, bool exclusive_shape = false) :
		position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), exclusive_shape(exclusive_shape)
	{
		material = physics->createMaterial(0.5f, 0.5f, 0.5f);
		update_transform();
		dynamic_actor = physics->createRigidDynamic(transform);

		create_shape(geometry, *material, exclusive_shape);
	}

	void create_shape(const physx::PxGeometry& geometry, const physx::PxMaterial& material, bool exclusive_shape = false) {
		shape = physics->createShape(geometry, material, exclusive_shape);
		dynamic_actor->attachShape(*shape);
	}

	void update_transform() {
		glm::quat rotatoion_quat_glm(glm::vec3(rotation.x, rotation.y, rotation.z));
		physx::PxQuat rotation_quat(rotatoion_quat_glm.x, rotatoion_quat_glm.y, rotatoion_quat_glm.z, rotatoion_quat_glm.w);
		transform = physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), rotation_quat);

		if (dynamic_actor != nullptr)
			dynamic_actor->setGlobalPose(transform);
	}
};

void physics_init() {
	if (physics != nullptr)
		return;
	
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
}

int main() {
	physics_init();
	
	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), true);
	box.rotation.x = 0.5f;
	box.position.y = 10.0f;
	box.update_transform();
	physics_scene->addActor(*box.dynamic_actor);

	PhysicsObject sphere(create_geometry::sphere(1.0f), true);
	sphere.position.y = 10.0f;
	sphere.position.z = 10.0f;
	sphere.update_transform();
	physics_scene->addActor(*sphere.dynamic_actor);

	PhysicsObject capsule(create_geometry::capsule(1.0f, 1.0f), true);
	capsule.position.y = 10.0f;
	capsule.rotation.x = physx::PxHalfPi;
	capsule.update_transform();
	physics_scene->addActor(*capsule.dynamic_actor);

	std::vector<physx::PxVec3> pyramid_index = { physx::PxVec3(0,1,0), physx::PxVec3(1,0,0), physx::PxVec3(-1,0,0), physx::PxVec3(0,0,1), physx::PxVec3(0,0,-1) };
	PhysicsObject pyramid(create_geometry::convex_hull(pyramid_index), true);
	pyramid.position.y = 10.0f;
	pyramid.position.z = 20.0f;
	pyramid.update_transform();
	physics_scene->addActor(*pyramid.dynamic_actor);

	physx::PxMaterial* plane_material = physics->createMaterial(physx::PxReal(0.5f), physx::PxReal(0.5f), physx::PxReal(0.5f));
	physx::PxRigidStatic* plane_actor = physx::PxCreatePlane(*physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 10.0f), *plane_material);
	physics_scene->addActor(*plane_actor);

	



	while (true) {
		physics_scene->simulate(1 / 1000.0f);
		physics_scene->fetchResults(true);

	}

}