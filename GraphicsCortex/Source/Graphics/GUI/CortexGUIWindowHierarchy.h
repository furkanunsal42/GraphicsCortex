#pragma once
#include "CortexGUIWindow.h"

class GUIWindowHierarchy;

template<typename W>
class GUIWindowHandle {
public:

	W* operator->() const;
	operator guiwindow_t();

private:
	friend GUIWindowHierarchy;

	GUIWindowHandle(guiwindow_t handle);

	guiwindow_t id = invalid_guiwindow;
};

class GUIWindowHierarchy {
public:

	static GUIWindowHierarchy& get();
	void release();

	template<typename W> GUIWindowHandle<W> create_window();
	template<typename W> GUIWindowHandle<W> create_window(guiwindow_t parent_window);
	void release_window(guiwindow_t window);
	bool does_window_exist(guiwindow_t window);
	bool is_root_window(guiwindow_t window);
	void set_parent_window(guiwindow_t window, guiwindow_t new_parent);
	//GUIWindowHandle& get_root_window(guiwindow_t window);
	//GUIWindowHandle& get_parent_window(guiwindow_t window);

	GUIWindow& get_window_data(guiwindow_t window);
	const std::vector<guiwindow_t>& get_window_children(guiwindow_t window);

private:
	GUIWindowHierarchy() = default;

	guiwindow_t next_guiwindow_id = 0;
	guiwindow_t generate_id();

	struct window_info {
		window_info() = default;
		std::shared_ptr<GUIWindow> window = nullptr;
		guiwindow_t id = invalid_guiwindow;
		guiwindow_t parent_window = invalid_guiwindow;
		std::vector<guiwindow_t> children;
	};

	std::unordered_map<guiwindow_t, window_info> windows;
};

#include "CortexGUIWindowHierarchy_Templated.h"
