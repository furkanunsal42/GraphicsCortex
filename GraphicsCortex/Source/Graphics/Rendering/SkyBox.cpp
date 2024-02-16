#include "SkyBox.h"
#include "Default_Programs.h"

SkyBox::SkyBox()
{
	box->material->set_program_without_texture_reset(default_program::cubemap_program_s());
}

void SkyBox::render(Camera& camera)
{
	cubemap->wait_async_load();

	camera.update_matrixes();
	camera.update_default_uniforms(*box->material->program);
	box->update_default_uniforms(*box->material->program);
	box->material->program->update_uniform(cubemap_sampler_name, *cubemap);

	bool face_culling = glIsEnabled(GL_CULL_FACE);
	int depth_function;
	glGetIntegerv(GL_DEPTH_FUNC, &depth_function);
	GLCall(glDisable(GL_CULL_FACE));
	GLCall(glDepthFunc(GL_LEQUAL));

	box->draw(false);

	if (face_culling) {
		GLCall(glEnable(GL_CULL_FACE));
	}
	GLCall(glDepthFunc(depth_function));
}
