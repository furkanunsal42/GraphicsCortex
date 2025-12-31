#include "RenderPass_Shadowmaps.h"

#include "Components/Component_Light.h"
#include "Components/Component_Skylight.h"

#include "UniformBuffer.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Constants.h"

const std::string RenderPass_Shadowmaps::directional_shadowmap_texture_name = "__d_shadowmaps";
const std::string RenderPass_Shadowmaps::directional_shadowmap_buffer_name = "__d_shadowmaps_buffer";

void RenderPass_Shadowmaps::on_initialize(int pass_index, RenderPipeline& pipeline, Scene& scene) {

	depth_program_cubemap = std::make_shared<Program>(
		Shader(
			default_paths::shadowmaps / "point_shadowmap.vert",
			default_paths::shadowmaps / "point_shadowmap.geom",
			default_paths::shadowmaps / "point_shadowmap.frag"
		)
	);

	depth_program = std::make_shared<Program>(
		Shader(
			default_paths::shadowmaps / "shadowmap.vert",
			default_paths::shadowmaps / "shadowmap.frag"
		)
	);

	if (framebuffer == nullptr)
		framebuffer = std::make_shared<Framebuffer>();

	
}

void RenderPass_Shadowmaps::init_shadowmaps() {
	if (framebuffer == nullptr)
		framebuffer = std::make_shared<Framebuffer>();

	if (d_shadowmaps == nullptr ||
		d_shadowmap_resolution != d_shadowmaps->get_size().x ||
		d_shadowmap_cascade_count != d_shadowmaps->get_size().z ||
		shadowmap_format != d_shadowmaps->get_internal_format_depthstencil()
		) {
		d_shadowmaps = std::make_shared<Texture2DArray>(
			d_shadowmap_resolution, d_shadowmap_resolution,
			d_shadowmap_cascade_count,
			shadowmap_format, 1, 0, 0
		);
	}
	//if (d_shadowmaps_color == nullptr ||
	//	d_shadowmap_resolution != d_shadowmaps_color->get_size().x ||
	//	d_shadowmap_cascade_count != d_shadowmaps_color->get_size().z
	//	) {
	//	d_shadowmaps_color = std::make_shared<Texture2DArray>(
	//		d_shadowmap_resolution, d_shadowmap_resolution,
	//		d_shadowmap_cascade_count,
	//		Texture2D::ColorTextureFormat::RGBA8, 1, 0, 0
	//	);
	//}

	if (d_shadowmaps_buffer == nullptr) {
		d_shadowmaps_buffer = std::make_shared<UniformBuffer>();
		for (int i = 0; i < d_shadowmap_max_count * d_shadowmap_cascade_count; i++)
			d_shadowmaps_buffer->push_variable<glm::mat4>();
		d_shadowmaps_buffer->push_variable_array(d_shadowmap_max_count * d_shadowmap_cascade_count); // distances
		d_shadowmaps_buffer->push_variable<int32_t>();
	}

}

void RenderPass_Shadowmaps::render_scene(std::span<MeshRendererComponent*> mesh_renderer_components, Program& program, const glm::mat4& view_matrix, const glm::mat4& projection_matrix) {

	if (mesh_renderer_components.size() == 0)
		return;

	//framebuffer->bind_draw();

	for (MeshRendererComponent* mesh_renderer : mesh_renderer_components) {

		TransformComponent* transform_c = mesh_renderer->get_transform();
		if (transform_c == nullptr) continue;
		glm::mat4 model_matrix = transform_c->get_matrix();

		MeshComponent* mesh_c = mesh_renderer->get_mesh();
		if (mesh_c == nullptr) continue;

		std::shared_ptr<Mesh> mesh = mesh_c->mesh;
		if (mesh == nullptr) continue;

		RenderParameters render_parameters;

		program.update_uniform("view", view_matrix);
		program.update_uniform("projection", projection_matrix);

		mesh->traverse([&](Mesh::Node& node, glm::mat4& transform) {

			if (node.get_submeshes().size() == 0) return;

			program.update_uniform("model", model_matrix * transform);

			for (mesh_t submesh : node.get_submeshes()) {

				primitive_renderer::render(
					program,
					*mesh->get_mesh(submesh),
					render_parameters
				);
			}
			});
	}
}

