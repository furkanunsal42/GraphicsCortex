#include "GUI2Dynamic.h"
#include <queue>
#include "GLMCout.h"

size_t GUI2Dynamic::find_last_of_type(NodeType type)
{
	if (node_stack.size() == 0)
		return Node::invalid_node;

	size_t traversing_node = node_stack.size() - 1;

	while (traversing_node != -1 && get_type(nodes[node_stack[traversing_node]]) != type)
		traversing_node--;

	size_t found_node = traversing_node != -1 ?
						traversing_node : 
						Node::invalid_node;

	return node_stack[traversing_node];
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

glm::vec4& GUI2Dynamic::node_padding(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_padding() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).padding;
	//case Box	: return std::get<BoxDesc>(node.desc).padding;
	case Grid	: return std::get<GridDesc>(node.desc).padding;
	case Stack	: return std::get<StackDesc>(node.desc).padding;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_padding() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec4 GUI2Dynamic::get_node_padding_non_ref(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::get_node_padding_non_ref() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	return get_type(nodes[node_id]) == NodeType::Box ? glm::vec4(0) : node_padding(node_id);
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

glm::ivec2& GUI2Dynamic::node_grid_index(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_grid_index() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).grid_slot;
	case Grid	: return std::get<GridDesc>(node.desc).grid_slot;
	case Stack	: return std::get<StackDesc>(node.desc).grid_slot;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_grid_index() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::ivec2& GUI2Dynamic::node_grid_span(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_grid_span() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).grid_span;
	case Grid	: return std::get<GridDesc>(node.desc).grid_span;
	case Stack	: return std::get<StackDesc>(node.desc).grid_span;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_grid_span() but an error is occured" << std::endl;
	ASSERT(false);
}

size_t GUI2Dynamic::get_node_id(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::get_node_id() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).id;
	case Box	: return std::get<BoxDesc>(node.desc).id;
	case Grid	: return std::get<GridDesc>(node.desc).id;
	case Stack	: return std::get<StackDesc>(node.desc).id;
	}

	std::cout << "[GUI Error] GUI2Dynamic::get_node_id() but an error is occured" << std::endl;
	ASSERT(false);
}

GUI2::MouseEvent& GUI2Dynamic::node_mouse_event(size_t node_id)
{
	if (node_id >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::node_mouse_event() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).mouse_event;
	case Box	: return std::get<BoxDesc>(node.desc).mouse_event;
	case Grid	: return std::get<GridDesc>(node.desc).mouse_event;
	case Stack	: return std::get<StackDesc>(node.desc).mouse_event;
	}

	std::cout << "[GUI Error] GUI2Dynamic::node_mouse_event() but an error is occured" << std::endl;
	ASSERT(false);

	GUI2::MouseEvent null_event;
	return null_event;
}


///////////		WINDOW		////////////

