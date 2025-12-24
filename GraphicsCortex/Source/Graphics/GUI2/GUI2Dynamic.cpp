#include "GUI2Dynamic.h"
#include <queue>

size_t GUI2Dynamic::find_last_of_type(NodeType type)
{
	if (node_stack.size() == 0)
		return Node::invalid_node;

	size_t traversing_node = node_stack.size() - 1;

	while (traversing_node != -1 && get_type(nodes[node_stack[traversing_node]]) != NodeType::Window)
		traversing_node--;

	size_t found_node = traversing_node != -1 ?
						traversing_node : 
						Node::invalid_node;

	return found_node;
}

GUI2Dynamic::NodeType GUI2Dynamic::get_type(const Node& node)
{
	switch (node.desc.index()) {
	case 0:	return NodeType::Window;
	case 1: return NodeType::Box;
	case 2: return NodeType::Grid;
	case 3: return NodeType::Stack;
	}
}

size_t GUI2Dynamic::push_node(size_t parent)
{
	if (nodes.size() <= parent && parent != Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::push_node() is called but given parent_id is not valid" << std::endl;
		ASSERT(false);
	}
	
	Node node;
	node.parent = parent;

	size_t id = nodes.size();
	nodes.push_back(node);

	if (parent == Node::invalid_node) {
		root_nodes.push_back(id);
		return id;
	}

	if (nodes[parent].child == Node::invalid_node) {
		nodes[parent].child = id;
	}
	else {
		size_t sibling			= Node::invalid_node;
		size_t next_sibling		= nodes[parent].child;
		
		while (next_sibling != Node::invalid_node) {
			sibling			= next_sibling;
			next_sibling	= nodes[next_sibling].sibling;
		} 

		nodes[sibling].sibling = id;
	}
	return id;
}

size_t GUI2Dynamic::push_node(size_t parent, WindowDesc desc)
{
	size_t id = push_node(parent);
	nodes[id].desc = desc;
	return id;
}

size_t GUI2Dynamic::push_node(size_t parent, BoxDesc desc)
{
	size_t id = push_node(parent);
	nodes[id].desc = desc;
	return id;
}

size_t GUI2Dynamic::push_node(size_t parent, GridDesc desc)
{
	size_t id = push_node(parent);
	nodes[id].desc = desc;
	return id;
}

size_t GUI2Dynamic::push_node(size_t parent, StackDesc desc)
{
	size_t id = push_node(parent);
	nodes[id].desc = desc;
	return id;
}

glm::vec2& GUI2Dynamic::node_position(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_position() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).position;
	case Box	: return std::get<BoxDesc>(node.desc).position;
	case Grid	: return std::get<GridDesc>(node.desc).position;
	case Stack	: return std::get<StackDesc>(node.desc).position;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_position() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUI2Dynamic::node_size(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).size;
	case Box	: return std::get<BoxDesc>(node.desc).size;
	case Grid	: return std::get<GridDesc>(node.desc).size;
	case Stack	: return std::get<StackDesc>(node.desc).size;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec4& GUI2Dynamic::node_margin(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_margin() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).margin;
	case Box	: return std::get<BoxDesc>(node.desc).margin;
	case Grid	: return std::get<GridDesc>(node.desc).margin;
	case Stack	: return std::get<StackDesc>(node.desc).margin;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_margin() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUI2Dynamic::node_target_size(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_target_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).target_size;
	case Box	: return std::get<BoxDesc>(node.desc).target_size;
	case Grid	: return std::get<GridDesc>(node.desc).target_size;
	case Stack	: return std::get<StackDesc>(node.desc).target_size;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_target_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUI2Dynamic::node_min_size(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_min_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).min_size;
	case Box	: return std::get<BoxDesc>(node.desc).min_size;
	case Grid	: return std::get<GridDesc>(node.desc).min_size;
	case Stack	: return std::get<StackDesc>(node.desc).min_size;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_min_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUI2Dynamic::node_max_size(size_t node_id){
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_max_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).max_size;
	case Grid	: return std::get<GridDesc>(node.desc).max_size;
	case Stack	: return std::get<StackDesc>(node.desc).max_size;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_max_size() but an error is occured" << std::endl;
	ASSERT(false);
}


///////////		WINDOW		////////////

