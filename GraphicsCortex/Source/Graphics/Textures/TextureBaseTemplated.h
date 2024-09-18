#pragma once

class Texture1D;
class Texture2D;
class Texture2DArray;
class TextureCubeMap;
class Texture3D;
class Renderbuffer;

template<typename TextureType>
std::string TextureBase2::ColorTextureFormat_to_OpenGL_compute_Sampler_type() {

	static_assert(!std::is_base_of<TextureBase2, TextureType>(), "[OpenGL Error] TextureBase::ColorTextureFormat_to_OpenGL_compute_Sampler_type<>() "
		"called but TextureType must be called with a Texture type");

	if (std::is_same<TextureType, Texture1D>)		return "sampler1D";
	if (std::is_same<TextureType, Texture2D>) 		return "sampler2D";
	if (std::is_same<TextureType, Texture2DArray>) 	return "sampler2DArray";
	if (std::is_same<TextureType, TextureCubeMap>) 	return "samplerCube";
	if (std::is_same<TextureType, Texture3D>) 		return "sampler3D";
	if (std::is_same<TextureType, Renderbuffer>) 	return "";

	return "";
}

template<typename TextureType>
int TextureBase2::get_texture_dimention()
{
	static_assert(!std::is_base_of<TextureBase2, TextureType>(), "[OpenGL Error] TextureBase::get_texture_dimention<>() "
		"called but TextureType must be called with a Texture type");

	if (std::is_same<TextureType, Texture1D>)		return 1;
	if (std::is_same<TextureType, Texture2D>) 		return 2; 
	if (std::is_same<TextureType, Texture2DArray>) 	return 3; 
	if (std::is_same<TextureType, TextureCubeMap>) 	return 3; 
	if (std::is_same<TextureType, Texture3D>) 		return 3; 
	if (std::is_same<TextureType, Renderbuffer>) 	return 2;

	return 0;
}

template<typename TextureType>
std::string TextureBase2::ColorTextureFormat_to_OpenGL_compute_Image_type(ColorTextureFormat color_texture_format) {

	static_assert(!std::is_base_of<TextureBase2, TextureType>(), "[OpenGL Error] TextureBase::ColorTextureFormat_to_OpenGL_compute_Image_type<>() "
		"called but TextureType must be called with a Texture type");

	std::string image_format = ColorTextureFormat_to_OpenGL_compute_Image_format(color_texture_format);
	if (image_format == "") return "";
	
	for (int i = 0; i < image_format.size(); i++)
		image_format[i] = std::tolower(image_format[i]);

	bool is_int;
	bool is_unsigned_int;
	bool is_float;
	
	if (image_format.size() >= 2)
		if (image_format.substr(image_format.size() - 2, 2) == "ui")
			is_unsigned_int = true;

	if (image_format.size() >= 1)
		if (image_format.substr(image_format.size() - 2, 2) == "i" && !is_unsigned_int)
			is_int = true;

	if (image_format.size() >= 1)
		if (image_format.substr(image_format.size() - 2, 2) == "f")
			is_int = true;

	std::string prefix = "";
	
	if (is_int) prefix = "i";
	if (is_unsigned_int) prefix = "u";
	if (is_float) prefix = "";

	if (std::is_same<TextureType, Texture1D>)		return prefix + "image1D";
	if (std::is_same<TextureType, Texture2D>) 		return prefix + "image2D";
	if (std::is_same<TextureType, Texture2DArray>) 	return prefix + "image2DArray";
	if (std::is_same<TextureType, TextureCubeMap>) 	return prefix + "imageCube";
	if (std::is_same<TextureType, Texture3D>) 		return prefix + "image3D";
	if (std::is_same<TextureType, Renderbuffer>) 	return "";

	return "";
}
