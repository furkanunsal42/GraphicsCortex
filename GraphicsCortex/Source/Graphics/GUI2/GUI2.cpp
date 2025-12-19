#include "GUI2.h"

GUI2::WindowResult GUI2::window(const std::string& idstr, WindowDesc& desc)
{
	size_t id = nodes.size();

	Node node;
	node.type = Node::Window;

	nodes.push_back(node);
	window_stack.push_back(id);

	windows_desc[idstr] = desc;

	return WindowResult();
}

void GUI2::window_end()
{
	window_stack.pop_back();
}

GUI2::Layout GUI2::generate_layout(IOState& io_state)
{


	return Layout();
}
