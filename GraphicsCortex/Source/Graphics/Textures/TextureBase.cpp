#include "TextureBase.h"
#include "GL/glew.h"
#include "Debuger.h"
#include <functional>
#include <iostream>

int TextureBase2::SamplingFilter_to_OpenGL(SamplingFilter filter)
{
	switch (filter) {
	case SamplingFilter::NEAREST: return GL_NEAREST;
	case SamplingFilter::LINEAR	: return GL_LINEAR;
	}
	return 0;
}

int TextureBase2::WrapMode_to_OpenGL(WrapMode wrap_mode)
{
	switch (wrap_mode) {
	case WrapMode::CLAMP			: return GL_CLAMP_TO_EDGE;
	case WrapMode::REPEAT			: return GL_REPEAT;
	case WrapMode::MIRRORED_REPEAT	: return GL_MIRRORED_REPEAT;
	}
	return 0;
}

int TextureBase2::ColorTextureFormat_to_OpenGL(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return GL_RED;
	case ColorTextureFormat::RG									:	return GL_RG;
	case ColorTextureFormat::RGB								:	return GL_RGB;
	case ColorTextureFormat::BGR								:	return GL_BGR;
	case ColorTextureFormat::RGBA								:	return GL_RGBA;
	case ColorTextureFormat::BGRA								:	return GL_BGRA;
	case ColorTextureFormat::R8 								:	return GL_R8;
	case ColorTextureFormat::R8_SNORM 							:	return GL_R8_SNORM;
	case ColorTextureFormat::R16 								:	return GL_R16;
	case ColorTextureFormat::R16_SNORM 							:	return GL_R16_SNORM;
	case ColorTextureFormat::RG8 								:	return GL_RG8;
	case ColorTextureFormat::RG8_SNORM 							:	return GL_RG8_SNORM;
	case ColorTextureFormat::RG16 								:	return GL_RG16;
	case ColorTextureFormat::RG16_SNORM 						:	return GL_RG16_SNORM;
	case ColorTextureFormat::R3_G3_B2 							:	return GL_R3_G3_B2;
	case ColorTextureFormat::RGB4 								:	return GL_RGB4;
	case ColorTextureFormat::RGB5 								:	return GL_RGB5;
	case ColorTextureFormat::RGB8 								:	return GL_RGB8;
	case ColorTextureFormat::RGB8_SNORM 						:	return GL_RGB8_SNORM;
	case ColorTextureFormat::RGB10 								:	return GL_RGB10;
	case ColorTextureFormat::RGB12 								:	return GL_RGB12;
	case ColorTextureFormat::RGB16_SNORM 						:	return GL_RGB16_SNORM;
	case ColorTextureFormat::RGBA2 								:	return GL_RGBA2;
	case ColorTextureFormat::RGBA4 								:	return GL_RGBA4;
	case ColorTextureFormat::RGB5_A1 							:	return GL_RGB5_A1;
	case ColorTextureFormat::RGBA8 								:	return GL_RGBA8;
	case ColorTextureFormat::RGBA8_SNORM 						:	return GL_RGBA8_SNORM;
	case ColorTextureFormat::RGB10_A2 							:	return GL_RGB10_A2;
	case ColorTextureFormat::RGB10_A2UI 						:	return GL_RGB10_A2UI;
	case ColorTextureFormat::RGBA12 							:	return GL_RGBA12;
	case ColorTextureFormat::RGBA16 							:	return GL_RGBA16;
	case ColorTextureFormat::SRGB8 								:	return GL_SRGB8;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return GL_SRGB8_ALPHA8;
	case ColorTextureFormat::R16F 								:	return GL_R16F;
	case ColorTextureFormat::RG16F 								:	return GL_RG16F;
	case ColorTextureFormat::RGB16F 							:	return GL_RGB16F;
	case ColorTextureFormat::RGBA16F 							:	return GL_RGBA16F;
	case ColorTextureFormat::R32F 								:	return GL_R32F;
	case ColorTextureFormat::RG32F 								:	return GL_RG32F;
	case ColorTextureFormat::RGB32F 							:	return GL_RGB32F;
	case ColorTextureFormat::RGBA32F 							:	return GL_RGBA32F;
	case ColorTextureFormat::R11F_G11F_B10F						:	return GL_R11F_G11F_B10F;
	case ColorTextureFormat::RGB9_E5 							:	return GL_RGB9_E5;
	case ColorTextureFormat::R8I 								:	return GL_R8I;
	case ColorTextureFormat::R8UI 								:	return GL_R8UI;
	case ColorTextureFormat::R16I 								:	return GL_R16I;
	case ColorTextureFormat::R16UI 								:	return GL_R16UI;
	case ColorTextureFormat::R32I 								:	return GL_R32I;
	case ColorTextureFormat::R32UI 								:	return GL_R32UI;
	case ColorTextureFormat::RG8I 								:	return GL_RG8I;
	case ColorTextureFormat::RG8UI 								:	return GL_RG8UI;
	case ColorTextureFormat::RG16I 								:	return GL_RG16I;
	case ColorTextureFormat::RG16UI 							:	return GL_RG16UI;
	case ColorTextureFormat::RG32I 								:	return GL_RG32I;
	case ColorTextureFormat::RG32UI 							:	return GL_RG32UI;
	case ColorTextureFormat::RGB8I 								:	return GL_RGB8I;
	case ColorTextureFormat::RGB8UI 							:	return GL_RGB8UI;
	case ColorTextureFormat::RGB16I 							:	return GL_RGB16I;
	case ColorTextureFormat::RGB16UI 							:	return GL_RGB16UI;
	case ColorTextureFormat::RGB32I 							:	return GL_RGB32I;
	case ColorTextureFormat::RGB32UI 							:	return GL_RGB32UI;
	case ColorTextureFormat::RGBA8I 							:	return GL_RGBA8I;
	case ColorTextureFormat::RGBA8UI 							:	return GL_RGBA8UI;
	case ColorTextureFormat::RGBA16I 							:	return GL_RGBA16I;
	case ColorTextureFormat::RGBA16UI 							:	return GL_RGBA16UI;
	case ColorTextureFormat::RGBA32I 							:	return GL_RGBA32I;
	case ColorTextureFormat::RGBA32UI 							:	return GL_RGBA32UI;
	case ColorTextureFormat::COMPRESSED_RED						:	return GL_COMPRESSED_RED;
	case ColorTextureFormat::COMPRESSED_RG						:	return GL_COMPRESSED_RG;
	case ColorTextureFormat::COMPRESSED_RGB						:	return GL_COMPRESSED_RGB;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return GL_COMPRESSED_RGBA;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return GL_COMPRESSED_SRGB;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return GL_COMPRESSED_SRGB_ALPHA;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return GL_COMPRESSED_RED_RGTC1;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return GL_COMPRESSED_SIGNED_RED_RGTC1;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return GL_COMPRESSED_RG_RGTC2;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return GL_COMPRESSED_SIGNED_RG_RGTC2;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return GL_COMPRESSED_RGBA_BPTC_UNORM;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
	}
	return 0;
}