std::array<glm::vec3, 8> RenderPass_Shadowmaps::get_frustum_corners(Camera& camera, float near_plane, float far_plane) {

	const auto clipped_projection = glm::perspective(glm::radians(camera.fov), camera.screen_width / camera.screen_height, near_plane, far_plane);
	const glm::mat4 inverse_matrix = glm::inverse(clipped_projection * camera.view_matrix);

	std::array<glm::vec3, 8> frustum_corners;

	for (unsigned int x = 0; x < 2; ++x) {
		for (unsigned int y = 0; y < 2; ++y) {
			for (unsigned int z = 0; z < 2; ++z) {
				const glm::vec4 corner4 =
					inverse_matrix * glm::vec4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * z - 1.0f,
						1.0f);
				frustum_corners[z * 2 * 2 + y * 2 + x] = glm::vec3(corner4) / corner4.w;
			}
		}
	}

	return frustum_corners;
}

glm::vec3 RenderPass_Shadowmaps::get_frustum_center(const std::array<glm::vec3, 8>& frustum_corners) {

	glm::vec3 center = glm::vec3(0, 0, 0);

	for (const glm::vec3& v : frustum_corners)
		center += v;

	center /= frustum_corners.size();

	return center;
}

glm::mat4 RenderPass_Shadowmaps::get_projection_matrix_from_frustum_ortho(const std::array<glm::vec3, 8>& frustum_corners, const glm::mat4& light_view_matrix, float z_multiplier) {
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const glm::vec3& v : frustum_corners)
	{
		const auto trf = light_view_matrix * glm::vec4(v, 1);
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	if (minZ < 0)
		minZ *= z_multiplier;
	else
		minZ /= z_multiplier;

	if (maxZ < 0)
		maxZ /= z_multiplier;
	else
		maxZ *= z_multiplier;

	glm::mat4 light_projection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	return light_projection;
}