void GUI2Dynamic::new_frame(GUI2& gui)
{
	nodes.clear();
	root_nodes.clear();
	node_stack.clear();

	last_window = Node::invalid_node;
	last_box	= Node::invalid_node;
	last_grid	= Node::invalid_node;
	last_stack	= Node::invalid_node;

	io_state = gui.get_io_state();

	for (auto iterator = resolved_properties.begin(); iterator != resolved_properties.end();) {
		auto& [key, value] = *iterator;
		
		if (value.node_id == Node::invalid_node) {
			iterator = resolved_properties.erase(iterator);
		}
		else {
			value.node_id = invalid_id;
			iterator++;
		}
	}
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::window_begin(size_t& id){

	if (id == invalid_id)
		id = generate_id();

	if (node_stack.size() != 0) {
		std::cout << "[GUI Error] GUI2Dynamic::window_begin() is called but windows must be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}
	
	WindowDesc desc;
	desc.id = id;
	size_t node_id = push_node(Node::invalid_node, desc);
	
	node_stack.push_back(node_id);
	last_window = node_id;
	
	resolved_properties[id].node_id = last_window;

	return window_prop();
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

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

	if (get_type(nodes[node_stack.back()]) != NodeType::Window) {
		std::cout << "[GUI Error] GUI2Dynamic::window_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	node_stack.pop_back();
	last_window = find_last_of_type(NodeType::Window);

	resolve(root_nodes.back());
}

///////////		GRID		////////////

GUI2Dynamic::GridDesc& GUI2Dynamic::grid_begin(size_t& id){
	
	if (id == invalid_id)
		id = generate_id();

	auto& desc = grid_begin();
	std::get<GridDesc>(nodes[last_grid].desc).id = id;
	
	resolved_properties[id].node_id = last_grid;
	
	return desc;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::grid_begin(){
	
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_begin() is called but grids cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

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

	return grid_prop();
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

	//if (glm::any(glm::greaterThan(grid_index + grid_span, glm::ivec2(desc.columns.size(), desc.rows.size())))) {
	//	std::cout << "[GUI Error] GUI2Dynamic::grid_region() is called but values the given region exceeds the layout" << std::endl;
	//	ASSERT(false);
	//}

	desc.current_grid_index = grid_index;
	desc.current_grid_span	= grid_span;
}

void GUI2Dynamic::grid_end(){
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

	if (get_type(nodes[node_stack.back()]) != NodeType::Grid) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	auto& desc = grid_prop();

	if (desc.columns.size() == 0 || desc.rows.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called without specifying at least one column and row" << std::endl;
		ASSERT(false);
	}

	//for (float column : desc.columns) {
	//	if (column == fit)
	//	{
	//		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called but (fit) is not a valid value for a column" << std::endl;
	//		ASSERT(false);
	//	}
	//}
	//
	//for (float row : desc.rows) {
	//	if (row == fit)
	//	{
	//		std::cout << "[GUI Error] GUI2Dynamic::grid_end() is called but (fit) is not a valid value for a row" << std::endl;
	//		ASSERT(false);
	//	}
	//}

	desc.current_grid_index	= glm::ivec2(0, 0);
	desc.current_grid_span	= glm::ivec2(0, 0);

	node_stack.pop_back();
	last_grid = find_last_of_type(NodeType::Grid);
}

///////////		STACK		////////////

GUI2Dynamic::StackDesc& GUI2Dynamic::stack_begin(size_t& id){
	
	if (id == invalid_id)
		id = generate_id();

	auto& desc = stack_begin();
	std::get<StackDesc>(nodes[last_stack].desc).id = id;
	
	resolved_properties[id].node_id = last_stack;

	return desc;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::stack_begin(){
	
	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_begin() is called but stacks cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

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

	return stack_prop();
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

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

	if (get_type(nodes[node_stack.back()]) != NodeType::Stack) {
		std::cout << "[GUI Error] GUI2Dynamic::stack_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	node_stack.pop_back();
	last_stack = find_last_of_type(NodeType::Stack);

}

///////////		BOX		////////////


GUI2Dynamic::BoxDesc& GUI2Dynamic::box_begin(size_t& id){
	if (id == invalid_id)
		id = generate_id();

	auto& desc = box_begin();
	
	std::get<BoxDesc>(nodes[last_box].desc).id = id;
	resolved_properties[id].node_id = last_box;
	
	return desc;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::box_begin(){

	if (node_stack.size() == 0) {
		std::cout << "[GUI Error] GUI2Dynamic::box_begin() is called but boxs cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(nodes[node_stack.back()]) == Box)
		box_end();

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

	return box_prop();
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
	
	size_t next_root_node = root_node;
	for (size_t root : root_nodes) {
		if (root > next_root_node) {
			next_root_node = root;
			break;
		}
	}

	if (next_root_node == root_node)
		next_root_node = nodes.size();

	do {
		
		Node& node = nodes[node_index];
		
		lambda(level, node_index);
		
		if (node.child != Node::invalid_node)
			level++;
		else if (node.sibling == Node::invalid_node)	
			level--;

		node_index++;

	} while (level != 0 && node_index < next_root_node);
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

	traverse_nodes_down(root_nodes[0], [&](int32_t level, size_t node) {

		NodeType type = get_type(nodes[node]);
		std::string name =
			type == NodeType::Window	? "Window" :
			type == NodeType::Box		? "Box" :
			type == NodeType::Grid		? "Grid" :
			type == NodeType::Stack		? "Stack" : "Error";

		for (int32_t i = 0; i < level; i++)
			std::cout << " | ";
		std::cout << name;
		std::cout << " size: " << node_size(node);
		std::cout << " position: " << node_position(node);
		std::cout << " target_size: " << node_target_size(node);
		if (type != NodeType::Window)	std::cout << " margin: " << node_margin(node);
		if (type != NodeType::Box)		std::cout << " padding: " << node_padding(node);
		std::cout << std::endl;

		});
}

///////////		RESOLVE		////////////

void GUI2Dynamic::resolve(size_t root_node, bool verbose)
{

	if (verbose) print_layout();
	
	resolve_phase0_fit(root_node);

	if (verbose) std::cout << std::endl;
	if (verbose) print_layout();

	resolve_phase1_avail_and_position(root_node);
	
	if (verbose) std::cout << std::endl;
	if (verbose) print_layout();

	resolve_phase2_mouse_event(root_node);
}

void GUI2Dynamic::resolve() {
	
	for (size_t root_node : root_nodes)
		resolve(root_node);
}

GUI2Dynamic::ResolvedProperties GUI2Dynamic::get_resolved_properties(size_t id)
{
	if (resolved_properties.find(id) == resolved_properties.end())
		return ResolvedProperties();

	return resolved_properties.at(id);
}

GUI2Dynamic::ResolvedProperties GUI2Dynamic::get_resolved_properties(const std::string& idstr)
{
	if (idstr_to_id.find(idstr) == idstr_to_id.end())
		return ResolvedProperties();

	size_t id = idstr_to_id.at(idstr);
	return get_resolved_properties(id);
}

GUI2::IOState& GUI2Dynamic::get_io_state() {
	return io_state;
}

int32_t GUI2Dynamic::get_levels_under_cursor()
{
	return levels_under_cursor;
}

size_t GUI2Dynamic::generate_id()
{
	size_t id = next_id_to_generate;
	next_id_to_generate++;
	return id;
}

void GUI2Dynamic::resolve_phase0_fit(size_t root_node){
	
	traverse_nodes_up(root_node, [&](int32_t level, size_t node_id) {

		Node& node = nodes[node_id];
		NodeType node_type = get_type(node);

		switch (node_type) {
		case Window:
		{
			auto& desc = std::get<WindowDesc>(node.desc);

			desc.size = desc.target_size;

			if (node.child == Node::invalid_node)
			{
				desc.size = glm::ivec2(0);
				break;
			}

			glm::vec2 child_size	= node_size(node.child);
			glm::vec2 child_margin	= non_avail(node_margin(node.child));
			
			if (desc.target_size.x == fit)  desc.size.x = child_size.x + desc.padding.x + desc.padding.z + child_margin.x;
			if (desc.target_size.y == fit)	desc.size.y = child_size.y + desc.padding.y + desc.padding.w + child_margin.y;

			break;
		}
		case Box:
		{
			auto& desc = std::get<BoxDesc>(node.desc);

			desc.size = desc.target_size;
			if (desc.target_size.x == fit)  desc.size.x = 0;
			if (desc.target_size.y == fit)	desc.size.y = 0;

			break;
		}
		case Grid:
		{
			auto& desc = std::get<GridDesc>(node.desc);

			desc.size = desc.target_size;

			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				if (desc.columns[column_id] == fit) {
					float max_value = 0;
					traverse_nodes_children(node_id, [&](size_t child) {
						if (node_grid_index(child).x == column_id) {
							if (node_grid_span(child).x != 1) {
								std::cout << "[GUI Error] grid slot cannot be \"fit\" while containing multi-slot children" << std::endl;
								ASSERT(false);
							}
							max_value = std::max(max_value, non_avail(node_size(child).x));
						}
						});
					desc.columns[column_id] = max_value;
				}
			}
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {
				if (desc.rows[row_id] == fit) {
					float max_value = 0;
					traverse_nodes_children(node_id, [&](size_t child) {
						if (node_grid_index(child).y == row_id) {
							if (node_grid_span(child).y != 1) {
								std::cout << "[GUI Error] grid slot cannot be \"fit\" while containing multi-slot children" << std::endl;
								ASSERT(false);
							}
							max_value = std::max(max_value, non_avail(node_size(child).y));
						}
						});
					desc.rows[row_id] = max_value;
				}
			}

			if (glm::any(glm::equal(desc.target_size, glm::vec2(fit)))) {

				glm::vec2 min_size = glm::vec2(0);

				for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++)
					if (desc.columns[column_id] > 0) min_size.x += desc.columns[column_id];

				for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++)
					if (desc.rows[row_id] > 0) min_size.y += desc.rows[row_id];

				if (desc.target_size.x == fit)  desc.size.x = min_size.x + desc.padding.x + desc.padding.z;
				if (desc.target_size.y == fit)	desc.size.y = min_size.y + desc.padding.y + desc.padding.w;
			}

			break;
		}
		case Stack:
		{
			auto& desc = std::get<StackDesc>(node.desc);

			desc.size = desc.target_size;

			if (node.child == Node::invalid_node) {
				if (desc.target_size.x == fit)  desc.size.x = 0;
				if (desc.target_size.y == fit)	desc.size.y = 0;
			}
			else if (glm::any(glm::equal(desc.target_size, glm::vec2(fit)))) {

				glm::vec2 min_size = glm::vec2(0);
				int32_t children_count = 0;
				traverse_nodes_children(node_id, [&](size_t child) {

					children_count++;
					glm::vec2 child_size = non_avail(node_size(child)) + non_avail(node_margin(child));

					if (desc.is_vertical) {
						min_size.x = glm::max(min_size.x, child_size.x);
						min_size.y += child_size.y;
					}
					else {
						min_size.x += child_size.x;
						min_size.y = glm::max(min_size.y, child_size.y);
					}

					});

				if (desc.is_vertical)	min_size.y += desc.spacing * glm::max((children_count - 1), 0);
				else					min_size.x += desc.spacing * glm::max((children_count - 1), 0);

				if (desc.target_size.x == fit)  desc.size.x = min_size.x + desc.padding.x + desc.padding.z;
				if (desc.target_size.y == fit)	desc.size.y = min_size.y + desc.padding.y + desc.padding.w;
			}

			break;
		}
		}
		});
}

void GUI2Dynamic::resolve_phase1_avail_and_position(size_t root_node)
{
	traverse_nodes_down(root_node, [&](int32_t level, size_t node_id) {

		Node& node = nodes[node_id];
		NodeType node_type = get_type(node);

		switch (node_type) {
		case Window:
		{
			auto& desc = std::get<WindowDesc>(node.desc);
			
			// handle maximize
			if (is_avail(desc.target_size.x)) desc.size.x = 1024;
			if (is_avail(desc.target_size.x)) desc.size.y = 1024;
			
			if (node.child != Node::invalid_node) {
				if (is_avail(node_target_size(node.child).x)) node_size(node.child).x = desc.size.x;
				if (is_avail(node_target_size(node.child).x)) node_size(node.child).y = desc.size.y;

				node_position(node.child) = desc.padding;
			}

			break;
		}
		case Box:
		{
			break;
		}
		case Grid:
		{
			if (node.child == Node::invalid_node)
				return;

			auto& desc = std::get<GridDesc>(node.desc);
			
			glm::vec2 cells_min_size		= glm::vec2(0);
			glm::ivec2 cells_avails_total	= glm::ivec2(0);

			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				cells_avails_total.x	+= avail_ratio(desc.columns[column_id]);
				cells_min_size.x		+= non_avail(desc.columns[column_id]);
			}
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {
				cells_avails_total.y	+= avail_ratio(desc.rows[row_id]);
				cells_min_size.y		+= non_avail(desc.rows[row_id]);
			}

			glm::vec2 self_size = desc.size - glm::vec2(desc.padding.x + desc.padding.z, desc.padding.y + desc.padding.w);
			glm::vec2 size_per_avail	= compute_size_per_avail(self_size - cells_min_size, cells_avails_total);

			float current_offset = 0;
			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				current_offset			+= compute_physical_size(desc.columns[column_id], size_per_avail.x);
				desc.columns[column_id] = current_offset;
			}

			current_offset = 0;
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {
				current_offset		+= compute_physical_size(desc.rows[row_id], size_per_avail.y);
				desc.rows[row_id]	= current_offset;
			}

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2 child_target_size = node_target_size(child_id);
				glm::vec4& child_margin_ref	= node_margin(child_id);
				
				glm::ivec2 child_total_avail	= avail_ratio(child_margin_ref) + avail_ratio(child_target_size);
				glm::vec2 child_non_avail_size	= non_avail(child_margin_ref) + non_avail(child_target_size);

				//if (glm::all(glm::equal(child_total_avail, glm::ivec2(0))))
				//	return;

				glm::ivec2 child_index	= node_grid_index(child_id);
				glm::ivec2 child_span	= node_grid_span(child_id);

				glm::vec2 cell_size = glm::vec2(
					desc.columns[child_index.x + child_span.x - 1]	- (child_index.x > 0 ? desc.columns[child_index.x - 1] : 0),
					desc.rows   [child_index.y + child_span.y - 1]	- (child_index.y > 0 ? desc.rows   [child_index.y - 1] : 0)
					);

				glm::vec2 child_size_per_avail = compute_size_per_avail(cell_size - child_non_avail_size, child_total_avail);
				
				glm::vec2& child_size_ref = node_size(child_id);

				if (is_avail(child_target_size.x)) child_size_ref.x  = avail_ratio(child_target_size.x) * child_size_per_avail.x;
				if (is_avail(child_target_size.y)) child_size_ref.y  = avail_ratio(child_target_size.y) * child_size_per_avail.y;

				glm::vec2 cell_position =
					glm::vec2(
						child_index.x > 0 ? desc.columns[child_index.x - 1] : 0,
						child_index.y > 0 ? desc.rows[child_index.y - 1]	: 0);

				node_position(child_id) =
					desc.position +
					cell_position + 
					glm::vec2(desc.padding.x, desc.padding.y) +
					glm::vec2(
						compute_physical_size(child_margin_ref.x, child_size_per_avail.x),
						compute_physical_size(child_margin_ref.y, child_size_per_avail.y)
					);

				});

			break;
		}
		case Stack:
		{
			if (node.child == Node::invalid_node)
				return;

			auto& desc = std::get<StackDesc>(node.desc);

			glm::vec2	children_min_size		= glm::vec2(0);
			int32_t		children_total_avails	= 0;
			int32_t		children_count			= 0;

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2 child_target_size = node_target_size(child_id);
				glm::vec4 child_margin		= node_margin(child_id);

				children_min_size		+= non_avail(child_target_size) + non_avail(child_margin);
				
				children_total_avails +=
					desc.is_vertical ?
					avail_ratio(child_target_size.y) + avail_ratio(child_margin).y :
					avail_ratio(child_target_size.x) + avail_ratio(child_margin).x;

				children_count++;
				});

			children_min_size += desc.is_vertical ? glm::vec2(0, children_count * desc.spacing) : glm::vec2(children_count * desc.spacing, 0);

			glm::vec2 self_size = desc.size - glm::vec2(desc.padding.x + desc.padding.z, desc.padding.y + desc.padding.w);
			
			int32_t size_per_avail = 
				desc.is_vertical ?
				compute_size_per_avail(self_size.y - children_min_size.y, children_total_avails) :
				compute_size_per_avail(self_size.x - children_min_size.x, children_total_avails);

			float current_position = 0;

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2 child_target_size		= node_target_size(child_id);

				glm::vec2& child_size_ref		= node_size(child_id);
				glm::vec2& child_position_ref	= node_position(child_id);
				
				glm::vec4& child_margin_ref		= node_margin(child_id);

				int32_t child_avail_off_side	= 
					desc.is_vertical ?
					avail_ratio(child_margin_ref.x ) +
					avail_ratio(child_margin_ref.z ) +
					avail_ratio(child_target_size.x)
					:
					avail_ratio(child_margin_ref.y ) +
					avail_ratio(child_margin_ref.w ) +
					avail_ratio(child_target_size.y);

				float child_physical_off_side	= 
					desc.is_vertical ?
					non_avail(child_margin_ref.x ) +
					non_avail(child_margin_ref.z ) +
					non_avail(child_target_size.x)
					:
					non_avail(child_margin_ref.y ) +
					non_avail(child_margin_ref.w ) +
					non_avail(child_target_size.y);

				float child_avail_per_size_off_side 
					= compute_size_per_avail(
						(desc.is_vertical ? self_size.x : self_size.y) - child_physical_off_side,
						child_avail_off_side
					);

				glm::vec2 effective_margin = glm::vec2(0);

				if (desc.is_vertical) {
					child_size_ref.x = compute_physical_size(child_size_ref.x, child_avail_per_size_off_side);
					child_size_ref.y = compute_physical_size(child_size_ref.y, size_per_avail);
					
					effective_margin.x = compute_physical_size(child_margin_ref.x, child_avail_per_size_off_side);
					effective_margin.y = compute_physical_size(child_margin_ref.y, size_per_avail);

				}
				else {
					child_size_ref.x = compute_physical_size(child_size_ref.x, size_per_avail);
					child_size_ref.y = compute_physical_size(child_size_ref.y, child_avail_per_size_off_side);

					effective_margin.x = compute_physical_size(child_margin_ref.x, size_per_avail);
					effective_margin.y = compute_physical_size(child_margin_ref.y, child_avail_per_size_off_side);
				}


				child_position_ref = 
					desc.position + 
					glm::vec2(desc.padding.x, desc.padding.y) + 
					effective_margin +
					(desc.is_vertical ? 
						glm::vec2(0, current_position) : 
						glm::vec2(current_position, 0)
						);

				current_position += desc.is_vertical ? child_size_ref.y : child_size_ref.x;
				current_position += desc.spacing;
				
				});

			break;
		}
		}

		});
}

