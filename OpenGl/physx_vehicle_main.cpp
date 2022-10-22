#include "API/GraphicsCortex.h"

void vehicle_control(GLFWwindow* window, physx::PxVehicleDrive4WRawInputData& vehicle_controller, physx::PxVehicleDrive4W* vehicle) {
	vehicle_controller.setDigitalAccel(false);
	vehicle_controller.setDigitalBrake(false);
	vehicle_controller.setDigitalHandbrake(false);
	vehicle_controller.setDigitalSteerLeft(false);
	vehicle_controller.setDigitalSteerRight(false);

	vehicle->mDriveDynData.setUseAutoGears(true);

	if (glfwGetKey(window, GLFW_KEY_W) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		vehicle_controller.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eREVERSE);
		vehicle_controller.setDigitalAccel(true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1) {
		vehicle_controller.setDigitalSteerLeft(true);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == 1) {
		vehicle_controller.setDigitalSteerRight(true);
	}


	if (glfwGetKey(window, GLFW_KEY_W) == 1 && glfwGetKey(window, GLFW_KEY_S) == 1) {
		vehicle->mDriveDynData.forceGearChange(snippetvehicle::PxVehicleGearsData::eFIRST);
		vehicle_controller.setDigitalAccel(false);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == 1 && glfwGetKey(window, GLFW_KEY_D) == 1) {
		vehicle_controller.setDigitalSteerLeft(false);
		vehicle_controller.setDigitalSteerRight(false);
	}

}

int main() {
	PhysicsScene scene;

	PhysicsObject plane(create_geometry::plane(0, 1, 0, 0), false);
	plane.make_drivable();
	scene.add_actor(plane);

	PhysicsVehicle vehicle;
	
	std::vector<physx::PxVec3> points = {	physx::PxVec3(1,0.2,-2.8), physx::PxVec3(1,1,-1.6), physx::PxVec3(1,1,0.6), physx::PxVec3(1,0.3,2.9), physx::PxVec3(1,-1,2.8), physx::PxVec3(1,-1,-2.8), 
											physx::PxVec3(-1,0.2,-2.8), physx::PxVec3(-1,1,-1.6), physx::PxVec3(-1,1,0.6), physx::PxVec3(-1,0.3,2.9), physx::PxVec3(-1,-1,2.8), physx::PxVec3(-1,-1,-2.8)};
	vehicle.set_chasis_mesh(create_geometry::convex_hull(points).convexMesh);
	vehicle.chassisDims = physx::PxVec3(2, 2, 6);

	vehicle.set_wheel_layout(1.9f, -0.2f, 5.0f, 0.0f);

	vehicle.compile();
	scene.add_actor(vehicle);

	PhysicsObject box(create_geometry::box(1.0f, 1.0f, 1.0f), PhysicsObject::DYNAMIC, true);
	scene.add_actor(box);
	box.set_position(4.0f, 10.0f, 4.0f);

	float timestep = 1 / 60.0f;

	GLFWwindow* window = frame::create_window(100, 100, "vehicle_control", 0, 1);

	while (true) {
		glfwPollEvents();
		frame::clear_window();

		vehicle_control(window, vehicle.InputData, vehicle.vehicle_drive);

		scene.simulation_step_start(timestep);
		scene.simulation_step_finish();

		glfwSwapBuffers(window);
	}

}