int TextureBase2::DepthStencilTextureFormat_to_OpenGL(DepthStencilTextureFormat depth_stencil_texture_format)
{
	switch (depth_stencil_texture_format) {
		case DepthStencilTextureFormat::STENCIL_INDEX		:	return GL_STENCIL_INDEX;
		case DepthStencilTextureFormat::DEPTH_COMPONENT		:	return GL_DEPTH_COMPONENT;
		case DepthStencilTextureFormat::DEPTH_STENCIL		:	return GL_DEPTH_STENCIL;
		case DepthStencilTextureFormat::DEPTH_COMPONENT32F	:	return GL_DEPTH_COMPONENT32F;
		case DepthStencilTextureFormat::DEPTH_COMPONENT24	:	return GL_DEPTH_COMPONENT24;
		case DepthStencilTextureFormat::DEPTH_COMPONENT16	:	return GL_DEPTH_COMPONENT16;
		case DepthStencilTextureFormat::DEPTH32F_STENCIL8	:	return GL_DEPTH32F_STENCIL8;
		case DepthStencilTextureFormat::DEPTH24_STENCIL8	:	return GL_DEPTH24_STENCIL8;
		case DepthStencilTextureFormat::STENCIL_INDEX8		:	return GL_STENCIL_INDEX8;
	}
	return 0;
}

int TextureBase2::Type_to_OpenGL(Type type)
{
	switch (type) {
	case Type::UNSIGNED_BYTE				:	return GL_UNSIGNED_BYTE;
	case Type::BYTE							:	return GL_BYTE; 
	case Type::UNSIGNED_SHORT				:	return GL_UNSIGNED_SHORT;
	case Type::SHORT						:	return GL_SHORT; 
	case Type::UNSIGNED_INT					:	return GL_UNSIGNED_INT; 
	case Type::INT							:	return GL_INT; 
	case Type::FLOAT						:	return GL_FLOAT; 
	case Type::HALF_FLOAT					:	return GL_HALF_FLOAT; 
	case Type::UNSIGNED_BYTE_3_3_2			:	return GL_UNSIGNED_BYTE_3_3_2; 
	case Type::UNSIGNED_BYTE_2_3_3_REV		:	return GL_UNSIGNED_BYTE_2_3_3_REV; 
	case Type::UNSIGNED_SHORT_5_6_5		  	:	return GL_UNSIGNED_SHORT_5_6_5; 
	case Type::UNSIGNED_SHORT_5_6_5_REV	  	:	return GL_UNSIGNED_SHORT_5_6_5_REV; 
	case Type::UNSIGNED_SHORT_4_4_4_4	  	:	return GL_UNSIGNED_SHORT_4_4_4_4; 
	case Type::UNSIGNED_SHORT_4_4_4_4_REV 	:	return GL_UNSIGNED_SHORT_4_4_4_4_REV; 
	case Type::UNSIGNED_SHORT_5_5_5_1	  	:	return GL_UNSIGNED_SHORT_5_5_5_1; 
	case Type::UNSIGNED_SHORT_1_5_5_5_REV 	:	return GL_UNSIGNED_SHORT_1_5_5_5_REV; 
	case Type::UNSIGNED_INT_8_8_8_8		  	:	return GL_UNSIGNED_INT_8_8_8_8; 
	case Type::UNSIGNED_INT_8_8_8_8_REV	  	:	return GL_UNSIGNED_INT_8_8_8_8_REV; 
	case Type::UNSIGNED_INT_10_10_10_2	  	:	return GL_UNSIGNED_INT_10_10_10_2; 
	case Type::UNSIGNED_INT_2_10_10_10_REV	:	return GL_UNSIGNED_INT_2_10_10_10_REV;
	case Type::UNSIGNED_INT_5_9_9_9_REV	  	:	return GL_UNSIGNED_INT_5_9_9_9_REV;
	}
	return 0;
}

int TextureBase2::ColorFormat_to_OpenGL(ColorFormat color_format)
{
	switch (color_format) {
	case ColorFormat::RED			:	return GL_RED;
	case ColorFormat::RG		 	:	return GL_RG;
	case ColorFormat::RGB		 	:	return GL_RGB;
	case ColorFormat::BGR		 	:	return GL_BGR;
	case ColorFormat::RGBA		 	:	return GL_RGBA;
	case ColorFormat::BGRA		 	:	return GL_BGRA;
	case ColorFormat::RED_INTEGER	:	return GL_RED_INTEGER;
	case ColorFormat::RG_INTEGER	:	return GL_RG_INTEGER;
	case ColorFormat::RGB_INTEGER	:	return GL_RGB_INTEGER;
	case ColorFormat::BGR_INTEGER	:	return GL_BGR_INTEGER;
	case ColorFormat::RGBA_INTEGER	:	return GL_RGBA_INTEGER;
	case ColorFormat::BGRA_INTEGER	:	return GL_BGRA_INTEGER;
	}
	return 0;
}

int TextureBase2::Type_bytes_per_channel(Type type) {
	switch (type) {
	case Type::UNSIGNED_BYTE				:	return 1;
	case Type::BYTE							:	return 1; 
	case Type::UNSIGNED_SHORT				:	return 2;
	case Type::SHORT						:	return 2; 
	case Type::UNSIGNED_INT					:	return 4; 
	case Type::INT							:	return 4; 
	case Type::FLOAT						:	return 4; 
	case Type::HALF_FLOAT					:	return 2; 
	case Type::UNSIGNED_BYTE_3_3_2			:	return 1; 
	case Type::UNSIGNED_BYTE_2_3_3_REV		:	return 1; 
	case Type::UNSIGNED_SHORT_5_6_5		  	:	return 2; 
	case Type::UNSIGNED_SHORT_5_6_5_REV	  	:	return 2; 
	case Type::UNSIGNED_SHORT_4_4_4_4	  	:	return 2; 
	case Type::UNSIGNED_SHORT_4_4_4_4_REV 	:	return 2; 
	case Type::UNSIGNED_SHORT_5_5_5_1	  	:	return 2; 
	case Type::UNSIGNED_SHORT_1_5_5_5_REV 	:	return 2; 
	case Type::UNSIGNED_INT_8_8_8_8		  	:	return 4; 
	case Type::UNSIGNED_INT_8_8_8_8_REV	  	:	return 4; 
	case Type::UNSIGNED_INT_10_10_10_2	  	:	return 4; 
	case Type::UNSIGNED_INT_2_10_10_10_REV	:	return 4;
	case Type::UNSIGNED_INT_5_9_9_9_REV	  	:	return 4;
	}
	return 0;
}

int TextureBase2::DepthStencilFormat_to_OpenGL(DepthStencilFormat depth_stencil_format)
{
	switch (depth_stencil_format) {
	case DepthStencilFormat::STENCIL		: return GL_STENCIL;
	case DepthStencilFormat::DEPTH			: return GL_DEPTH;
	case DepthStencilFormat::DEPTH_STENCIL	: return GL_DEPTH_STENCIL;
	}
	return 0;
}

