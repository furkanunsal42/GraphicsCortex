#include "GraphicsCortex.h"
#include "FilteredBackProjection/FBP3D.h"
#include "FilteredBackProjection/FBP3DSegmentedMemory.h"
#include "Application/ProgramSourcePaths.h"
#include "Application/ReconstructionInfo.h"
#include "DirectoryUtils.h"
#include "TextureArithmatic/TextureArithmatic.h"
#include "TextureArithmatic/GraphicsOperation.h"

using namespace shader_directory;


int main() {

	glm::ivec3 volume_dimentions(512, 512, 512);
	glm::vec3 voxel_size(200.0f / volume_dimentions.x, 200.0f / volume_dimentions.y, 200.0f / volume_dimentions.z);
	int projection_count = 1440;
	int window_width = 512;

	int slice_index_number_length = 6;
	std::string reconstruction_path = "reconstruction";

	Frame frame(window_width, window_width, "CTReconstructor", 0, 0, true, true, false, Frame::CallbackLevel::DISABLED, false);
	Scene scene(frame);
	scene.camera->fov = 90;
	scene.camera->max_distance = 1000;

	int floating_point_precision = 16;
	std::vector<std::pair<std::string, std::string>> fbp_shader_defines_to_use =
		floating_point_precision == 16 ? _fbp_shader_defines_f16 : _fbp_shader_defines_f32;

	std::vector<std::pair<std::string, std::string>> ffft_shader_defines_to_use =
		floating_point_precision == 16 ? _ffft_shader_defines_f16 : _ffft_shader_defines_f32;

	Texture2D::ColorTextureFormat fbp_volume_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
	Texture2D::ColorTextureFormat fbp_projection_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::R16F : Texture2D::ColorTextureFormat::R32F;
	Texture2D::ColorTextureFormat fbp_distanced_projection_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::RGBA16F : Texture2D::ColorTextureFormat::RGBA32F;
	Texture2D::ColorTextureFormat fbp_projection_complex_format_to_use = floating_point_precision == 16 ? Texture2D::ColorTextureFormat::RG16F : Texture2D::ColorTextureFormat::RG32F;

	fbp_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FBP/";
	ffft_shader_directory = "../CTReconstructor/Source/GLSL/Compute/FFT/";
	util_shader_directory = "../CTReconstructor/Source/GLSL/Compute/Util/";

	std::shared_ptr<FBP3D> solver = std::make_shared<FBP3D>(fbp_shader_defines_to_use, ffft_shader_defines_to_use);
	solver->set_projections_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y / 1, projection_count));
	solver->set_distanced_projections_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y / 1, projection_count));
	solver->set_volume_max_segment_size(glm::ivec3(volume_dimentions.x, volume_dimentions.y / 1, volume_dimentions.z));

	solver->set_volume_format(fbp_volume_format_to_use);
	solver->set_projection_format(fbp_projection_format_to_use);
	solver->set_distanced_projection_format(fbp_distanced_projection_format_to_use);
	solver->set_projection_complex_format(fbp_projection_complex_format_to_use);

	solver->set_display_progress(true, 128);

	std::shared_ptr<Texture1D> min_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	min_texture->is_bindless = false;
	std::shared_ptr<Texture1D> max_texture = std::make_shared<Texture1D>(1, solver->histogram_int_texture_internal_format, 1, 0);
	max_texture->is_bindless = false;

	solver->read_projections("C:/Users/furkan.unsal/Desktop/Projektionen", 2048, 2048, 1, 2, volume_dimentions.x, volume_dimentions.y, projection_count);

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, false, true, [&](glm::ivec3 projection_segment_index) {
		solver->log_normalize_projections(95.0 / 255);
		solver->apply_fdk_weights_to_projections(730.87f, 669.04f, 409.60f);
		solver->apply_filter_to_projections(FBP2D::FilterType::RAM_LAK);
		});

	solver->generate_blank_volume(volume_dimentions.x, volume_dimentions.y, volume_dimentions.z);

	fbp_segmented_memory::iterate_horizontal_projection_segments(*solver, true, false, [&](glm::ivec3 projection_index) {
		fbp_segmented_memory::iterate_horizontal_volume_segments_per_projection(*solver, projection_index.y, [&](glm::ivec3 volume_segment_index) {
			if (solver->get_volume_segment_count() == glm::ivec3(1, 1, 1))
				solver->projections_clear_ram();
			solver->project_backward_cone_fdk_from_projections_matrix(730.87f, 669.04f, 409.60f, 213.84f, 213.84f, 1, volume_dimentions.x, volume_dimentions.y, 0);
			solver->compute_min_value_of_volume(*min_texture);
			solver->compute_max_value_of_volume(*max_texture);
			});
		});

	solver->projections_clear_ram();
	solver->projections_clear_vram();

	fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {
		solver->clip_negatives_of_volume();
		//solver->normalize_min_max_values(*min_texture, *max_texture);
		});

	GraphicsOperation op;

	Texture1D min_tex(1, Texture1D::ColorTextureFormat::R32I, 1, 0);
	Texture1D max_tex(1, Texture1D::ColorTextureFormat::R32I, 1, 0);
	solver->compute_min_value_of_volume(min_tex);
	solver->compute_max_value_of_volume(max_tex);

	int32_t sensitivity = 256 * 256 * 256;
	int32_t histogram_min = *(int32_t*)min_tex.get_image(Texture1D::ColorFormat::RED_INTEGER, Texture1D::Type::INT, 0)->get_image_data() / sensitivity;
	int32_t histogram_max = *(int32_t*)max_tex.get_image(Texture1D::ColorFormat::RED_INTEGER, Texture1D::Type::INT, 0)->get_image_data() / sensitivity;

	std::cout << histogram_min << " " << histogram_max << std::endl;

	Texture1D histogram(4096, Texture1D::ColorTextureFormat::R32UI, 1, 0);
	solver->bh_compute_volume_histogram(glm::vec2(-256, 256/*histogram_min, histogram_max*/), histogram);

	std::shared_ptr<Image> image = histogram.get_image(Texture1D::ColorFormat::RED_INTEGER, Texture1D::Type::UNSIGNED_INT, 0);
	std::shared_ptr<Texture2D> histogram_render = std::make_shared<Texture2D>(4096, window_width, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);

	op.compute(
		*histogram_render,
		histogram, false,
		"vec4 source_prev = imageLoad(source_data, source_index_type(id.x - 1));"
		"bool condution = id.y < (log(source.x));"
		"imageStore(target_data, target_index_type(id), vec4(int(condution)));"
	);

	std::shared_ptr<Texture2D> preview = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_volume_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> preview_w = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);

	Framebuffer fb;

	//fb.attach_color(0, histogram_render);
	fb.attach_color(0, preview_w);
	int i = 0;
	while (frame.is_running()) {
		frame.handle_window();
		frame.clear_window(1, 0, 1, 1);

		std::function<void(GLFWwindow * window, double xoffset, double yoffset)> func;

	

		i = i % solver->get_volume_size().y;

		solver->load_volume_slice_y(i, *preview);
		op.compute(
			*preview_w,
			*preview, false,
			"source.xxxx"
		);

		fb.activate_draw_buffer(0);
		fb.blit_to_screen(0, 0, /*4096*/ window_width, window_width, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::NEAREST);
	}

	solver->generate_blank_distanced_projections(volume_dimentions.x, volume_dimentions.y, volume_dimentions.y/*projection_count*/, 2);
	solver->bh_project_forward_parallel_to_distanced_projections({ 2, 15, 30, 100 }, 1, volume_dimentions.y/*projection_count*/, 0);
	
	//solver->bh_compute_distance_attenuation_graph_from_distanced_projections(128, 128, 40, 40);

	//{
	//	std::shared_ptr<VertexAttributeBuffer> vab = std::make_shared<VertexAttributeBuffer>();
	//	vab->attach_vertex_buffer(0, solver->distance_attenuation_table, VertexAttributeBuffer::a_f32, 3, sizeof(glm::vec3), 0, true);
	//
	//	std::shared_ptr<Program> program = std::make_shared<Program>(Shader("../GraphicsCortex/Source/GLSL/Debug/basic.vert", "../GraphicsCortex/Source/GLSL/Debug/flatcolor.frag"));
	//	program->update_uniform("model", glm::identity<glm::mat4x4>());
	//	program->update_uniform("color", glm::vec4(1, 1, 0, 1));
	//
	//	while (frame.is_running()) {
	//		double deltatime = frame.handle_window();
	//		frame.clear_window();
	//		frame.display_performance();
	//
	//		scene.camera->handle_movements(frame.window, deltatime);
	//		scene.camera->update_matrixes();
	//		scene.camera->update_default_uniforms(*program);
	//
	//		primitive_renderer::render(*program, *vab, PrimitiveType::point);
	//	}
	//}
	

	ReconstructionInfo info;
	info.name = compute_filename(reconstruction_path);
	info.volume_resolution = volume_dimentions;
	info.voxel_size_mm = voxel_size;
	info.save_to_disc(reconstruction_path);

	//solver->project_forward_parallel(1, projection_count, 0);

	std::shared_ptr<Texture2D> slice = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_volume_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_complex = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_projection_complex_format(), 1, 0, 0);
	std::shared_ptr<Texture2D> slice_white = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0);
	std::shared_ptr<Texture2D> slice_distanced = std::make_shared<Texture2D>(volume_dimentions.x, volume_dimentions.z, solver->get_distanced_projection_format(), 1, 0, 0);
	TextureArithmatic arithmatic;

	std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>();

	frame.set_visibility(true);
	while (true) {
		fbp_segmented_memory::iterate_horizontal_volume_segments(*solver, false, false, [&](glm::ivec3 volume_segment_index) {

			int segment_height = solver->get_volume_max_segment_size().y;
			for (int slice_id = volume_segment_index.y * segment_height; slice_id < (volume_segment_index.y + 1) * segment_height; slice_id++) {
				if (!frame.is_running()) exit(0);
				double delta_time = frame.handle_window();
				frame.clear_window();
				frame.display_performance(180);

				//solver->load_volume_slice_y(slice_id, *slice);
				//solver->load_projection(slice_id, *slice);
				//solver->load_sinogram(slice_id, *slice);
				solver->load_distanced_projection(slice_id, 1, *slice_distanced);

				//solver->fbp_solver->_texture_blit_float1_to_float4(*slice, *slice_white);
				arithmatic.operation_binary(*slice_white, *slice_distanced, "vec4(operand.xyz, 1) / 200");

				framebuffer->attach_color(0, slice_white);
				framebuffer->set_read_buffer(0);
				framebuffer->blit_to_screen(0, 0, volume_dimentions.x, volume_dimentions.z, 0, 0, window_width, window_width, Framebuffer::Channel::COLOR, Framebuffer::Filter::LINEAR);
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
			}
			});
	}
}