#pragma once
#include "Config.h"
#include "Image.h"
#include "glm.hpp"

class TextureBase2{
public:
	enum class SamplingFilter {
		NEAREST,
		LINEAR,
	};

	enum class WrapMode {
		CLAMP,
		REPEAT,
		MIRRORED_REPEAT,
	};

	enum class ColorTextureFormat {
		//Base Internal Formats
		RED,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,

		// Sized Internal Formats
		// Format			// Type
		R8 				,	//	GL_RED 			8 				
		R8_SNORM 		,	//	GL_RED 			s8 				
		R16 			,	//	GL_RED 			16 				
		R16_SNORM 		,	//	GL_RED 			s16 				
		RG8 			,	//	GL_RG 			8 		8 			
		RG8_SNORM 		,	//	GL_RG 			s8 		s8 			
		RG16 			,	//	GL_RG 			16 		16 			
		RG16_SNORM 		,	//	GL_RG 			s16				s16 			
		R3_G3_B2 		,	//	GL_RGB 			3 		3 		2 		
		RGB4 			,	//	GL_RGB 			4 		4 		4 		
		RGB5 			,	//	GL_RGB 			5 		5 		5 		
		RGB8 			,	//	GL_RGB 			8 		8 		8 		
		RGB8_SNORM 		,	//	GL_RGB 			s8 		s8 		s8 		
		RGB10 			,	//	GL_RGB 			10 		10 		10 		
		RGB12 			,	//	GL_RGB 			12 		12 		12 		
		RGB16_SNORM 	,	//	GL_RGB 			16 		16 		16 		
		RGBA2 			,	//	GL_RGB 			2 		2 		2 		2 	
		RGBA4 			,	//	GL_RGB 			4 		4 		4 		4 	
		RGB5_A1 		,	//	GL_RGBA 		5 		5 		5 		1 	
		RGBA8 			,	//	GL_RGBA 		8 		8 		8 		8 	
		RGBA8_SNORM 	,	//	GL_RGBA 		s8 		s8 		s8 		s8 	
		RGB10_A2 		,	//	GL_RGBA 		10 		10 		10 		2 	
		RGB10_A2UI 		,	//	GL_RGBA 		ui10	 		ui10	ui10	ui2 	
		RGBA12 			,	//	GL_RGBA 		12 		12 		12 		12 	
		RGBA16 			,	//	GL_RGBA 		16 		16 		16 		16 	
		SRGB8 			,	//	GL_RGB 			8 		8 		8 			
		SRGB8_ALPHA8 	,	//	GL_RGBA 		8 		8 		8 		8 	
		R16F 			,	//	GL_RED 			f16 				
		RG16F 			,	//	GL_RG 			f16 	f16 			
		RGB16F 			,	//	GL_RGB 			f16 	f16 	f16 		
		RGBA16F 		,	//	GL_RGBA 		f16 	f16 	f16 	f16 	
		R32F 			,	//	GL_RED 			f32 				
		RG32F 			,	//	GL_RG 			f32 	f32 			
		RGB32F 			,	//	GL_RGB 			f32 	f32 	f32 		
		RGBA32F 		,	//	GL_RGBA 		f32 	f32 	f32 	f32 	
		R11F_G11F_B10F 	,	//	GL_RGB 			f11 	f11 	f10 		
		RGB9_E5 		,	//	GL_RGB 			9 		9 		9 		5
		R8I 			,	//	GL_RED 			i8 				
		R8UI 			,	//	GL_RED 			ui8 				
		R16I 			,	//	GL_RED 			i16 				
		R16UI 			,	//	GL_RED 			ui16 				
		R32I 			,	//	GL_RED 			i32 				
		R32UI 			,	//	GL_RED 			ui32 				
		RG8I 			,	//	GL_RG 			i8 		i8 			
		RG8UI 			,	//	GL_RG 			ui8 	ui8 			
		RG16I 			,	//	GL_RG 			i16 	i16 			
		RG16UI 			,	//	GL_RG 			ui16 	ui16 			
		RG32I 			,	//	GL_RG 			i32 	i32 			
		RG32UI 			,	//	GL_RG 			ui32 	ui32 			
		RGB8I 			,	//	GL_RGB 			i8 		i8 		i8 		
		RGB8UI 			,	//	GL_RGB 			ui8 	ui8 	ui8 		
		RGB16I 			,	//	GL_RGB 			i16 	i16 	i16 		
		RGB16UI 		,	//	GL_RGB 			ui16 	ui16 	ui16 		
		RGB32I 			,	//	GL_RGB 			i32 	i32 	i32 		
		RGB32UI 		,	//	GL_RGB 			ui32 	ui32 	ui32 		
		RGBA8I 			,	//	GL_RGBA 		i8 		i8 		i8 		i8 	
		RGBA8UI 		,	//	GL_RGBA 		ui8 	ui8 	ui8 	ui8 	
		RGBA16I 		,	//	GL_RGBA 		i16 	i16 	i16 	i16 	
		RGBA16UI 		,	//	GL_RGBA 		ui16 	ui16 	ui16 	ui16 	
		RGBA32I 		,	//	GL_RGBA 		i32 	i32 	i32 	i32 	
		RGBA32UI 		,	//	GL_RGBA 		ui32 	ui32 	ui32 	ui32