void GUI2Dynamic::resolve_phase2_mouse_event(size_t root_node)
{
	if (root_node >= nodes.size()) {
		std::cout << "[GUI Error] GUI2Dynamic::resolve_phase2_mouse_event() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	if (resolved_properties.size() == 0)
		return;

	for (auto& entry : resolved_properties) {
		auto& [resolved_id, resolved_property] = entry;
		
		resolved_property.event		= GUI2::None;
	}

	glm::vec2&			mouse_position_ref = io_state.mouse_position;
	GUI2::MouseEvent	mouse_event_ref		= io_state.mouse_state;
	glm::vec2			root_position		= node_position(root_node);

	struct stack_info {
		size_t widget = 0;
		int32_t level = 0;
	};

	std::vector<stack_info> stack;
	stack.push_back({ .widget=root_node, .level=0 });

	levels_under_cursor = 0;

	while (stack.size() != 0) {

		auto [node_id, node_level] = stack.back();
		stack.pop_back();

		GUI2::MouseEvent& event_ref = node_mouse_event(node_id);

		glm::vec2 position  = node_id != root_node ? root_position + node_position(node_id) : root_position;
		glm::vec2 size		= node_size(node_id);

		AABB2 node_area		= AABB2(position, position + size);

		if (!node_area.does_contain(mouse_position_ref)) {
			event_ref = GUI2::MouseEvent::None;
			continue;
		}

		event_ref = io_state.mouse_state;
		size_t resolved_id = get_node_id(node_id);

		if (resolved_properties.find(resolved_id) != resolved_properties.end()) {
			resolved_properties[resolved_id].event = event_ref;
			resolved_properties[resolved_id].level = node_level;
			levels_under_cursor = std::max(levels_under_cursor, node_level + 1);
		}

		traverse_nodes_children(node_id, [&](size_t child) {
			stack.push_back({ child, node_level + 1 });
			});
	}

	for (auto& entry : resolved_properties) {
		auto& [resolved_id, resolved_property] = entry;
		size_t node_id = resolved_property.node_id;

		resolved_property.position	= node_position(node_id);
		resolved_property.size		= node_size(node_id);

	}
}

void GUI2Dynamic::publish(GUI2& gui)
{
	if (nodes.size() == 0) {
		return;
	}

	if (node_stack.size() != 0) {
		std::cout << "GUI2Dynamic::publish() is called but hierarchy isn't completed yet" << std::endl;
		ASSERT(false);
	}
	
	for (size_t root_node : root_nodes) {

		if (get_type(nodes[root_node]) != NodeType::Window) {
			std::cout << "GUI2Dynamic::publish() is called with invalid hierarchy" << std::endl;
			ASSERT(false);
		}

		{
			auto& desc = std::get<WindowDesc>(nodes[root_node].desc);

			gui.window_begin(gui_idstr_prefix + "window" + std::to_string(desc.id),
				desc.size,
				desc.position
			);

			gui.window_prop().position		= desc.position;
			gui.window_prop().size			= desc.size;
			gui.window_prop().is_resizable	= desc.is_resizable;
			gui.window_prop().is_decorated	= desc.is_decorated;

			gui.window_prop().color			= desc.color;
		}

		traverse_nodes_down(root_node, [&](int32_t level, size_t child) {

			switch (get_type(nodes[child])) {
			case Box :
			{
				auto& desc = std::get<BoxDesc>(nodes[child].desc);
				gui.box_begin(desc.position, desc.size);
				gui.box_prop().color				= desc.color;
				gui.box_prop().border_thickness		= desc.border_thickness;
				gui.box_prop().border_rounding		= desc.border_rounding;
				gui.box_prop().border_color0		= desc.border_color0;
				gui.box_prop().border_color1		= desc.border_color1;
				gui.box_prop().border_color2		= desc.border_color2;
				gui.box_prop().border_color3		= desc.border_color3;
				gui.box_prop().shadow_thickness		= desc.shadow_thickness;
				gui.box_prop().shadow_color			= desc.shadow_color;
				gui.box_prop().uv00					= desc.uv00;
				gui.box_prop().uv11					= desc.uv11;
				gui.box_prop().texture_handle		= desc.texture_handle;
				gui.box_prop().z					= (float)child;
				gui.box_end();


				break;
			}
			case Window:
			case Grid:
			case Stack:
				break;
			}
			});

		gui.window_end();
	}
}

bool GUI2Dynamic::is_avail(float value) {
	return avail_ratio(value) != 0;
}

bool GUI2Dynamic::is_any_avail(glm::vec2 value)
{
	return is_avail(value.x) || is_avail(value.y);
}

bool GUI2Dynamic::is_any_avail(glm::vec4 value)
{
	return is_avail(value.x) || is_avail(value.y) || is_avail(value.z) || is_avail(value.w);
}

int32_t GUI2Dynamic::avail_ratio(float value) {
	return value <= avail ? std::round(value / avail) : 0;
}

glm::ivec2 GUI2Dynamic::avail_ratio(glm::vec2 value)
{
	return glm::ivec2(avail_ratio(value.x), avail_ratio(value.y));
}

glm::ivec2 GUI2Dynamic::avail_ratio(glm::vec4 value)
{
	return glm::ivec2(avail_ratio(value.x) + avail_ratio(value.z), avail_ratio(value.y) + avail_ratio(value.w));
}

float GUI2Dynamic::non_avail(float value)
{
	return is_avail(value) ? 0 : value;
}

glm::vec2 GUI2Dynamic::non_avail(glm::vec2 value)
{
	return glm::vec2(non_avail(value.x), non_avail(value.y));
}

glm::vec2 GUI2Dynamic::non_avail(glm::vec4 value)
{
	return glm::vec2(non_avail(value.x) + non_avail(value.z), non_avail(value.y) + non_avail(value.w));
}

float GUI2Dynamic::compute_size_per_avail(float remaining_size, int32_t avail_total)
{
	return glm::max(avail_total != 0 ? remaining_size / avail_total : 0, 0.0f);
}

glm::vec2 GUI2Dynamic::compute_size_per_avail(glm::vec2 remaining_size, glm::ivec2 avail_total)
{
	return glm::vec2(
		compute_size_per_avail(remaining_size.x, avail_total.x),
		compute_size_per_avail(remaining_size.y, avail_total.y)
	);
}

float GUI2Dynamic::compute_physical_size(float value, float size_per_avail)
{
	return is_avail(value) ? avail_ratio(value) * size_per_avail : value;
}

glm::vec2 GUI2Dynamic::compute_physical_size(glm::vec2 value, glm::vec2 size_per_avail)
{
	return glm::vec2(compute_physical_size(value.x, size_per_avail.x), compute_physical_size(value.y, size_per_avail.y));
}

glm::vec2 GUI2Dynamic::compute_physical_size(glm::vec4 value, glm::vec2 size_per_avail)
{
	return glm::vec2(
		compute_physical_size(value.x, size_per_avail.x) + compute_physical_size(value.z, size_per_avail.x),
		compute_physical_size(value.y, size_per_avail.y) + compute_physical_size(value.w, size_per_avail.y));
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_padding(glm::vec4 value)
{
	padding = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_name(std::string value)
{
	name = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_color(glm::vec4 value)
{
	color = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_thickness(glm::vec4 value)
{
	border_thickness = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_rounding(glm::vec4 value)
{
	border_rounding = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_color0(glm::vec4 value)
{
	border_color0 = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_color1(glm::vec4 value)
{
	border_color1 = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_color2(glm::vec4 value)
{
	border_color2 = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_border_color3(glm::vec4 value)
{
	border_color3 = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_shadow_thickness(glm::vec4 value)
{
	shadow_thickness = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_shadow_color(glm::vec4 value)
{
	shadow_color = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_is_decorated(bool value)
{
	is_decorated = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_is_resizable(bool value)
{
	is_resizable = value;
	return *this;
}

GUI2Dynamic::WindowDesc& GUI2Dynamic::WindowDesc::set_position(glm::vec2 value)
{
	position = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_uv00(glm::vec2 value)
{
	uv00 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_uv11(glm::vec2 value)
{
	uv11 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_texture_handle(uint64_t value)
{
	texture_handle = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_margin(glm::vec4 value)
{
	margin = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_color(glm::vec4 value)
{
	color = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_thickness(glm::vec4 value)
{
	border_thickness = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_rounding(glm::vec4 value)
{
	border_rounding = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_color0(glm::vec4 value)
{
	border_color0 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_color1(glm::vec4 value)
{
	border_color1 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_color2(glm::vec4 value)
{
	border_color2 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_border_color3(glm::vec4 value)
{
	border_color3 = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_shadow_thickness(glm::vec4 value)
{
	shadow_thickness = value;
	return *this;
}

GUI2Dynamic::BoxDesc& GUI2Dynamic::BoxDesc::set_shadow_color(glm::vec4 value)
{
	shadow_color = value;
	return *this;
}


GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_margin(glm::vec4 value)
{
	margin = value;
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_padding(glm::vec4 value)
{
	padding = value;
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::add_column(float width) {
	columns.push_back(width);
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::add_row(float height) {
	rows.push_back(height);
	return *this;
}

GUI2Dynamic::GridDesc& GUI2Dynamic::GridDesc::set_permeable_event(bool value)
{
	permeable_events = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_margin(glm::vec4 value) {
	margin = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_padding(glm::vec4 value) {
	padding = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_target_size(glm::vec2 value) {
	target_size = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_min_size(glm::vec2 value) {
	min_size = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_max_size(glm::vec2 value) {
	max_size = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_spacing(float value) {
	spacing = value;
	return *this;
}

GUI2Dynamic::StackDesc& GUI2Dynamic::StackDesc::set_is_vertical(bool value) {
	is_vertical = value;
	return *this;
}

bool GUI2Dynamic::ResolvedProperties::does_exists()
{
	return node_id != Node::invalid_node;
}