int TextureBase2::ColorFormat_channels(ColorFormat color_format)
{
	switch (color_format) {
	case ColorFormat::RED:	return 1;
	case ColorFormat::RG:	return 2;
	case ColorFormat::RGB:	return 3;
	case ColorFormat::BGR:	return 3;
	case ColorFormat::RGBA:	return 4;
	case ColorFormat::BGRA:	return 4;
	case ColorFormat::RED_INTEGER:	return 1;
	case ColorFormat::RG_INTEGER:	return 2;
	case ColorFormat::RGB_INTEGER:	return 3;
	case ColorFormat::BGR_INTEGER:	return 3;
	case ColorFormat::RGBA_INTEGER:	return 4;
	case ColorFormat::BGRA_INTEGER:	return 4;
	}
	return 0;
}

int TextureBase2::ColorTextureFormat_channels(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return 1;
	case ColorTextureFormat::RG									:	return 2;
	case ColorTextureFormat::RGB								:	return 3;
	case ColorTextureFormat::BGR								:	return 3;
	case ColorTextureFormat::RGBA								:	return 4;
	case ColorTextureFormat::BGRA								:	return 4;
	case ColorTextureFormat::R8 								:	return 1;
	case ColorTextureFormat::R8_SNORM 							:	return 1;
	case ColorTextureFormat::R16 								:	return 1;
	case ColorTextureFormat::R16_SNORM 							:	return 1;
	case ColorTextureFormat::RG8 								:	return 2;
	case ColorTextureFormat::RG8_SNORM 							:	return 2;
	case ColorTextureFormat::RG16 								:	return 2;
	case ColorTextureFormat::RG16_SNORM 						:	return 2;
	case ColorTextureFormat::R3_G3_B2 							:	return 3;
	case ColorTextureFormat::RGB4 								:	return 3;
	case ColorTextureFormat::RGB5 								:	return 3;
	case ColorTextureFormat::RGB8 								:	return 3;
	case ColorTextureFormat::RGB8_SNORM 						:	return 3;
	case ColorTextureFormat::RGB10 								:	return 3;
	case ColorTextureFormat::RGB12 								:	return 3;
	case ColorTextureFormat::RGB16_SNORM 						:	return 3;
	case ColorTextureFormat::RGBA2 								:	return 4;
	case ColorTextureFormat::RGBA4 								:	return 4;
	case ColorTextureFormat::RGB5_A1 							:	return 4;
	case ColorTextureFormat::RGBA8 								:	return 4;
	case ColorTextureFormat::RGBA8_SNORM 						:	return 4;
	case ColorTextureFormat::RGB10_A2 							:	return 4;
	case ColorTextureFormat::RGB10_A2UI 						:	return 4;
	case ColorTextureFormat::RGBA12 							:	return 4;
	case ColorTextureFormat::RGBA16 							:	return 4;
	case ColorTextureFormat::SRGB8 								:	return 3;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return 4;
	case ColorTextureFormat::R16F 								:	return 1;
	case ColorTextureFormat::RG16F 								:	return 2;
	case ColorTextureFormat::RGB16F 							:	return 3;
	case ColorTextureFormat::RGBA16F 							:	return 4;
	case ColorTextureFormat::R32F 								:	return 1;
	case ColorTextureFormat::RG32F 								:	return 2;
	case ColorTextureFormat::RGB32F 							:	return 3;
	case ColorTextureFormat::RGBA32F 							:	return 4;
	case ColorTextureFormat::R11F_G11F_B10F						:	return 3;
	case ColorTextureFormat::RGB9_E5 							:	return 3;
	case ColorTextureFormat::R8I 								:	return 1;
	case ColorTextureFormat::R8UI 								:	return 1;
	case ColorTextureFormat::R16I 								:	return 1;
	case ColorTextureFormat::R16UI 								:	return 1;
	case ColorTextureFormat::R32I 								:	return 1;
	case ColorTextureFormat::R32UI 								:	return 1;
	case ColorTextureFormat::RG8I 								:	return 2;
	case ColorTextureFormat::RG8UI 								:	return 2;
	case ColorTextureFormat::RG16I 								:	return 2;
	case ColorTextureFormat::RG16UI 							:	return 2;
	case ColorTextureFormat::RG32I 								:	return 2;
	case ColorTextureFormat::RG32UI 							:	return 2;
	case ColorTextureFormat::RGB8I 								:	return 3;
	case ColorTextureFormat::RGB8UI 							:	return 3;
	case ColorTextureFormat::RGB16I 							:	return 3;
	case ColorTextureFormat::RGB16UI 							:	return 3;
	case ColorTextureFormat::RGB32I 							:	return 3;
	case ColorTextureFormat::RGB32UI 							:	return 3;
	case ColorTextureFormat::RGBA8I 							:	return 4;
	case ColorTextureFormat::RGBA8UI 							:	return 4;
	case ColorTextureFormat::RGBA16I 							:	return 4;
	case ColorTextureFormat::RGBA16UI 							:	return 4;
	case ColorTextureFormat::RGBA32I 							:	return 4;
	case ColorTextureFormat::RGBA32UI 							:	return 4;
	case ColorTextureFormat::COMPRESSED_RED						:	return 1;
	case ColorTextureFormat::COMPRESSED_RG						:	return 2;
	case ColorTextureFormat::COMPRESSED_RGB						:	return 3;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return 4;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return 3;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return 4;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return 1;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return 1;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return 2;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return 2;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return 4;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return 4;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return 3;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return 3;
	}
	return 0;
}