		// Compressed Generic Internal Formats
		COMPRESSED_RED,
		COMPRESSED_RG,
		COMPRESSED_RGB,
		COMPRESSED_RGBA,
		COMPRESSED_SRGB,
		COMPRESSED_SRGB_ALPHA,

		// Compressed Spesific Internal Formats
		COMPRESSED_RED_RGTC1,
		COMPRESSED_SIGNED_RED_RGTC1,
		COMPRESSED_RG_RGTC2,
		COMPRESSED_SIGNED_RG_RGTC2,
		COMPRESSED_RGBA_BPTC_UNORM,
		COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
		COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
		COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
	};

	enum class DepthStencilTextureFormat {
		// Base Internal Formats
		STENCIL_INDEX,
		DEPTH_COMPONENT,
		DEPTH_STENCIL,

		// Sized Internal Formats
		DEPTH_COMPONENT32F,
		DEPTH_COMPONENT24,
		DEPTH_COMPONENT16,
		DEPTH32F_STENCIL8,
		DEPTH24_STENCIL8,
	};

	enum class Type {
		UNSIGNED_BYTE,
		BYTE, 
		UNSIGNED_SHORT,
		SHORT, 
		UNSIGNED_INT, 
		INT, 
		FLOAT, 
		UNSIGNED_BYTE_3_3_2, 
		UNSIGNED_BYTE_2_3_3_REV, 
		UNSIGNED_SHORT_5_6_5, 
		UNSIGNED_SHORT_5_6_5_REV, 
		UNSIGNED_SHORT_4_4_4_4, 
		UNSIGNED_SHORT_4_4_4_4_REV, 
		UNSIGNED_SHORT_5_5_5_1, 
		UNSIGNED_SHORT_1_5_5_5_REV, 
		UNSIGNED_INT_8_8_8_8, 
		UNSIGNED_INT_8_8_8_8_REV, 
		UNSIGNED_INT_10_10_10_2, 
		UNSIGNED_INT_2_10_10_10_REV,
		UNSIGNED_INT_5_9_9_9_REV,		//*
	};

	enum class ColorFormat {
		RED,
		RG, 
		RGB, 
		BGR, 
		RGBA, 
		BGRA, 
		RED_INTEGER, 
		RG_INTEGER, 
		RGB_INTEGER, 
		BGR_INTEGER, 
		RGBA_INTEGER, 
		BGRA_INTEGER, 
	};

	enum class DepthStencilFormat {
		STENCIL,
		DEPTH,
		DEPTH_STENCIL,
	};

