#pragma once

#include "GraphicsCortex.h"
#include "TestBench/TestBench.h"

#pragma comment (lib, "Dwmapi")

#define NOMINMAX
#include "Windows.h"
#include "dwmapi.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

//WNDPROC original_proc;
//
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch (uMsg)
//	{
//	case WM_NCCALCSIZE:
//	{
//		// Remove the window's standard sizing border
//		if (wParam == TRUE && lParam != NULL)
//		{
//			NCCALCSIZE_PARAMS* pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
//			pParams->rgrc[0].top += 1;
//			pParams->rgrc[0].right -= 1;
//			pParams->rgrc[0].bottom -= 1;
//			pParams->rgrc[0].left += 1;
//		}
//		return 0;
//	}
//	case WM_NCPAINT:
//	{
//		// Prevent the non-client area from being painted
//		return 0;
//	}
//	case WM_NCHITTEST:
//	{
//		// Expand the hit test area for resizing
//		const int borderWidth = 8; // Adjust this value to control the hit test area size
//
//		POINTS mousePos = MAKEPOINTS(lParam);
//		POINT clientMousePos = { mousePos.x, mousePos.y };
//		ScreenToClient(hWnd, &clientMousePos);
//
//		RECT windowRect;
//		GetClientRect(hWnd, &windowRect);
//
//		if (clientMousePos.y >= windowRect.bottom - borderWidth)
//		{
//			if (clientMousePos.x <= borderWidth)
//				return HTBOTTOMLEFT;
//			else if (clientMousePos.x >= windowRect.right - borderWidth)
//				return HTBOTTOMRIGHT;
//			else
//				return HTBOTTOM;
//		}
//		else if (clientMousePos.y <= borderWidth)
//		{
//			if (clientMousePos.x <= borderWidth)
//				return HTTOPLEFT;
//			else if (clientMousePos.x >= windowRect.right - borderWidth)
//				return HTTOPRIGHT;
//			else
//				return HTTOP;
//		}
//		else if (clientMousePos.x <= borderWidth)
//		{
//			return HTLEFT;
//		}
//		else if (clientMousePos.x >= windowRect.right - borderWidth)
//		{
//			return HTRIGHT;
//		}
//
//		break;
//	}
//	case WM_NCACTIVATE:
//	{
//		// Prevent non-client area from being redrawn during window activation
//		return TRUE;
//	}
//	}
//
//	return CallWindowProc(original_proc, hWnd, uMsg, wParam, lParam);
//}

void disableTitlebar(GLFWwindow* window)
{
	HWND hWnd = glfwGetWin32Window(window);

	//LONG_PTR lStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	//lStyle |= WS_THICKFRAME;
	//lStyle &= ~WS_CAPTION;
	//lStyle &= ~WS_SYSMENU;
	//lStyle &= ~WS_BORDER;
	//lStyle &= ~WS_MINIMIZEBOX;
	//lStyle &= ~WS_MAXIMIZEBOX;

	//lStyle |= WS_CAPTION;
	//lStyle |= WS_SYSMENU;

	//lStyle |= WS_MINIMIZEBOX;
	//lStyle |= WS_MAXIMIZEBOX;

	//lStyle &= ~WS_OVERLAPPED;
	//
	//SetWindowLongPtr(hWnd, GWL_STYLE, lStyle);
	////
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int width = windowRect.right - windowRect.left;
	int height = windowRect.bottom - windowRect.top;
	//
	////LONG_PTR Style = (WS_CAPTION | WS_SYSMENU);
	////LONG a = SetWindowLongA(hWnd, GWL_STYLE, lStyle & ~WS_BORDER);
	//
	////original_proc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	////(WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
	//SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE);

	 // Extend the frame into the client area.
	MARGINS margins;

	margins.cxLeftWidth		= 0;      // 8
	margins.cxRightWidth	= 0;    // 8
	margins.cyBottomHeight	= 0; // 20
	margins.cyTopHeight		= 0;       // 27

	DwmExtendFrameIntoClientArea(hWnd, &margins);

	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_FRAMECHANGED | SWP_NOMOVE);
}

class WindowSystemTest : public TestBench {
public:

	bool run() {

		auto monitors = Monitor::get_all_monitors();
		std::cout << (const char*)monitors[0].get_name().c_str() << std::endl;

		WindowDescription desc;
		desc.w_scale_framebuffer_size = false;
		desc.w_scale_window_size = false;
		auto window = std::make_shared<Window>(desc);
		//window->set_window_decorated(false);
		//window->set_window_resizable(true);

		window->window_request_attention();

		disableTitlebar((GLFWwindow*)window->get_handle());

		//Window window2(glm::vec2(100, 100), "window2");
		//window2 = std::move(*window);
		//window2.context_make_current();

		window->newsletters->on_filepath_drop_events.subscribe([](const std::vector<std::filesystem::path>& paths) {
			for (auto& path : paths)
				std::wcout << path.c_str() << std::endl;
			});
		window->newsletters->on_should_close_events.subscribe([]() {
			exit(0);
			});

		window->newsletters->on_window_refresh_events.subscribe([&]() {
			primitive_renderer::clear();
			window->swap_buffers();
			});

		while (true) {
			window->handle_events(true);
			//window->set_window_position(glm::dvec2(window->get_window_position()) + window->get_cursor_position());
			primitive_renderer::clear();
		}

		return true;
	}
};