int TextureBase2::ColorTextureFormat_bytes_per_channel(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return 0;
	case ColorTextureFormat::RG									:	return 0;
	case ColorTextureFormat::RGB								:	return 0;
	case ColorTextureFormat::BGR								:	return 0;
	case ColorTextureFormat::RGBA								:	return 0;
	case ColorTextureFormat::BGRA								:	return 0;
	case ColorTextureFormat::R8 								:	return 1;
	case ColorTextureFormat::R8_SNORM 							:	return 1;
	case ColorTextureFormat::R16 								:	return 2;
	case ColorTextureFormat::R16_SNORM 							:	return 2;
	case ColorTextureFormat::RG8 								:	return 1;
	case ColorTextureFormat::RG8_SNORM 							:	return 1;
	case ColorTextureFormat::RG16 								:	return 2;
	case ColorTextureFormat::RG16_SNORM 						:	return 2;
	case ColorTextureFormat::R3_G3_B2 							:	return 1;
	case ColorTextureFormat::RGB4 								:	return 1;
	case ColorTextureFormat::RGB5 								:	return 1;
	case ColorTextureFormat::RGB8 								:	return 1;
	case ColorTextureFormat::RGB8_SNORM 						:	return 1;
	case ColorTextureFormat::RGB10 								:	return 2;
	case ColorTextureFormat::RGB12 								:	return 2;
	case ColorTextureFormat::RGB16_SNORM 						:	return 2;
	case ColorTextureFormat::RGBA2 								:	return 1;
	case ColorTextureFormat::RGBA4 								:	return 1;
	case ColorTextureFormat::RGB5_A1 							:	return 1;
	case ColorTextureFormat::RGBA8 								:	return 1;
	case ColorTextureFormat::RGBA8_SNORM 						:	return 1;
	case ColorTextureFormat::RGB10_A2 							:	return 2;
	case ColorTextureFormat::RGB10_A2UI 						:	return 2;
	case ColorTextureFormat::RGBA12 							:	return 2;
	case ColorTextureFormat::RGBA16 							:	return 2;
	case ColorTextureFormat::SRGB8 								:	return 1;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return 1;
	case ColorTextureFormat::R16F 								:	return 2;
	case ColorTextureFormat::RG16F 								:	return 2;
	case ColorTextureFormat::RGB16F 							:	return 2;
	case ColorTextureFormat::RGBA16F 							:	return 2;
	case ColorTextureFormat::R32F 								:	return 4;
	case ColorTextureFormat::RG32F 								:	return 4;
	case ColorTextureFormat::RGB32F 							:	return 4;
	case ColorTextureFormat::RGBA32F 							:	return 4;
	case ColorTextureFormat::R11F_G11F_B10F						:	return 2;
	case ColorTextureFormat::RGB9_E5 							:	return 2;
	case ColorTextureFormat::R8I 								:	return 1;
	case ColorTextureFormat::R8UI 								:	return 1;
	case ColorTextureFormat::R16I 								:	return 2;
	case ColorTextureFormat::R16UI 								:	return 2;
	case ColorTextureFormat::R32I 								:	return 4;
	case ColorTextureFormat::R32UI 								:	return 4;
	case ColorTextureFormat::RG8I 								:	return 1;
	case ColorTextureFormat::RG8UI 								:	return 1;
	case ColorTextureFormat::RG16I 								:	return 2;
	case ColorTextureFormat::RG16UI 							:	return 2;
	case ColorTextureFormat::RG32I 								:	return 4;
	case ColorTextureFormat::RG32UI 							:	return 4;
	case ColorTextureFormat::RGB8I 								:	return 1;
	case ColorTextureFormat::RGB8UI 							:	return 1;
	case ColorTextureFormat::RGB16I 							:	return 2;
	case ColorTextureFormat::RGB16UI 							:	return 2;
	case ColorTextureFormat::RGB32I 							:	return 4;
	case ColorTextureFormat::RGB32UI 							:	return 4;
	case ColorTextureFormat::RGBA8I 							:	return 1;
	case ColorTextureFormat::RGBA8UI 							:	return 1;
	case ColorTextureFormat::RGBA16I 							:	return 2;
	case ColorTextureFormat::RGBA16UI 							:	return 2;
	case ColorTextureFormat::RGBA32I 							:	return 4;
	case ColorTextureFormat::RGBA32UI 							:	return 4;
	case ColorTextureFormat::COMPRESSED_RED						:	return 0;
	case ColorTextureFormat::COMPRESSED_RG						:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB						:	return 0;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return 0;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return 0;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return 0;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return 0;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return 0;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return 0;
	}
	return 0;
}

int TextureBase2::ColorTextureFormat_bytes_per_pixel(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return 0;
	case ColorTextureFormat::RG									:	return 0;
	case ColorTextureFormat::RGB								:	return 0;
	case ColorTextureFormat::BGR								:	return 0;
	case ColorTextureFormat::RGBA								:	return 0;
	case ColorTextureFormat::BGRA								:	return 0;
	case ColorTextureFormat::R8 								:	return 1;
	case ColorTextureFormat::R8_SNORM 							:	return 1;
	case ColorTextureFormat::R16 								:	return 2;
	case ColorTextureFormat::R16_SNORM 							:	return 2;
	case ColorTextureFormat::RG8 								:	return 2;
	case ColorTextureFormat::RG8_SNORM 							:	return 2;
	case ColorTextureFormat::RG16 								:	return 4;
	case ColorTextureFormat::RG16_SNORM 						:	return 4;
	case ColorTextureFormat::R3_G3_B2 							:	return 1;
	case ColorTextureFormat::RGB4 								:	return 2;
	case ColorTextureFormat::RGB5 								:	return 2;
	case ColorTextureFormat::RGB8 								:	return 3;
	case ColorTextureFormat::RGB8_SNORM 						:	return 3;
	case ColorTextureFormat::RGB10 								:	return 4;
	case ColorTextureFormat::RGB12 								:	return 5;
	case ColorTextureFormat::RGB16_SNORM 						:	return 6;
	case ColorTextureFormat::RGBA2 								:	return 1;
	case ColorTextureFormat::RGBA4 								:	return 2;
	case ColorTextureFormat::RGB5_A1 							:	return 2;
	case ColorTextureFormat::RGBA8 								:	return 4;
	case ColorTextureFormat::RGBA8_SNORM 						:	return 4;
	case ColorTextureFormat::RGB10_A2 							:	return 4;
	case ColorTextureFormat::RGB10_A2UI 						:	return 4;
	case ColorTextureFormat::RGBA12 							:	return 6;
	case ColorTextureFormat::RGBA16 							:	return 8;
	case ColorTextureFormat::SRGB8 								:	return 3;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return 4;
	case ColorTextureFormat::R16F 								:	return 2;
	case ColorTextureFormat::RG16F 								:	return 4;
	case ColorTextureFormat::RGB16F 							:	return 6;
	case ColorTextureFormat::RGBA16F 							:	return 8;
	case ColorTextureFormat::R32F 								:	return 4;
	case ColorTextureFormat::RG32F 								:	return 8;
	case ColorTextureFormat::RGB32F 							:	return 12;
	case ColorTextureFormat::RGBA32F 							:	return 16;
	case ColorTextureFormat::R11F_G11F_B10F						:	return 4;
	case ColorTextureFormat::RGB9_E5 							:	return 4;
	case ColorTextureFormat::R8I 								:	return 1;
	case ColorTextureFormat::R8UI 								:	return 1;
	case ColorTextureFormat::R16I 								:	return 2;
	case ColorTextureFormat::R16UI 								:	return 2;
	case ColorTextureFormat::R32I 								:	return 4;
	case ColorTextureFormat::R32UI 								:	return 4;
	case ColorTextureFormat::RG8I 								:	return 2;
	case ColorTextureFormat::RG8UI 								:	return 2;
	case ColorTextureFormat::RG16I 								:	return 4;
	case ColorTextureFormat::RG16UI 							:	return 4;
	case ColorTextureFormat::RG32I 								:	return 8;
	case ColorTextureFormat::RG32UI 							:	return 8;
	case ColorTextureFormat::RGB8I 								:	return 3;
	case ColorTextureFormat::RGB8UI 							:	return 3;
	case ColorTextureFormat::RGB16I 							:	return 6;
	case ColorTextureFormat::RGB16UI 							:	return 6;
	case ColorTextureFormat::RGB32I 							:	return 9;
	case ColorTextureFormat::RGB32UI 							:	return 9;
	case ColorTextureFormat::RGBA8I 							:	return 4;
	case ColorTextureFormat::RGBA8UI 							:	return 4;
	case ColorTextureFormat::RGBA16I 							:	return 8;
	case ColorTextureFormat::RGBA16UI 							:	return 8;
	case ColorTextureFormat::RGBA32I 							:	return 16;
	case ColorTextureFormat::RGBA32UI 							:	return 16;
	case ColorTextureFormat::COMPRESSED_RED						:	return 0;
	case ColorTextureFormat::COMPRESSED_RG						:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB						:	return 0;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return 0;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return 0;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return 0;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return 0;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return 0;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return 0;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return 0;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return 0;
	}
	return 0;
}

