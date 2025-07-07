#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "FilteredBackProjection/FBP3DSegmentedMemory.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"

int main() {
	
	glm::ivec3 volume_size(1024, 1024, 1024);
	glm::ivec2 frame_size(1920, 1080);
	int32_t projection_count = 1440;
	glm::ivec2 projection_size = glm::ivec2(2048, 2048);

	Window frame(frame_size.x, frame_size.y, "CTReconstructor Rendering", 0, 0, true, true, true, Frame::CallbackLevel::NOTIFICATION, 0);
	Scene scene(frame);

	std::shared_ptr<Program> volumetric_program = std::make_shared<Program>(
		Shader("../GraphicsCortex/Source/GLSL/Surface/surface.vert",
			   "../GraphicsCortex/Source/GLSL/Surface/surface.geom",
			   "../CTReconstructor/Source/GLSL/VolumetricRendering/volumetric.frag"));
	
	std::shared_ptr<BindlessMaterial> volumetric_material = std::make_shared<BindlessMaterial>(volumetric_program);

	std::shared_ptr<Graphic> box = default_geometry::cube(volumetric_material, glm::vec3(1, 1, 1));
	scene.add(box);

	std::shared_ptr<FBP3D> solver;
	{
		int floating_point_precision = 16;
		std::vector<std::pair<std::string, std::string>> fbp_shader_defines_to_use =
			floating_point_precision == 16 ? _fbp_shader_defines_f16 : _fbp_shader_defines_f32;

		std::vector<std::pair<std::string, std::string>> ffft_shader_defines_to_use =
			floating_point_precision == 16 ? _ffft_shader_defines_f16 : _ffft_shader_defines_f32;

		Texture2D::ColorTextureFormat fbp_volume_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
		Texture2D::ColorTextureFormat fbp_projection_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
		Texture2D::ColorTextureFormat fbp_projection_complex_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::RG16F : Texture2D::ColorTextureFormat::RG32F;

		solver = std::make_shared<FBP3D>(fbp_shader_defines_to_use, ffft_shader_defines_to_use);
	
		solver->set_projections_max_segment_size(glm::ivec3(volume_size.x, volume_size.y / 1, projection_count));
		solver->set_volume_max_segment_size(glm::ivec3(volume_size.x, volume_size.y / 1, volume_size.z));
		solver->set_volume_format(fbp_volume_format_to_use);
		solver->set_projection_format(fbp_projection_format_to_use);
		solver->set_projection_complex_format(fbp_projection_complex_format_to_use);
	}
	
	solver->set_display_progress(true, 128);
	
	solver->read_projections("C:/Users/FurkanPC/Desktop/Projektionen", 2048, 2048, 1, 2, volume_size.x, volume_size.y, projection_count);

	solver->projections_transfer_ram_to_vram();
	solver->log_normalize_projections(95.0 / 255);
	solver->apply_fdk_weights_to_projections(730.87f, 669.04f, 409.60f);
	solver->apply_filter_to_projections(FBP2D::FilterType::RAM_LAK);

	solver->generate_blank_volume(volume_size.x, volume_size.y, volume_size.y);

	solver->project_backward_cone_fdk_from_projections_matrix(730.87f, 669.04f, 409.60f, 213.84f, 213.84f, 1, volume_size.x, volume_size.y, 0);
	solver->clip_negatives_of_volume();
	//solver->normalize_min_max_values();

	solver->projections_clear_ram();
	solver->projections_clear_vram();

	volumetric_program->update_uniform("volume", *solver->volume[0][0][0].get_texture());

	while (frame.is_running()) {
		double deltatime = frame.handle_window();
		frame.clear_window(0, 0, 0);
		frame.display_performance(180);
		
		scene.camera->handle_movements(frame.window, deltatime);

		GLCall(glCullFace(GL_FRONT));
		scene.render();
	}
}