void GUI2Dynamic::window_begin(const std::string& idstr){
	if (node_stack.size() != 0) {
		std::cout << "[GUI Error] GUI2Dynamic::window_begin() is called but windows must be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}
	
	WindowDesc desc;
	desc.idstr = idstr;
	size_t id = push_node(Node::invalid_node, desc);
	
	node_stack.push_back(id);
	last_window = id;
	
	resolved_properties[idstr];
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::window_prop(){
	
	if (last_window == Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::window_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<WindowDesc>(nodes[last_window].desc);
}

void GUI2Dynamic::window_end(){

	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::window_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) != NodeType::Window) {
		std::cout << "[GUI Error] GUI2Dynamic::window_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	node_stack.pop_back();
	last_window = find_last_of_type(NodeType::Window);
}

///////////		GRID		////////////

void GUI2Dynamic::grid_begin(const std::string& idstr){
	grid_begin();
	std::get<GridDesc>(nodes[last_grid].desc).idstr = idstr;
	resolved_properties[idstr];
}

void GUI2Dynamic::grid_begin(){
	
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_begin() is called but grids cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Window && nodes[node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span  = glm::ivec2(0, 0);
	if (get_type(nodes[node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span  = desc.current_grid_span;
	}

	GridDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(node_stack.back(), desc);

	node_stack.push_back(id);
	last_grid = id;

}

GUI2Dynamic::GridDesc& GUI2Dynamic::grid_prop(){
	if (last_grid == Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_prop() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<GridDesc>(nodes[last_grid].desc);
}

void GUI2Dynamic::grid_add_column(float width){
	grid_prop().columns.push_back(width);
}

void GUI2Dynamic::grid_add_row(float height){
	grid_prop().rows.push_back(height);
}

void GUI2Dynamic::grid_region(glm::ivec2 grid_index, glm::ivec2 grid_span){
	
	if (last_grid == Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_region() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (glm::any(glm::lessThan(grid_index, glm::ivec2(0))) || glm::any(glm::lessThanEqual(grid_span, glm::ivec2(0)))) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_region() is called but values for index and span are invalid" << std::endl;
		ASSERT(false);
	}

	GUI2Dynamic::GridDesc& desc = grid_prop();

	if (glm::any(glm::greaterThan(grid_index + grid_span, glm::ivec2(desc.columns.size(), desc.rows.size())))) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_region() is called but values the given region exceeds the layout" << std::endl;
		ASSERT(false);
	}

	desc.current_grid_index = grid_index;
	desc.current_grid_span	= grid_span;
}

void GUI2Dynamic::grid_end(){
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) != NodeType::Grid) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	auto& desc = grid_prop();

	if (desc.columns.size() == 0 || desc.rows.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called without specifying at least one column and row" << std::endl;
		ASSERT(false);
	}

	desc.current_grid_index	= glm::ivec2(0, 0);
	desc.current_grid_span	= glm::ivec2(0, 0);

	node_stack.pop_back();
	last_grid = find_last_of_type(NodeType::Grid);
}

///////////		STACK		////////////

void GUI2Dynamic::stack_begin(const std::string& idstr){
	stack_begin();
	std::get<StackDesc>(nodes[last_stack].desc).idstr = idstr;
	resolved_properties[idstr];
}

void GUI2Dynamic::stack_begin(){
	
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_begin() is called but stacks cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Window && nodes[node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span  = glm::ivec2(0, 0);
	if (get_type(nodes[node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span  = desc.current_grid_span;
	}

	StackDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(node_stack.back(), desc);

	node_stack.push_back(id);
	last_stack = id;

}

GUI2Dynamic::StackDesc& GUI2Dynamic::stack_prop(){
	if (last_stack == Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_prop() is called but there are no stacks in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<StackDesc>(nodes[last_stack].desc);
}

void GUI2Dynamic::stack_end(){
	
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_end() is called but there are no stacks in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) != NodeType::Stack) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	node_stack.pop_back();
	last_stack = find_last_of_type(NodeType::Stack);

}

///////////		BOX		////////////


void GUI2Dynamic::box_begin(const std::string& idstr){
	box_begin();
	std::get<BoxDesc>(nodes[last_box].desc).idstr = idstr;
	resolved_properties[idstr];
}

void GUI2Dynamic::box_begin(){

	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::box_begin() is called but boxs cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Window && nodes[node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::box_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUI2Dynamic::box_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span = glm::ivec2(0, 0);
	if (get_type(nodes[node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span = desc.current_grid_span;
	}

	BoxDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(node_stack.back(), desc);

	node_stack.push_back(id);
	last_box = id;

}

GUI2Dynamic::BoxDesc& GUI2Dynamic::box_prop(){
	
	if (last_box == Node::invalid_node) {
		std::cout << "[GUI Error] GUI2Dynamic::box_prop() is called but there are no boxes in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<BoxDesc>(nodes[last_box].desc);

}

void GUI2Dynamic::box_end(){

	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::box_end() is called but there are no boxes in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) != NodeType::Box) {
		std::cout << "[GUI Error] GUI2Dynamic::box_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	node_stack.pop_back();
	last_box = find_last_of_type(NodeType::Box);

}

///////////		TRAVERSAL		////////////

//																	  level	   self
void GUI2Dynamic::_traverse_nodes(size_t root_node, std::function<void(int32_t, size_t)> lambda) {

	if (root_node >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::_traverse_nodes() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	struct node_info {
		int32_t level;
		size_t node;
	};
	std::vector<node_info> stack;

	stack.push_back({0, root_node});

	while (stack.size() != 0) {

		node_info info	= stack.back();
		stack.pop_back();

		Node& node = nodes[info.node];

		std::vector<size_t> children;
		size_t iterating_child = node.child;
		while (iterating_child != Node::invalid_node) {

			children.push_back(iterating_child);
			iterating_child = nodes[iterating_child].sibling;

		}

		for (size_t i = children.size() - 1; i != -1; i--)
			stack.push_back({ info.level+1, children[i] });

		lambda(info.level, info.node);

	}

}

void GUI2Dynamic::traverse_nodes_down(size_t root_node, std::function<void(int32_t, size_t)> lambda) {
	
	if (root_node >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::traverse_nodes_down() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	size_t	node_index = root_node;
	int32_t level = 0;

	do {
		
		Node& node = nodes[node_index];
		
		lambda(level, node_index);
		
		if (node.child != Node::invalid_node)
			level++;
		else if (node.sibling == Node::invalid_node)	
			level--;

		node_index++;

	} while (level != 0 && node_index < nodes.size());
}

void GUI2Dynamic::traverse_nodes_up(size_t root_node, std::function<void(int32_t, size_t)> lambda) {

	if (root_node >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::traverse_nodes_up() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	size_t root_node_index = -1;
	for (int32_t i = 0; i < root_nodes.size(); i++) {
		if (root_nodes[i] == root_node)
			root_node_index = i;
	}

	if (root_node_index == -1) {
		std::cout << "[GUI Error] GUI2Dynamic::traverse_nodes_up() is called but given root_node is not an actual root node" << std::endl;
		ASSERT(false);
	}

	size_t end_node = root_node_index != root_nodes.size() - 1 ? root_nodes[root_node_index + 1] : nodes.size();

	size_t	node_index = end_node - 1;
	int32_t level = 0;

	do {

		Node& node = nodes[node_index];

		if (node.child != Node::invalid_node)
			level--;
		else if (node.sibling == Node::invalid_node)
			level++;

		lambda(level, node_index);
		
		node_index--;

	} while (node_index >= root_node && node_index != -1);
}

void GUI2Dynamic::traverse_nodes_children(size_t parent_node, std::function<void(size_t)> lambda) {

	if (parent_node >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::traverse_nodes_children() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	size_t child = nodes[parent_node].child;

	while (child != Node::invalid_node) {
		lambda(child);
		child = nodes[child].sibling;
	}
}

void GUI2Dynamic::print_nodes() {
	for (size_t i = 0; i < nodes.size(); i++) {
		
		Node& node = nodes[i];

		NodeType type		= get_type(node);
		std::string name	= 
			type == NodeType::Window	? "Window"	:
			type == NodeType::Box		? "Box"		:
			type == NodeType::Grid		? "Grid"	:
			type == NodeType::Stack		? "Stack"	: "Error";

		std::cout << i << "\t\t" << name << "\tchild: " << node.child << " sib:" << node.sibling << " parent:" << node.parent << std::endl;
	}
		
}

void GUI2Dynamic::print_layout()
{
	if (root_nodes.size() == 0)
		return;

	traverse_nodes_up(root_nodes[0], [&](int32_t level, size_t node) {

		NodeType type = get_type(nodes[node]);
		std::string name =
			type == NodeType::Window	? "Window" :
			type == NodeType::Box		? "Box" :
			type == NodeType::Grid		? "Grid" :
			type == NodeType::Stack		? "Stack" : "Error";

		for (int32_t i = 0; i < level; i++)
			std::cout << " | ";
		std::cout << name;
		std::cout << std::endl;

		});
}


///////////		RESOLVE		////////////

void GUI2Dynamic::resolve() {
	
	traverse_nodes_up(root_nodes[0], [&](int32_t level, size_t node_id) {

		Node& node = nodes[node_id];
		NodeType node_type = get_type(node);

		switch (node_type) {
		case Window	: break; 
		case Box	: 
		{
			auto& desc = std::get<BoxDesc>(node.desc);
			
			desc.size = desc.target_size;
			if (desc.target_size.x == fit)  desc.size.x = 0;
			if (desc.target_size.y == fit)	desc.size.y = 0;
			
			break;
		}
		case Grid	: 
		{

			break;
		}
		case Stack	: 
		{

			auto& desc = std::get<StackDesc>(node.desc);

			desc.size = desc.target_size;

			if (node.child == Node::invalid_node) {
				if (desc.target_size.x == fit)  desc.size.x = 0;
				if (desc.target_size.y == fit)	desc.size.y = 0;
			}
			else {
				
				glm::vec2 min_size = glm::vec2(0);
				int32_t children_count = 0;
				traverse_nodes_children(node_id, [&](size_t child) {

					children_count++;
					glm::vec2 child_size = node_size(child);
					if (desc.is_vertical) {
						min_size.x =  glm::max(min_size.x, child_size.x);
						min_size.y += child_size.y;
					}
					else {
						min_size.x += child_size.x;
						min_size.y = glm::max(min_size.y, child_size.y);
					}

					});

				if (desc.is_vertical)	min_size.y += desc.spacing * glm::max((children_count - 1), 0);
				else					min_size.x += desc.spacing * glm::max((children_count - 1), 0);

				if (desc.target_size.x == fit)  desc.size.x = min_size.x;
				if (desc.target_size.y == fit)	desc.size.y = min_size.y;
			}
			
			break;
		}
		}

		});

}