TextureBase2::ColorFormat TextureBase2::ColorTextureFormat_to_ColorFormat(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return ColorFormat::RED;
	case ColorTextureFormat::RG									:	return ColorFormat::RG;
	case ColorTextureFormat::RGB								:	return ColorFormat::RGB;
	case ColorTextureFormat::BGR								:	return ColorFormat::BGR;
	case ColorTextureFormat::RGBA								:	return ColorFormat::RGBA;
	case ColorTextureFormat::BGRA								:	return ColorFormat::BGRA;
	case ColorTextureFormat::R8 								:	return ColorFormat::RED;
	case ColorTextureFormat::R8_SNORM 							:	return ColorFormat::RED;
	case ColorTextureFormat::R16 								:	return ColorFormat::RED;
	case ColorTextureFormat::R16_SNORM 							:	return ColorFormat::RED;
	case ColorTextureFormat::RG8 								:	return ColorFormat::RG;
	case ColorTextureFormat::RG8_SNORM 							:	return ColorFormat::RG;
	case ColorTextureFormat::RG16 								:	return ColorFormat::RG;
	case ColorTextureFormat::RG16_SNORM 						:	return ColorFormat::RG;
	case ColorTextureFormat::R3_G3_B2 							:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB4 								:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB5 								:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB8 								:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB8_SNORM 						:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB10 								:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB12 								:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB16_SNORM 						:	return ColorFormat::RGB;
	case ColorTextureFormat::RGBA2 								:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGBA4 								:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGB5_A1 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGBA8 								:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGBA8_SNORM 						:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGB10_A2 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGB10_A2UI 						:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGBA12 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::RGBA16 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::SRGB8 								:	return ColorFormat::RGB;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return ColorFormat::RGBA;
	case ColorTextureFormat::R16F 								:	return ColorFormat::RED;
	case ColorTextureFormat::RG16F 								:	return ColorFormat::RG;
	case ColorTextureFormat::RGB16F 							:	return ColorFormat::RGB;
	case ColorTextureFormat::RGBA16F 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::R32F 								:	return ColorFormat::RED;
	case ColorTextureFormat::RG32F 								:	return ColorFormat::RG;
	case ColorTextureFormat::RGB32F 							:	return ColorFormat::RGB;
	case ColorTextureFormat::RGBA32F 							:	return ColorFormat::RGBA;
	case ColorTextureFormat::R11F_G11F_B10F						:	return ColorFormat::RGB;
	case ColorTextureFormat::RGB9_E5 							:	return ColorFormat::RGB;
	case ColorTextureFormat::R8I 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::R8UI 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::R16I 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::R16UI 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::R32I 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::R32UI 								:	return ColorFormat::RED_INTEGER;
	case ColorTextureFormat::RG8I 								:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RG8UI 								:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RG16I 								:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RG16UI 							:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RG32I 								:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RG32UI 							:	return ColorFormat::RG_INTEGER;
	case ColorTextureFormat::RGB8I 								:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGB8UI 							:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGB16I 							:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGB16UI 							:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGB32I 							:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGB32UI 							:	return ColorFormat::RGB_INTEGER;
	case ColorTextureFormat::RGBA8I 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::RGBA8UI 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::RGBA16I 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::RGBA16UI 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::RGBA32I 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::RGBA32UI 							:	return ColorFormat::RGBA_INTEGER;
	case ColorTextureFormat::COMPRESSED_RED						:	return ColorFormat::RED;
	case ColorTextureFormat::COMPRESSED_RG						:	return ColorFormat::RG;
	case ColorTextureFormat::COMPRESSED_RGB						:	return ColorFormat::RGB;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return ColorFormat::RGBA;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return ColorFormat::RGB;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return ColorFormat::RGBA;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return ColorFormat::RED;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return ColorFormat::RED;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return ColorFormat::RG;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return ColorFormat::RG;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return ColorFormat::RGBA;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return ColorFormat::RGBA;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return ColorFormat::RGB;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return ColorFormat::RGB;
	}
	std::cout << "[OpenGL Error] TextureColorFormat conversion failed" << std::endl;
	ASSERT(false);
}

TextureBase2::Type TextureBase2::ColorTextureFormat_to_Type(ColorTextureFormat color_texture_format)
{
	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RG									:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::BGR								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGBA								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::BGRA								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::R8 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::R8_SNORM 							:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::R16 								:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::R16_SNORM 							:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RG8 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RG8_SNORM 							:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RG16 								:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RG16_SNORM 						:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::R3_G3_B2 							:	return Type::UNSIGNED_BYTE_3_3_2;
	case ColorTextureFormat::RGB4 								:	return Type::UNSIGNED_SHORT_4_4_4_4;
	case ColorTextureFormat::RGB5 								:	return Type::UNSIGNED_SHORT_5_5_5_1;
	case ColorTextureFormat::RGB8 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB8_SNORM 						:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB10 								:	return Type::UNSIGNED_INT_10_10_10_2;
	case ColorTextureFormat::RGB12 								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB16_SNORM 						:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RGBA2 								:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGBA4 								:	return Type::UNSIGNED_SHORT_4_4_4_4;
	case ColorTextureFormat::RGB5_A1 							:	return Type::UNSIGNED_SHORT_5_5_5_1;
	case ColorTextureFormat::RGBA8 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGBA8_SNORM 						:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB10_A2 							:	return Type::UNSIGNED_INT_10_10_10_2;
	case ColorTextureFormat::RGB10_A2UI 						:	return Type::UNSIGNED_INT_10_10_10_2;
	case ColorTextureFormat::RGBA12 							:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGBA16 							:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::SRGB8 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::R16F 								:	return Type::HALF_FLOAT;
	case ColorTextureFormat::RG16F 								:	return Type::HALF_FLOAT;
	case ColorTextureFormat::RGB16F 							:	return Type::HALF_FLOAT;
	case ColorTextureFormat::RGBA16F 							:	return Type::HALF_FLOAT;
	case ColorTextureFormat::R32F 								:	return Type::FLOAT;
	case ColorTextureFormat::RG32F 								:	return Type::FLOAT;
	case ColorTextureFormat::RGB32F 							:	return Type::FLOAT;
	case ColorTextureFormat::RGBA32F 							:	return Type::FLOAT;
	case ColorTextureFormat::R11F_G11F_B10F						:	return Type::FLOAT;
	case ColorTextureFormat::RGB9_E5 							:	return Type::UNSIGNED_INT_5_9_9_9_REV;
	case ColorTextureFormat::R8I 								:	return Type::BYTE;
	case ColorTextureFormat::R8UI 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::R16I 								:	return Type::SHORT;
	case ColorTextureFormat::R16UI 								:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::R32I 								:	return Type::INT;
	case ColorTextureFormat::R32UI 								:	return Type::UNSIGNED_INT;
	case ColorTextureFormat::RG8I 								:	return Type::BYTE;
	case ColorTextureFormat::RG8UI 								:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RG16I 								:	return Type::SHORT;
	case ColorTextureFormat::RG16UI 							:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RG32I 								:	return Type::INT;
	case ColorTextureFormat::RG32UI 							:	return Type::UNSIGNED_INT;
	case ColorTextureFormat::RGB8I 								:	return Type::BYTE;
	case ColorTextureFormat::RGB8UI 							:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB16I 							:	return Type::SHORT;
	case ColorTextureFormat::RGB16UI 							:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RGB32I 							:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGB32UI 							:	return Type::UNSIGNED_INT;
	case ColorTextureFormat::RGBA8I 							:	return Type::BYTE;
	case ColorTextureFormat::RGBA8UI 							:	return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::RGBA16I 							:	return Type::SHORT;
	case ColorTextureFormat::RGBA16UI 							:	return Type::UNSIGNED_SHORT;
	case ColorTextureFormat::RGBA32I 							:	return Type::INT;
	case ColorTextureFormat::RGBA32UI 							:	return Type::UNSIGNED_INT;
	case ColorTextureFormat::COMPRESSED_RED						:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RG						:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RGB						:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RGBA					:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_SRGB					:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:	std::cout << "[OpenGL Error] TextureColorFormat to Type conversion failed " << std::endl; ASSERT(false); return Type::UNSIGNED_BYTE;
	}
	std::cout << "[OpenGL Error] TextureColorFormat conversion failed" << std::endl;
	ASSERT(false);
}