	enum class WellDefinedColorFormat {
		// Sized Internal Formats
		// Format			// Type
		R8_SNORM 		,	//	GL_RED 			s8 				
		R16_SNORM 		,	//	GL_RED 			s16 				
		RG8_SNORM 		,	//	GL_RG 			s8 		s8 			
		RG16_SNORM 		,	//	GL_RG 			s16				s16 			
		RGB8_SNORM 		,	//	GL_RGB 			s8 		s8 		s8 		
		RGBA8_SNORM 	,	//	GL_RGBA 		s8 		s8 		s8 		s8 	
		RGB10_A2UI 		,	//	GL_RGBA 		ui10	 		ui10	ui10	ui2 	
		R16F 			,	//	GL_RED 			f16 				
		RG16F 			,	//	GL_RG 			f16 	f16 			
		RGB16F 			,	//	GL_RGB 			f16 	f16 	f16 		
		RGBA16F 		,	//	GL_RGBA 		f16 	f16 	f16 	f16 	
		R32F 			,	//	GL_RED 			f32 				
		RG32F 			,	//	GL_RG 			f32 	f32 			
		RGB32F 			,	//	GL_RGB 			f32 	f32 	f32 		
		RGBA32F 		,	//	GL_RGBA 		f32 	f32 	f32 	f32 	
		R11F_G11F_B10F 	,	//	GL_RGB 			f11 	f11 	f10 		
		R8I 			,	//	GL_RED 			i8 				
		R8UI 			,	//	GL_RED 			ui8 				
		R16I 			,	//	GL_RED 			i16 				
		R16UI 			,	//	GL_RED 			ui16 				
		R32I 			,	//	GL_RED 			i32 				
		R32UI 			,	//	GL_RED 			ui32 				
		RG8I 			,	//	GL_RG 			i8 		i8 			
		RG8UI 			,	//	GL_RG 			ui8 	ui8 			
		RG16I 			,	//	GL_RG 			i16 	i16 			
		RG16UI 			,	//	GL_RG 			ui16 	ui16 			
		RG32I 			,	//	GL_RG 			i32 	i32 			
		RG32UI 			,	//	GL_RG 			ui32 	ui32 			
		RGB8I 			,	//	GL_RGB 			i8 		i8 		i8 		
		RGB8UI 			,	//	GL_RGB 			ui8 	ui8 	ui8 		
		RGB16I 			,	//	GL_RGB 			i16 	i16 	i16 		
		RGB16UI 		,	//	GL_RGB 			ui16 	ui16 	ui16 		
		RGB32I 			,	//	GL_RGB 			i32 	i32 	i32 		
		RGB32UI 		,	//	GL_RGB 			ui32 	ui32 	ui32 		
		RGBA8I 			,	//	GL_RGBA 		i8 		i8 		i8 		i8 	
		RGBA8UI 		,	//	GL_RGBA 		ui8 	ui8 	ui8 	ui8 	
		RGBA16I 		,	//	GL_RGBA 		i16 	i16 	i16 	i16 	
		RGBA16UI 		,	//	GL_RGBA 		ui16 	ui16 	ui16 	ui16 	
		RGBA32I 		,	//	GL_RGBA 		i32 	i32 	i32 	i32 	
		RGBA32UI 		,	//	GL_RGBA 		ui32 	ui32 	ui32 	ui32
	};

	static int SamplingFilter_to_OpenGL(SamplingFilter filter);
	static int WrapMode_to_OpenGL(WrapMode wrap_mode);
	static int ColorTextureFormat_to_OpenGL(ColorTextureFormat color_texture_format);
	static int DepthStencilTextureFormat_to_OpenGL(DepthStencilTextureFormat depth_stencil_texture_format);
	static int Type_to_OpenGL(Type type);
	static int ColorFormat_to_OpenGL(ColorFormat color_format);
	static int DepthStencilFormat_to_OpenGL(DepthStencilFormat depth_stencil_format);
	static int ColorFormat_channels(ColorFormat color_format);
	//static int WellDefinedFormat_to_OpenGL(WellDefinedColorFormat well_defined_color_format);

	unsigned int id = 0;
	uint64_t texture_handle = 0;

	TextureBase2() {};
	~TextureBase2() {};
	virtual void release() = 0;
	virtual void bind(int texture_slot) = 0;
	virtual void unbind() = 0;

	virtual SamplingFilter query_mag_filter() = 0;
	virtual SamplingFilter query_min_filter() = 0;
	virtual SamplingFilter query_mipmap_min_filter() = 0;
	virtual WrapMode query_wrap_u() = 0;
	virtual WrapMode query_wrap_v() = 0;
	virtual WrapMode query_wrap_w() = 0;
	virtual int query_base_level() = 0;
	//virtual int query_compare_mode() = 0;
	//virtual query_compare_function() = 0;
	virtual float query_lod_bias() = 0;
	virtual int query_max_level() = 0;
	virtual int query_max_lod() = 0;
	virtual int query_min_lod() = 0;
	virtual int query_swizzle_r() = 0;
	virtual int query_swizzle_g() = 0;
	virtual int query_swizzle_b() = 0;
	virtual int query_swizzle_a() = 0;
	virtual int query_swizzle_rgba() = 0;

	virtual int query_width(int mipmap_level) = 0;
	virtual int query_height(int mipmap_level) = 0;
	virtual int query_depth(int mipmap_level) = 0;
	virtual int query_internal_format(int mipmap_level) = 0;
	virtual int query_red_type(int mipmap_level) = 0;
	virtual int query_green_type(int mipmap_level) = 0;
	virtual int query_blue_type(int mipmap_level) = 0;
	virtual int query_alpha_type(int mipmap_level) = 0;
	virtual int query_depth_type(int mipmap_level) = 0;
	virtual int query_red_size(int mipmap_level) = 0;
	virtual int query_green_size(int mipmap_level) = 0;
	virtual int query_blue_size(int mipmap_level) = 0;
	virtual int query_alpha_size(int mipmap_level) = 0;
	virtual int query_depth_size(int mipmap_level) = 0;
	virtual bool query_is_compressed(int mipmap_level) = 0;
	virtual int query_compressed_image_size(int mipmap_level) = 0;

private:
	int target = NULL;
};
