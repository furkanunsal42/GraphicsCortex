#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"

int main() {
	
	glm::ivec3 volume_size(512, 512, 512);
	glm::ivec2 frame_size(1024, 1024);

	Frame frame(frame_size.x, frame_size.y, "CTReconstructor Rendering", 0, 0, true, true, true, Frame::CallbackLevel::NOTIFICATION, 0);
	Scene scene(frame);

	std::shared_ptr<Program> volumetric_program = std::make_shared<Program>(
		Shader("../GraphicsCortex/Source/GLSL/Surface/surface.vert",
			   "../GraphicsCortex/Source/GLSL/Surface/surface.geom",
			   "../CTReconstructor/Source/GLSL/VolumetricRendering/volumetric.frag"));
	
	std::shared_ptr<BindlessMaterial> volumetric_material = std::make_shared<BindlessMaterial>(volumetric_program);

	std::shared_ptr<Graphic> box = default_geometry::cube(volumetric_material, glm::vec3(1, 1, 1));
	scene.add(box);

	std::shared_ptr<FBP3D> fbp_solver = std::make_shared<FBP3D>();

	fbp_solver->generate_shepplogan(volume_size.x, volume_size.y, volume_size.z);
	fbp_solver->read_projections_as_sinograms("C:/Users/FURKAN.UNSAL/Desktop/Projektionen", 2048, 2048, 1, 2, volume_size.x, volume_size.y, 1440);
	fbp_solver->log_normalize_sinograms(97.0 / 255);
	
	//fbp_solver->apply_fdk_weights_to_sinograms(730.87f, 669.04f, 409.60f);
	fbp_solver->apply_filter_to_sinograms(FBP2D::FilterType::SHEPP_LOGAN);
	fbp_solver->project_backward_cone_fdk_from_sinograms(730.87f, 669.04f, 409.60f, 213.84f, 1, volume_size.x, volume_size.y, 0);
	fbp_solver->normalize_histogram();

	volumetric_program->update_uniform("volume", *fbp_solver->volume[0][0][0]);

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(0, 0, 0);
		frame.display_performance(180);
		
		scene.camera->handle_movements(frame.window, deltatime);

		GLCall(glCullFace(GL_FRONT));
		scene.render();
	}
}