bool TextureBase2::is_intager_ColorTextureFormat(ColorTextureFormat format)
{
	switch (format) {
	case ColorTextureFormat::RED								:	return false;
	case ColorTextureFormat::RG									:	return false;
	case ColorTextureFormat::RGB								:	return false;
	case ColorTextureFormat::BGR								:	return false;
	case ColorTextureFormat::RGBA								:	return false;
	case ColorTextureFormat::BGRA								:	return false;
	case ColorTextureFormat::R8 								:	return false;
	case ColorTextureFormat::R8_SNORM 							:	return false;
	case ColorTextureFormat::R16 								:	return false;
	case ColorTextureFormat::R16_SNORM 							:	return false;
	case ColorTextureFormat::RG8 								:	return false;
	case ColorTextureFormat::RG8_SNORM 							:	return false;
	case ColorTextureFormat::RG16 								:	return false;
	case ColorTextureFormat::RG16_SNORM 						:	return false;
	case ColorTextureFormat::R3_G3_B2 							:	return false;
	case ColorTextureFormat::RGB4 								:	return false;
	case ColorTextureFormat::RGB5 								:	return false;
	case ColorTextureFormat::RGB8 								:	return false;
	case ColorTextureFormat::RGB8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10 								:	return false;
	case ColorTextureFormat::RGB12 								:	return false;
	case ColorTextureFormat::RGB16_SNORM 						:	return false;
	case ColorTextureFormat::RGBA2 								:	return false;
	case ColorTextureFormat::RGBA4 								:	return false;
	case ColorTextureFormat::RGB5_A1 							:	return false;
	case ColorTextureFormat::RGBA8 								:	return false;
	case ColorTextureFormat::RGBA8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10_A2 							:	return false;
	case ColorTextureFormat::RGB10_A2UI 						:	return false;
	case ColorTextureFormat::RGBA12 							:	return false;
	case ColorTextureFormat::RGBA16 							:	return false;
	case ColorTextureFormat::SRGB8 								:	return false;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return false;
	case ColorTextureFormat::R16F 								:	return false;
	case ColorTextureFormat::RG16F 								:	return false;
	case ColorTextureFormat::RGB16F 							:	return false;
	case ColorTextureFormat::RGBA16F 							:	return false;
	case ColorTextureFormat::R32F 								:	return false;
	case ColorTextureFormat::RG32F 								:	return false;
	case ColorTextureFormat::RGB32F 							:	return false;
	case ColorTextureFormat::RGBA32F 							:	return false;
	case ColorTextureFormat::R11F_G11F_B10F						:	return false;
	case ColorTextureFormat::RGB9_E5 							:	return false;
	case ColorTextureFormat::R8I 								:	return true;
	case ColorTextureFormat::R8UI 								:	return false;
	case ColorTextureFormat::R16I 								:	return true;
	case ColorTextureFormat::R16UI 								:	return false;
	case ColorTextureFormat::R32I 								:	return true;
	case ColorTextureFormat::R32UI 								:	return false;
	case ColorTextureFormat::RG8I 								:	return true;
	case ColorTextureFormat::RG8UI 								:	return false;
	case ColorTextureFormat::RG16I 								:	return true;
	case ColorTextureFormat::RG16UI 							:	return false;
	case ColorTextureFormat::RG32I 								:	return true;
	case ColorTextureFormat::RG32UI 							:	return false;
	case ColorTextureFormat::RGB8I 								:	return true;
	case ColorTextureFormat::RGB8UI 							:	return false;
	case ColorTextureFormat::RGB16I 							:	return true;
	case ColorTextureFormat::RGB16UI 							:	return false;
	case ColorTextureFormat::RGB32I 							:	return true;
	case ColorTextureFormat::RGB32UI 							:	return false;
	case ColorTextureFormat::RGBA8I 							:	return true;
	case ColorTextureFormat::RGBA8UI 							:	return false;
	case ColorTextureFormat::RGBA16I 							:	return true;
	case ColorTextureFormat::RGBA16UI 							:	return false;
	case ColorTextureFormat::RGBA32I 							:	return true;
	case ColorTextureFormat::RGBA32UI 							:	return false;
	case ColorTextureFormat::COMPRESSED_RED						:	return false;
	case ColorTextureFormat::COMPRESSED_RG						:	return false;
	case ColorTextureFormat::COMPRESSED_RGB						:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return false;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return false;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT :	return false;
	}
	std::cout << "[OpenGL Error] TextureColorFormat conversion failed" << std::endl;
	ASSERT(false);
}

