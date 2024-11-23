#pragma once

#include "RenderParameters.h"

class Framebuffer;
class Program;
class Buffer;
class Mesh2;

namespace primitive_renderer {

	void render(Program& program, Mesh2& mesh);

}