#include "TextureBase.h"
#include "GL/glew.h"

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