bool TextureBase2::is_unsigned_intager_ColorTextureFormat(ColorTextureFormat format)
{
	switch (format) {
	case ColorTextureFormat::RED								:	return false;
	case ColorTextureFormat::RG									:	return false;
	case ColorTextureFormat::RGB								:	return false;
	case ColorTextureFormat::BGR								:	return false;
	case ColorTextureFormat::RGBA								:	return false;
	case ColorTextureFormat::BGRA								:	return false;
	case ColorTextureFormat::R8 								:	return false;
	case ColorTextureFormat::R8_SNORM 							:	return false;
	case ColorTextureFormat::R16 								:	return false;
	case ColorTextureFormat::R16_SNORM 							:	return false;
	case ColorTextureFormat::RG8 								:	return false;
	case ColorTextureFormat::RG8_SNORM 							:	return false;
	case ColorTextureFormat::RG16 								:	return false;
	case ColorTextureFormat::RG16_SNORM 						:	return false;
	case ColorTextureFormat::R3_G3_B2 							:	return false;
	case ColorTextureFormat::RGB4 								:	return false;
	case ColorTextureFormat::RGB5 								:	return false;
	case ColorTextureFormat::RGB8 								:	return false;
	case ColorTextureFormat::RGB8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10 								:	return false;
	case ColorTextureFormat::RGB12 								:	return false;
	case ColorTextureFormat::RGB16_SNORM 						:	return false;
	case ColorTextureFormat::RGBA2 								:	return false;
	case ColorTextureFormat::RGBA4 								:	return false;
	case ColorTextureFormat::RGB5_A1 							:	return false;
	case ColorTextureFormat::RGBA8 								:	return false;
	case ColorTextureFormat::RGBA8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10_A2 							:	return false;
	case ColorTextureFormat::RGB10_A2UI 						:	return false;
	case ColorTextureFormat::RGBA12 							:	return false;
	case ColorTextureFormat::RGBA16 							:	return false;
	case ColorTextureFormat::SRGB8 								:	return false;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return false;
	case ColorTextureFormat::R16F 								:	return false;
	case ColorTextureFormat::RG16F 								:	return false;
	case ColorTextureFormat::RGB16F 							:	return false;
	case ColorTextureFormat::RGBA16F 							:	return false;
	case ColorTextureFormat::R32F 								:	return false;
	case ColorTextureFormat::RG32F 								:	return false;
	case ColorTextureFormat::RGB32F 							:	return false;
	case ColorTextureFormat::RGBA32F 							:	return false;
	case ColorTextureFormat::R11F_G11F_B10F						:	return false;
	case ColorTextureFormat::RGB9_E5 							:	return false;
	case ColorTextureFormat::R8I 								:	return false;
	case ColorTextureFormat::R8UI 								:	return true;
	case ColorTextureFormat::R16I 								:	return false;
	case ColorTextureFormat::R16UI 								:	return true;
	case ColorTextureFormat::R32I 								:	return false;
	case ColorTextureFormat::R32UI 								:	return true;
	case ColorTextureFormat::RG8I 								:	return false;
	case ColorTextureFormat::RG8UI 								:	return true;
	case ColorTextureFormat::RG16I 								:	return false;
	case ColorTextureFormat::RG16UI 							:	return true;
	case ColorTextureFormat::RG32I 								:	return false;
	case ColorTextureFormat::RG32UI 							:	return true;
	case ColorTextureFormat::RGB8I 								:	return false;
	case ColorTextureFormat::RGB8UI 							:	return true;
	case ColorTextureFormat::RGB16I 							:	return false;
	case ColorTextureFormat::RGB16UI 							:	return true;
	case ColorTextureFormat::RGB32I 							:	return false;
	case ColorTextureFormat::RGB32UI 							:	return true;
	case ColorTextureFormat::RGBA8I 							:	return false;
	case ColorTextureFormat::RGBA8UI 							:	return true;
	case ColorTextureFormat::RGBA16I 							:	return false;
	case ColorTextureFormat::RGBA16UI 							:	return true;
	case ColorTextureFormat::RGBA32I 							:	return false;
	case ColorTextureFormat::RGBA32UI 							:	return true;
	case ColorTextureFormat::COMPRESSED_RED						:	return false;
	case ColorTextureFormat::COMPRESSED_RG						:	return false;
	case ColorTextureFormat::COMPRESSED_RGB						:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return false;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return false;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT :	return false;
	}
	std::cout << "[OpenGL Error] TextureColorFormat conversion failed" << std::endl;
	ASSERT(false);
}

bool TextureBase2::is_float_ColorTextureFormat(ColorTextureFormat format)
{
	switch (format) {
	case ColorTextureFormat::RED								:	return false;
	case ColorTextureFormat::RG									:	return false;
	case ColorTextureFormat::RGB								:	return false;
	case ColorTextureFormat::BGR								:	return false;
	case ColorTextureFormat::RGBA								:	return false;
	case ColorTextureFormat::BGRA								:	return false;
	case ColorTextureFormat::R8 								:	return false;
	case ColorTextureFormat::R8_SNORM 							:	return false;
	case ColorTextureFormat::R16 								:	return false;
	case ColorTextureFormat::R16_SNORM 							:	return false;
	case ColorTextureFormat::RG8 								:	return false;
	case ColorTextureFormat::RG8_SNORM 							:	return false;
	case ColorTextureFormat::RG16 								:	return false;
	case ColorTextureFormat::RG16_SNORM 						:	return false;
	case ColorTextureFormat::R3_G3_B2 							:	return false;
	case ColorTextureFormat::RGB4 								:	return false;
	case ColorTextureFormat::RGB5 								:	return false;
	case ColorTextureFormat::RGB8 								:	return false;
	case ColorTextureFormat::RGB8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10 								:	return false;
	case ColorTextureFormat::RGB12 								:	return false;
	case ColorTextureFormat::RGB16_SNORM 						:	return false;
	case ColorTextureFormat::RGBA2 								:	return false;
	case ColorTextureFormat::RGBA4 								:	return false;
	case ColorTextureFormat::RGB5_A1 							:	return false;
	case ColorTextureFormat::RGBA8 								:	return false;
	case ColorTextureFormat::RGBA8_SNORM 						:	return false;
	case ColorTextureFormat::RGB10_A2 							:	return false;
	case ColorTextureFormat::RGB10_A2UI 						:	return false;
	case ColorTextureFormat::RGBA12 							:	return false;
	case ColorTextureFormat::RGBA16 							:	return false;
	case ColorTextureFormat::SRGB8 								:	return false;
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return false;
	case ColorTextureFormat::R16F 								:	return true;
	case ColorTextureFormat::RG16F 								:	return true;
	case ColorTextureFormat::RGB16F 							:	return true;
	case ColorTextureFormat::RGBA16F 							:	return true;
	case ColorTextureFormat::R32F 								:	return true;
	case ColorTextureFormat::RG32F 								:	return true;
	case ColorTextureFormat::RGB32F 							:	return true;
	case ColorTextureFormat::RGBA32F 							:	return true;
	case ColorTextureFormat::R11F_G11F_B10F						:	return true;
	case ColorTextureFormat::RGB9_E5 							:	return false;
	case ColorTextureFormat::R8I 								:	return false;
	case ColorTextureFormat::R8UI 								:	return false;
	case ColorTextureFormat::R16I 								:	return false;
	case ColorTextureFormat::R16UI 								:	return false;
	case ColorTextureFormat::R32I 								:	return false;
	case ColorTextureFormat::R32UI 								:	return false;
	case ColorTextureFormat::RG8I 								:	return false;
	case ColorTextureFormat::RG8UI 								:	return false;
	case ColorTextureFormat::RG16I 								:	return false;
	case ColorTextureFormat::RG16UI 							:	return false;
	case ColorTextureFormat::RG32I 								:	return false;
	case ColorTextureFormat::RG32UI 							:	return false;
	case ColorTextureFormat::RGB8I 								:	return false;
	case ColorTextureFormat::RGB8UI 							:	return false;
	case ColorTextureFormat::RGB16I 							:	return false;
	case ColorTextureFormat::RGB16UI 							:	return false;
	case ColorTextureFormat::RGB32I 							:	return false;
	case ColorTextureFormat::RGB32UI 							:	return false;
	case ColorTextureFormat::RGBA8I 							:	return false;
	case ColorTextureFormat::RGBA8UI 							:	return false;
	case ColorTextureFormat::RGBA16I 							:	return false;
	case ColorTextureFormat::RGBA16UI 							:	return false;
	case ColorTextureFormat::RGBA32I 							:	return false;
	case ColorTextureFormat::RGBA32UI 							:	return false;
	case ColorTextureFormat::COMPRESSED_RED						:	return false;
	case ColorTextureFormat::COMPRESSED_RG						:	return false;
	case ColorTextureFormat::COMPRESSED_RGB						:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB					:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return false;
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return false;
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return false;
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return false;
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return false;
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return false;
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT :	return false;
	}
	std::cout << "[OpenGL Error] TextureColorFormat conversion failed" << std::endl;
	ASSERT(false);
}

