#include "GL/glew.h"

#include "RenderParameters.h"
#include "Debuger.h"

RenderParameters::RenderParameters(bool effective)
{
	this->effective = effective;

	for (int32_t i = 0; i < maximum_clip_plane_count; i++)
		clip_plane[i] = false;
}

void RenderParameters::apply() const
{
	if (!effective)
		return;

	if (blend) {
		GLCall(glEnable(GL_BLEND));
	}
	else {
		GLCall(glDisable(GL_BLEND));
	}

	GLCall(glBlendFunc(
		blend_source_function, 
		blend_target_function
	));

	for (int32_t i = 0; i < maximum_clip_plane_count; i++) {

		if (clip_plane[i]) {
			GLCall(glEnable(GL_CLIP_PLANE0 + i));
		}
		else {
			GLCall(glDisable(GL_CLIP_PLANE0 + i));
		}
	}

	if (cull_face) {
		GLCall(glEnable(GL_CULL_FACE));
	}
	else {
		GLCall(glDisable(GL_CULL_FACE));
	}

	GLCall(glCullFace(
		cull_face_direction == BACK ? GL_BACK :
		cull_face_direction == FRONT ? GL_FRONT :
		cull_face_direction == FRONT_AND_BACK ? GL_FRONT_AND_BACK : GL_BACK
	));

	if (depth_test) {
		GLCall(glEnable(GL_DEPTH_TEST));
	}
	else {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	if (depth_clamp) {
		GLCall(glEnable(GL_DEPTH_CLAMP));
	}
	else {
		GLCall(glDisable(GL_DEPTH_CLAMP));
	}

	GLCall(glDepthRange(
		depth_clamp_near_value, 
		depth_clamp_far_value
	));

	if (framebuffer_srgb) {
		GLCall(glEnable(GL_FRAMEBUFFER_SRGB));
	}
	else {
		GLCall(glDisable(GL_FRAMEBUFFER_SRGB));
	}

	if (multisample) {
		GLCall(glEnable(GL_MULTISAMPLE));
	}
	else {
		GLCall(glDisable(GL_MULTISAMPLE));
	}

	if (primitive_restart) {
		GLCall(glEnable(GL_PRIMITIVE_RESTART));
	}
	else {
		GLCall(glDisable(GL_PRIMITIVE_RESTART));
	}

	if (primitive_restart_fixed_index) {
		GLCall(glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX));
	}
	else {
		GLCall(glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX));
	}

	GLCall(glPrimitiveRestartIndex(primitive_restart_index));

	//if (scissor_test) {
	//	GLCall(glEnable(GL_SCISSOR_TEST));
	//}
	//else {
	//	GLCall(glDisable(GL_SCISSOR_TEST));
	//}

	if (stencil_test) {
		GLCall(glEnable(GL_STENCIL_TEST));
	}
	else {
		GLCall(glDisable(GL_STENCIL_TEST));
	}

	GLCall(glStencilFunc(
		stencil_function == NEVER		? GL_NEVER		:
		stencil_function == LESS		? GL_LESS		:
		stencil_function == LEQUAL		? GL_LEQUAL		:
		stencil_function == GREATER		? GL_GREATER	:
		stencil_function == GEQUAL		? GL_GEQUAL		:
		stencil_function == EQUAL		? GL_EQUAL		:
		stencil_function == NOTEQUAL	? GL_NOTEQUAL	:
		stencil_function == ALWAYS		? GL_ALWAYS		: GL_NEVER,
		stencil_referance,
		stencil_mask
		));

	GLCall(glStencilOp(
		stencil_operation_on_stencil_fail == KEEP			? GL_KEEP		:
		stencil_operation_on_stencil_fail == ZERO			? GL_ZERO		:
		stencil_operation_on_stencil_fail == REPLACE		? GL_REPLACE	:
		stencil_operation_on_stencil_fail == INCR			? GL_INCR		:
		stencil_operation_on_stencil_fail == INCR_WRAP		? GL_INCR_WRAP	:
		stencil_operation_on_stencil_fail == DECR			? GL_DECR		:
		stencil_operation_on_stencil_fail == DECR_WRAP		? GL_DECR_WRAP	:
		stencil_operation_on_stencil_fail == INVERT			? GL_INVERT		: GL_KEEP,

		stencil_operation_on_depth_fail == KEEP				? GL_KEEP		:
		stencil_operation_on_depth_fail == ZERO				? GL_ZERO		:
		stencil_operation_on_depth_fail == REPLACE			? GL_REPLACE	:
		stencil_operation_on_depth_fail == INCR				? GL_INCR		:
		stencil_operation_on_depth_fail == INCR_WRAP		? GL_INCR_WRAP	:
		stencil_operation_on_depth_fail == DECR				? GL_DECR		:
		stencil_operation_on_depth_fail == DECR_WRAP		? GL_DECR_WRAP	:
		stencil_operation_on_depth_fail == INVERT			? GL_INVERT		: GL_KEEP,

		stencil_operation_on_pass == KEEP					? GL_KEEP		:
		stencil_operation_on_pass == ZERO					? GL_ZERO		:
		stencil_operation_on_pass == REPLACE				? GL_REPLACE	:
		stencil_operation_on_pass == INCR					? GL_INCR		:
		stencil_operation_on_pass == INCR_WRAP				? GL_INCR_WRAP	:
		stencil_operation_on_pass == DECR					? GL_DECR		:
		stencil_operation_on_pass == DECR_WRAP				? GL_DECR_WRAP	:
		stencil_operation_on_pass == INVERT					? GL_INVERT		: GL_KEEP
	));
}
