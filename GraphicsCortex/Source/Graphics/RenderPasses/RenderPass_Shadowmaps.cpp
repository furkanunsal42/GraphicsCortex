#include "RenderPass_Shadowmaps.h"

#include "Components/Component_Light.h"
#include "Components/Component_Skylight.h"

#include "UniformBuffer.h"
#include "PrimitiveRenderer.h"
#include "Component/Entity.h"
#include "Constants.h"

const std::string RenderPass_Shadowmaps::directional_shadowmap_texture_name = "__d_shadowmaps";

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
		shadowmap_format != d_shadowmaps->get_internal_format_color()
		) {
		d_shadowmaps = std::make_shared<Texture2DArray>(
			d_shadowmap_resolution, d_shadowmap_resolution,
			d_shadowmap_cascade_count,
			shadowmap_format, 1, 0, 0
		);
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

		Camera camera;
		camera.update_matrixes();
		camera.update_default_uniforms(program);

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

std::array<glm::vec3, 8> RenderPass_Shadowmaps::get_frustum_corners(Camera& camera, float z_percentage, int32_t index) {

	const glm::mat4 inverse_matrix = glm::inverse(camera.projection_matrix * camera.view_matrix);

	std::array<glm::vec3, 8> frustum_corners;

	for (unsigned int x = 0; x < 2; ++x) {
		for (unsigned int y = 0; y < 2; ++y) {
			for (unsigned int z = 0; z < 2; ++z) {
				const glm::vec4 corner4 =
					inverse_matrix * glm::vec4(
						2.0f * x - 1.0f,
						2.0f * y - 1.0f,
						2.0f * ((z_percentage * index) + (z * z_percentage)) - 1.0f,
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

				float z_percentage = 1.0f / d_shadowmap_cascade_count;

				std::array<glm::vec3, 8> frustum_corners = get_frustum_corners(camera, 0.25f, 1);
				glm::vec3 frustum_center = get_frustum_center(frustum_corners);

				glm::mat4 light_view_matrix = glm::lookAt(
					frustum_center,
					frustum_center + glm::normalize(light_transform.get_z_direction()),
					glm::vec3(0, 1, 0)
				);

				glm::mat4 light_projection_ortho = get_projection_matrix_from_frustum_ortho(frustum_corners, light_view_matrix, 100);

				framebuffer->attach_color(0, *d_shadowmaps, cascade, 0);
				framebuffer->activate_draw_buffer(0);
				framebuffer->bind_draw();

				primitive_renderer::clear(0, 0, 0, 1);

				render_scene(mesh_renderer_components, *depth_program, light_view_matrix, light_projection_ortho);
			}

			break;
		case LightComponent::point:
			break;
		case LightComponent::spot:
			break;
		}
	}

	pipeline.textures_map[directional_shadowmap_texture_name] = d_shadowmaps;
}
