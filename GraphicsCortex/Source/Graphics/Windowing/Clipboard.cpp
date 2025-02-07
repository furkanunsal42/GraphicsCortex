#include "Clipboard.h"
#include <GLFW/glfw3.h>
#include <Library.h>

std::string clipboard::get()
{
	OpenGLBackend::_init_glfw();
	
	const char* clipboard_str = glfwGetClipboardString(nullptr);
	return std::string(clipboard_str);
}

void clipboard::set(const std::string& value)
{
	OpenGLBackend::_init_glfw();

	glfwSetClipboardString(nullptr, value.c_str());
}
