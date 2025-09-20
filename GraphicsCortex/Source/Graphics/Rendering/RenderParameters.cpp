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

	(glBlendFunc(
		blend_source_function == BlendFunction::ZERO						? GL_ZERO					: 
		blend_source_function == BlendFunction::ONE							? GL_ONE					: 
		blend_source_function == BlendFunction::SOURCE_COLOR			   	? GL_SRC_COLOR				: 
		blend_source_function == BlendFunction::ONE_MINUS_SOURCE_COLOR	   	? GL_ONE_MINUS_SRC_COLOR	:	 
		blend_source_function == BlendFunction::DESTINATION_COLOR		   	? GL_DST_COLOR				: 
		blend_source_function == BlendFunction::ONE_MINUS_DESTINATION_COLOR	? GL_ONE_MINUS_DST_COLOR	: 
		blend_source_function == BlendFunction::SOURCE_ALPHA			   	? GL_SRC_ALPHA				: 
		blend_source_function == BlendFunction::ONE_MINUS_SOURCE_ALPHA	   	? GL_ONE_MINUS_SRC_ALPHA	: 
		blend_source_function == BlendFunction::DESTINATION_ALPHA		   	? GL_DST_ALPHA				: 
		blend_source_function == BlendFunction::ONE_MINUS_DESTINATION_ALPHA	? GL_ONE_MINUS_DST_ALPHA	: GL_ZERO,
		
		blend_target_function == BlendFunction::ZERO						? GL_ZERO					: 
		blend_target_function == BlendFunction::ONE							? GL_ONE					: 
		blend_target_function == BlendFunction::SOURCE_COLOR			   	? GL_SRC_COLOR				: 
		blend_target_function == BlendFunction::ONE_MINUS_SOURCE_COLOR	   	? GL_ONE_MINUS_SRC_COLOR	:	 
		blend_target_function == BlendFunction::DESTINATION_COLOR		   	? GL_DST_COLOR				: 
		blend_target_function == BlendFunction::ONE_MINUS_DESTINATION_COLOR	? GL_ONE_MINUS_DST_COLOR	: 
		blend_target_function == BlendFunction::SOURCE_ALPHA			   	? GL_SRC_ALPHA				: 
		blend_target_function == BlendFunction::ONE_MINUS_SOURCE_ALPHA	   	? GL_ONE_MINUS_SRC_ALPHA	: 
		blend_target_function == BlendFunction::DESTINATION_ALPHA		   	? GL_DST_ALPHA				: 
		blend_target_function == BlendFunction::ONE_MINUS_DESTINATION_ALPHA	? GL_ONE_MINUS_DST_ALPHA	: GL_ZERO
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
		cull_face_direction == CullDirection::BACK ? GL_BACK :
		cull_face_direction == CullDirection::FRONT ? GL_FRONT :
		cull_face_direction == CullDirection::FRONT_AND_BACK ? GL_FRONT_AND_BACK : GL_BACK
	));

	

	if (depth_test) {
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(
			depth_function == DepthStencilFunction::NEVER		? GL_NEVER		:
			depth_function == DepthStencilFunction::LESS		? GL_LESS		:
			depth_function == DepthStencilFunction::LEQUAL		? GL_LEQUAL		:
			depth_function == DepthStencilFunction::GREATER		? GL_GREATER	:
			depth_function == DepthStencilFunction::GEQUAL		? GL_GEQUAL		:
			depth_function == DepthStencilFunction::EQUAL		? GL_EQUAL		:
			depth_function == DepthStencilFunction::NOTEQUAL	? GL_NOTEQUAL	:
			depth_function == DepthStencilFunction::ALWAYS		? GL_ALWAYS		: GL_NEVER
		));
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

	if (scissor_test) {
		GLCall(glEnable(GL_SCISSOR_TEST));
		GLCall(glScissor(scissor_viewport.x, scissor_viewport.y, scissor_viewport.z, scissor_viewport.w));
	}
	else {
		GLCall(glDisable(GL_SCISSOR_TEST));
		GLCall(glScissor(0, 0, 1, 1));
	}

	if (stencil_test) {
		GLCall(glEnable(GL_STENCIL_TEST));
	}
	else {
		GLCall(glDisable(GL_STENCIL_TEST));
	}

	GLCall(glStencilFunc(
		stencil_function == DepthStencilFunction::NEVER		? GL_NEVER		:
		stencil_function == DepthStencilFunction::LESS		? GL_LESS		:
		stencil_function == DepthStencilFunction::LEQUAL	? GL_LEQUAL		:
		stencil_function == DepthStencilFunction::GREATER	? GL_GREATER	:
		stencil_function == DepthStencilFunction::GEQUAL	? GL_GEQUAL		:
		stencil_function == DepthStencilFunction::EQUAL		? GL_EQUAL		:
		stencil_function == DepthStencilFunction::NOTEQUAL	? GL_NOTEQUAL	:
		stencil_function == DepthStencilFunction::ALWAYS	? GL_ALWAYS		: GL_NEVER,
		stencil_referance,
		stencil_mask
		));

	GLCall(glStencilOp(
		stencil_operation_on_stencil_fail == StencilOperation::KEEP			? GL_KEEP		:
		stencil_operation_on_stencil_fail == StencilOperation::ZERO			? GL_ZERO		:
		stencil_operation_on_stencil_fail == StencilOperation::REPLACE		? GL_REPLACE	:
		stencil_operation_on_stencil_fail == StencilOperation::INCR			? GL_INCR		:
		stencil_operation_on_stencil_fail == StencilOperation::INCR_WRAP	? GL_INCR_WRAP	:
		stencil_operation_on_stencil_fail == StencilOperation::DECR			? GL_DECR		:
		stencil_operation_on_stencil_fail == StencilOperation::DECR_WRAP	? GL_DECR_WRAP	:
		stencil_operation_on_stencil_fail == StencilOperation::INVERT		? GL_INVERT		: GL_KEEP,

		stencil_operation_on_depth_fail == StencilOperation::KEEP			? GL_KEEP		:
		stencil_operation_on_depth_fail == StencilOperation::ZERO			? GL_ZERO		:
		stencil_operation_on_depth_fail == StencilOperation::REPLACE		? GL_REPLACE	:
		stencil_operation_on_depth_fail == StencilOperation::INCR			? GL_INCR		:
		stencil_operation_on_depth_fail == StencilOperation::INCR_WRAP		? GL_INCR_WRAP	:
		stencil_operation_on_depth_fail == StencilOperation::DECR			? GL_DECR		:
		stencil_operation_on_depth_fail == StencilOperation::DECR_WRAP		? GL_DECR_WRAP	:
		stencil_operation_on_depth_fail == StencilOperation::INVERT			? GL_INVERT		: GL_KEEP,

		stencil_operation_on_pass == StencilOperation::KEEP					? GL_KEEP		:
		stencil_operation_on_pass == StencilOperation::ZERO					? GL_ZERO		:
		stencil_operation_on_pass == StencilOperation::REPLACE				? GL_REPLACE	:
		stencil_operation_on_pass == StencilOperation::INCR					? GL_INCR		:
		stencil_operation_on_pass == StencilOperation::INCR_WRAP			? GL_INCR_WRAP	:
		stencil_operation_on_pass == StencilOperation::DECR					? GL_DECR		:
		stencil_operation_on_pass == StencilOperation::DECR_WRAP			? GL_DECR_WRAP	:
		stencil_operation_on_pass == StencilOperation::INVERT				? GL_INVERT		: GL_KEEP
	));

	GLCall(glPointSize(point_size));
	GLCall(glLineWidth(line_width));

}
