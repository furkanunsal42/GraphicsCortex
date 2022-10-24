#include "API/GraphicsCortex.h"


int main() {
	PhysicsScene scene;

	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(box);
	box.set_position(4.0f, 10.0f, 4.0f);

	PhysicsObject sphere(create_geometry::sphere(1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(sphere);
	sphere.set_position(4.0f, 10.0f, 4.0f);

	auto link = box.add_link(sphere, PhysicsLink::PRISMATIC);
	link->set_limit(physx::PxVec3(0.4, 0.4, 1));

	PhysicsObject capsule(create_geometry::capsule(1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(capsule);
	capsule.set_position(0.0f, 10.0f, 0.0f);
	capsule.set_rotation(physx::PxHalfPi, 10.0f, 0.0f);

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
		scene.simulation_step_start(1 / 60.0f);
		scene.simulation_step_finish();
	}

}