void RenderPass_Shadowmaps::on_render(int pass_index, RenderPipeline& pipeline, Scene& scene, Camera& camera) {

	std::span<MeshRendererComponent*> mesh_renderer_components = scene.get_components<MeshRendererComponent>();
	std::span<LightComponent*> light_components = scene.get_components<LightComponent>();

	uint32_t d_light_count = 0;
	uint32_t p_light_count = 0;
	uint32_t s_light_count = 0;

	// count the light types
	for (LightComponent* light : light_components) {

		Entity* entity = light->get_entity();
		if (entity == nullptr) {
			ASSERT(false);
		}
		std::shared_ptr<TransformComponent> transform_c = entity->get_component<TransformComponent>();
		if (transform_c == nullptr)
			continue;

		switch (light->type) {
		case LightComponent::directional:
			d_light_count++;
			break;
		case LightComponent::point:
			p_light_count++;
			break;
		case LightComponent::spot:
			s_light_count++;
			break;
		}
	}

	uint32_t d_light_index = 0;
	uint32_t p_light_index = 0;
	uint32_t s_light_index = 0;

	init_shadowmaps();
	camera.update_matrixes();

	int32_t d_light_count_times_cascade = d_light_count * d_shadowmap_cascade_count;
	d_shadowmaps_buffer->set_data(
		d_shadowmap_cascade_count * d_shadowmap_max_count * sizeof(glm::mat4) +
		d_shadowmap_max_count * d_shadowmap_cascade_count * sizeof(glm::vec4),
		0,
		sizeof(int32_t),
		&d_light_count_times_cascade);

	std::vector<float> cascade_far_planes{
		camera.max_distance / 80.0f, 
		camera.max_distance / 70.0f, 
		camera.max_distance / 60.0f, 
		camera.max_distance / 50.0f,
		camera.max_distance / 40.0f,
		camera.max_distance / 30.0f,
		camera.max_distance / 20.0f,
		camera.max_distance / 10.0f,
		camera.max_distance / 5.0f,
		camera.max_distance / 3.0f 
	};

	for (LightComponent* light : light_components) {
		Entity* entity = light->get_entity();
		if (entity == nullptr) {
			ASSERT(false);
		}
		std::shared_ptr<TransformComponent> light_transform_c = entity->get_component<TransformComponent>();
		if (light_transform_c == nullptr)
			continue;

		TransformComponent& light_transform = *light_transform_c;


		switch (light->type) {
		case LightComponent::directional:

			for (int32_t cascade = 0; cascade < d_shadowmap_cascade_count; cascade++) {

				std::array<glm::vec3, 8> frustum_corners;
				float cascade_max_distance;
				if (cascade == 0) {
					cascade_max_distance = cascade_far_planes[cascade];
					frustum_corners = get_frustum_corners(camera, camera.min_distance, cascade_max_distance);
				}
				else if (cascade < cascade_far_planes.size()) {
					cascade_max_distance = cascade_far_planes[cascade];
					frustum_corners = get_frustum_corners(camera, cascade_far_planes[cascade-1], cascade_max_distance);
				}
				else if (cascade >= cascade_far_planes.size()) {
					cascade_max_distance = camera.max_distance;
					frustum_corners = get_frustum_corners(camera, cascade_far_planes[cascade_far_planes.size()-1], cascade_max_distance);
				}
				
				glm::vec3 frustum_center = get_frustum_center(frustum_corners);

				glm::mat4 light_view_matrix = glm::lookAt(
					frustum_center,
					frustum_center + glm::normalize(light_transform.get_z_direction()),
					glm::vec3(0, 1, 0)
				);

				glm::mat4 light_projection_ortho = get_projection_matrix_from_frustum_ortho(frustum_corners, light_view_matrix, 100);

				glm::mat4 projection_view = light_projection_ortho * light_view_matrix;
				d_shadowmaps_buffer->set_data(
					sizeof(glm::mat4) * cascade, 
					0,
					sizeof(glm::mat4),
					&projection_view
				);
				d_shadowmaps_buffer->set_data(
					d_shadowmap_cascade_count * d_shadowmap_max_count * sizeof(glm::mat4) +
					cascade * sizeof(glm::vec4),
					0, 
					sizeof(float), 
					&cascade_max_distance
				);

				//framebuffer->attach_color(0, *d_shadowmaps_color, cascade, 0);
				framebuffer->attach_depth(*d_shadowmaps, cascade, 0);
				framebuffer->activate_draw_buffer(0);
				framebuffer->bind_draw();

				glm::vec4 viewport = primitive_renderer::get_viewport_position_size();
				primitive_renderer::set_viewport_size(glm::ivec2(d_shadowmap_resolution));
				primitive_renderer::set_viewport_position(glm::ivec2(0));

				primitive_renderer::clear(0, 0, 0, 1);

				glEnable(GL_DEPTH_TEST);
				glCullFace(GL_BACK);
				glCullFace(GL_FRONT);
				render_scene(mesh_renderer_components, *depth_program, light_view_matrix, light_projection_ortho);
				glCullFace(GL_BACK);
				
				framebuffer->deattach_color(0);
				framebuffer->deattach_depth();

				primitive_renderer::set_viewport(viewport);
			}

			break;
		case LightComponent::point:
			break;
		case LightComponent::spot:
			break;
		}

	}
	
	d_shadowmaps_buffer->upload_data();
	pipeline.uniform_buffers_map[directional_shadowmap_buffer_name] = d_shadowmaps_buffer;
	pipeline.textures_map[directional_shadowmap_texture_name] = d_shadowmaps;
}
