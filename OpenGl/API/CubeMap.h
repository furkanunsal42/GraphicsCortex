#pragma once

#include "Texture.h"
#include "Graphic.h"
#include "Default_Assets.h"
#include "Camera.h"
#include "ShaderCompiler.h"

#include <string>
#include <unordered_map>

enum cube_map {
	RIGHT = 0, LEFT, TOP, BOTTOM, FRONT, BACK
};

class CubeMapTexture{
public:
	Graphic cube = default_geometry::cube(glm::vec3(2, 2, 2));
	Camera* camera = nullptr;
	Program* program = nullptr;

	unsigned int id = 0;
	// right left top bottom front back for every array
	int width[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	int height[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	int channels[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
	unsigned char* image_data[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

	std::string face_texture_filepaths[6]{"", "", "", "", "", ""};
	
	bool vertical_flip[6] = { false, false, false, false, false, false };
	unsigned int min_filter = GL_NEAREST;
	unsigned int mag_filter = GL_NEAREST;
	unsigned int wrap_s = GL_CLAMP_TO_EDGE;
	unsigned int wrap_t = GL_CLAMP_TO_EDGE;
	unsigned int wrap_r = GL_CLAMP_TO_EDGE;
	unsigned int internal_format = NULL;
	unsigned int format = NULL;
	unsigned int data_type = GL_UNSIGNED_BYTE;
	int texture_slot = 11;
	CubeMapTexture();
	void load_data(int desired_channels = 3, bool free_ram = false);
	void free_image_ram();
	void draw();
	void bind();
	void unbind();
};