bool TextureBase2::is_intager_ColorFormat(ColorFormat format)
{
	return false;
}

std::string TextureBase2::ColorTextureFormat_to_OpenGL_compute_Image_format(ColorTextureFormat color_texture_format){

	switch (color_texture_format) {
	case ColorTextureFormat::RED								:	return "";
	case ColorTextureFormat::RG									:	return "";
	case ColorTextureFormat::RGB								:	return "";
	case ColorTextureFormat::BGR								:	return "";
	case ColorTextureFormat::RGBA								:	return "";
	case ColorTextureFormat::BGRA								:	return "";
	case ColorTextureFormat::R8 								:	return "r8";
	case ColorTextureFormat::R8_SNORM 							:	return "r8_snorm";
	case ColorTextureFormat::R16 								:	return "r16";
	case ColorTextureFormat::R16_SNORM 							:	return "r16_snorm";
	case ColorTextureFormat::RG8 								:	return "rg8";
	case ColorTextureFormat::RG8_SNORM 							:	return "rg8_snorm";
	case ColorTextureFormat::RG16 								:	return "rg16";
	case ColorTextureFormat::RG16_SNORM 						:	return "rg16_snorm";
	case ColorTextureFormat::R3_G3_B2 							:	return "";
	case ColorTextureFormat::RGB4 								:	return "";
	case ColorTextureFormat::RGB5 								:	return "";
	case ColorTextureFormat::RGB8 								:	return "";
	case ColorTextureFormat::RGB8_SNORM 						:	return "";
	case ColorTextureFormat::RGB10 								:	return "";
	case ColorTextureFormat::RGB12 								:	return "";
	case ColorTextureFormat::RGB16_SNORM 						:	return "";
	case ColorTextureFormat::RGBA2 								:	return "";
	case ColorTextureFormat::RGBA4 								:	return "";
	case ColorTextureFormat::RGB5_A1 							:	return "";
	case ColorTextureFormat::RGBA8 								:	return "rgba8";
	case ColorTextureFormat::RGBA8_SNORM 						:	return "rgba8_snorm";
	case ColorTextureFormat::RGB10_A2 							:	return "rgb10_a2";
	case ColorTextureFormat::RGB10_A2UI 						:	return "rgb10_a2ui";
	case ColorTextureFormat::RGBA12 							:	return "";
	case ColorTextureFormat::RGBA16 							:	return "rgba16";
	case ColorTextureFormat::SRGB8 								:	return "";
	case ColorTextureFormat::SRGB8_ALPHA8 						:	return "rgba8";	//?
	case ColorTextureFormat::R16F 								:	return "r16f";
	case ColorTextureFormat::RG16F 								:	return "rg16f";
	case ColorTextureFormat::RGB16F 							:	return "";
	case ColorTextureFormat::RGBA16F 							:	return "rgba16f";
	case ColorTextureFormat::R32F 								:	return "r32f";
	case ColorTextureFormat::RG32F 								:	return "rg32f";
	case ColorTextureFormat::RGB32F 							:	return "";
	case ColorTextureFormat::RGBA32F 							:	return "rgba32f";
	case ColorTextureFormat::R11F_G11F_B10F						:	return "r11f_g11f_b10f";
	case ColorTextureFormat::RGB9_E5 							:	return "";
	case ColorTextureFormat::R8I 								:	return "r8i";
	case ColorTextureFormat::R8UI 								:	return "r8ui";
	case ColorTextureFormat::R16I 								:	return "r16i";
	case ColorTextureFormat::R16UI 								:	return "r16ui";
	case ColorTextureFormat::R32I 								:	return "r32i";
	case ColorTextureFormat::R32UI 								:	return "r32ui";
	case ColorTextureFormat::RG8I 								:	return "rg8i";
	case ColorTextureFormat::RG8UI 								:	return "rg8ui";
	case ColorTextureFormat::RG16I 								:	return "rg16i";
	case ColorTextureFormat::RG16UI 							:	return "rg16ui";
	case ColorTextureFormat::RG32I 								:	return "rg32i";
	case ColorTextureFormat::RG32UI 							:	return "rg32ui";
	case ColorTextureFormat::RGB8I 								:	return "";
	case ColorTextureFormat::RGB8UI 							:	return "";
	case ColorTextureFormat::RGB16I 							:	return "";
	case ColorTextureFormat::RGB16UI 							:	return "";
	case ColorTextureFormat::RGB32I 							:	return "";
	case ColorTextureFormat::RGB32UI 							:	return "";
	case ColorTextureFormat::RGBA8I 							:	return "rgba8i";
	case ColorTextureFormat::RGBA8UI 							:	return "rgba8ui";
	case ColorTextureFormat::RGBA16I 							:	return "rgba16i";
	case ColorTextureFormat::RGBA16UI 							:	return "rgba16ui";
	case ColorTextureFormat::RGBA32I 							:	return "rgba32i";
	case ColorTextureFormat::RGBA32UI 							:	return "rgba32ui";
	case ColorTextureFormat::COMPRESSED_RED						:	return "";
	case ColorTextureFormat::COMPRESSED_RG						:	return "";
	case ColorTextureFormat::COMPRESSED_RGB						:	return "";
	case ColorTextureFormat::COMPRESSED_RGBA					:	return "";
	case ColorTextureFormat::COMPRESSED_SRGB					:	return "";
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA				:	return "";
	case ColorTextureFormat::COMPRESSED_RED_RGTC1				:	return "";
	case ColorTextureFormat::COMPRESSED_SIGNED_RED_RGTC1		:	return "";
	case ColorTextureFormat::COMPRESSED_RG_RGTC2				:	return "";
	case ColorTextureFormat::COMPRESSED_SIGNED_RG_RGTC2			:	return "";
	case ColorTextureFormat::COMPRESSED_RGBA_BPTC_UNORM			:	return "";
	case ColorTextureFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM	:	return "";
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT	:	return "";
	case ColorTextureFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT	:	return "";
	}
}

