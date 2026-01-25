#include "GUIDynamic.h"
#include <queue>
#include "GLMCout.h"
#include "Monitor.h"

size_t GUIDynamic::find_last_of_type(NodeType type)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::find_last_of_type() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0)
		return Node::invalid_node;

	size_t traversing_node = layout.node_stack.size() - 1;

	while (traversing_node != -1 && get_type(layout.nodes[layout.node_stack[traversing_node]]) != type)
		traversing_node--;

	size_t found_node = traversing_node != -1 ?
						traversing_node : 
						Node::invalid_node;

	return layout.node_stack[traversing_node];
}

GUIDynamic::NodeType GUIDynamic::get_type(const Node& node)
{
	switch (node.desc.index()) {
	case 0:	return NodeType::Window;
	case 1: return NodeType::Box;
	case 2: return NodeType::Grid;
	case 3: return NodeType::Stack;
	}
}

size_t GUIDynamic::push_node(size_t parent)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::push_node() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.nodes.size() <= parent && parent != Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::push_node() is called but given parent_id is not valid" << std::endl;
		ASSERT(false);
	}
	
	Node node;
	node.parent = parent;

	size_t id = layout.nodes.size();
	layout.nodes.push_back(node);

	if (parent == Node::invalid_node) {
		return id;
	}

	if (layout.nodes[parent].child == Node::invalid_node) {
		layout.nodes[parent].child = id;
	}
	else {
		size_t sibling			= Node::invalid_node;
		size_t next_sibling		= layout.nodes[parent].child;
		
		while (next_sibling != Node::invalid_node) {
			sibling			= next_sibling;
			next_sibling	= layout.nodes[next_sibling].sibling;
		} 

		layout.nodes[sibling].sibling = id;
	}
	return id;
}

size_t GUIDynamic::push_node(size_t parent, WindowDesc desc)
{
	LayoutState& layout = layout_states[layout_stack.back()];

	size_t id = push_node(parent);
	layout.nodes[id].desc = desc;
	return id;
}

size_t GUIDynamic::push_node(size_t parent, BoxDesc desc)
{
	LayoutState& layout = layout_states[layout_stack.back()];

	size_t id = push_node(parent);
	layout.nodes[id].desc = desc;
	return id;
}

size_t GUIDynamic::push_node(size_t parent, GridDesc desc)
{
	LayoutState& layout = layout_states[layout_stack.back()];

	size_t id = push_node(parent);
	layout.nodes[id].desc = desc;
	return id;
}

size_t GUIDynamic::push_node(size_t parent, StackDesc desc)
{
	LayoutState& layout = layout_states[layout_stack.back()];

	size_t id = push_node(parent);
	layout.nodes[id].desc = desc;
	return id;
}

glm::vec2& GUIDynamic::node_position(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_position() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_position() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).position;
	case Box	: return std::get<BoxDesc>(node.desc).position;
	case Grid	: return std::get<GridDesc>(node.desc).position;
	case Stack	: return std::get<StackDesc>(node.desc).position;
	}

	std::cout << "[GUI Error] GUIDynamic::node_position() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUIDynamic::node_size(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_size() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).size;
	case Box	: return std::get<BoxDesc>(node.desc).size;
	case Grid	: return std::get<GridDesc>(node.desc).size;
	case Stack	: return std::get<StackDesc>(node.desc).size;
	}

	std::cout << "[GUI Error] GUIDynamic::node_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec4& GUIDynamic::node_margin(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_margin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_margin() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).margin;
	case Box	: return std::get<BoxDesc>(node.desc).margin;
	case Grid	: return std::get<GridDesc>(node.desc).margin;
	case Stack	: return std::get<StackDesc>(node.desc).margin;
	}

	std::cout << "[GUI Error] GUIDynamic::node_margin() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec4& GUIDynamic::node_padding(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_padding() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_padding() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).padding;
	//case Box	: return std::get<BoxDesc>(node.desc).padding;
	case Grid	: return std::get<GridDesc>(node.desc).padding;
	case Stack	: return std::get<StackDesc>(node.desc).padding;
	}

	std::cout << "[GUI Error] GUIDynamic::node_padding() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec4 GUIDynamic::get_node_padding_non_ref(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::get_node_padding_non_ref() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::get_node_padding_non_ref() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	return get_type(layout.nodes[node_id]) == NodeType::Box ? glm::vec4(0) : node_padding(node_id);
}

glm::vec2& GUIDynamic::node_target_size(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_target_size() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_target_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).target_size;
	case Box	: return std::get<BoxDesc>(node.desc).target_size;
	case Grid	: return std::get<GridDesc>(node.desc).target_size;
	case Stack	: return std::get<StackDesc>(node.desc).target_size;
	}

	std::cout << "[GUI Error] GUIDynamic::node_target_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUIDynamic::node_min_size(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_min_size() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_min_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).min_size;
	case Box	: return std::get<BoxDesc>(node.desc).min_size;
	case Grid	: return std::get<GridDesc>(node.desc).min_size;
	case Stack	: return std::get<StackDesc>(node.desc).min_size;
	}

	std::cout << "[GUI Error] GUIDynamic::node_min_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::vec2& GUIDynamic::node_max_size(size_t node_id){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_max_size() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_max_size() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).max_size;
	case Grid	: return std::get<GridDesc>(node.desc).max_size;
	case Stack	: return std::get<StackDesc>(node.desc).max_size;
	}

	std::cout << "[GUI Error] GUIDynamic::node_max_size() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::ivec2& GUIDynamic::node_grid_index(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_grid_index() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_grid_index() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).grid_slot;
	case Grid	: return std::get<GridDesc>(node.desc).grid_slot;
	case Stack	: return std::get<StackDesc>(node.desc).grid_slot;
	}

	std::cout << "[GUI Error] GUIDynamic::node_grid_index() but an error is occured" << std::endl;
	ASSERT(false);
}

glm::ivec2& GUIDynamic::node_grid_span(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_grid_span() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_grid_span() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	//case Window	: return std::get<WindowDesc>(node.desc).max_size;
	case Box	: return std::get<BoxDesc>(node.desc).grid_span;
	case Grid	: return std::get<GridDesc>(node.desc).grid_span;
	case Stack	: return std::get<StackDesc>(node.desc).grid_span;
	}

	std::cout << "[GUI Error] GUIDynamic::node_grid_span() but an error is occured" << std::endl;
	ASSERT(false);
}

size_t GUIDynamic::get_node_id(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::get_node_id() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::get_node_id() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).id;
	case Box	: return std::get<BoxDesc>(node.desc).id;
	case Grid	: return std::get<GridDesc>(node.desc).id;
	case Stack	: return std::get<StackDesc>(node.desc).id;
	}

	std::cout << "[GUI Error] GUIDynamic::get_node_id() but an error is occured" << std::endl;
	ASSERT(false);
}
GUI::MouseEvent& GUIDynamic::node_mouse_event(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_mouse_event() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_mouse_event() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window	: return std::get<WindowDesc>(node.desc).mouse_event;
	case Box	: return std::get<BoxDesc>(node.desc).mouse_event;
	case Grid	: return std::get<GridDesc>(node.desc).mouse_event;
	case Stack	: return std::get<StackDesc>(node.desc).mouse_event;
	}

	std::cout << "[GUI Error] GUIDynamic::node_mouse_event() but an error is occured" << std::endl;
	ASSERT(false);

	GUI::MouseEvent null_event;
	return null_event;
}

bool& GUIDynamic::node_pass_through_events(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_pass_through_events() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_pass_through_events() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	//case Window: return std::get<WindowDesc>(node.desc).;
	case Box:	return std::get<BoxDesc>(node.desc).pass_through_events;
	case Grid:	return std::get<GridDesc>(node.desc).pass_through_events;
	case Stack: return std::get<StackDesc>(node.desc).pass_through_events;
	}

	std::cout << "[GUI Error] GUIDynamic::node_pass_through_events() but an error is occured" << std::endl;
	ASSERT(false);

	bool null_value;
	return null_value;
}

bool GUIDynamic::node_pass_through_events_non_ref(size_t node_id)
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::node_pass_through_events_non_ref() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (node_id >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::node_pass_through_events_non_ref() is called but given node is not valid" << std::endl;
		ASSERT(false);
	}

	Node& node = layout.nodes[node_id];

	switch (get_type(node)) {
	case Window: return true;
	case Box:	return std::get<BoxDesc>(node.desc).pass_through_events;
	case Grid:	return std::get<GridDesc>(node.desc).pass_through_events;
	case Stack: return std::get<StackDesc>(node.desc).pass_through_events;
	}

	std::cout << "[GUI Error] GUIDynamic::node_pass_through_events_non_ref() but an error is occured" << std::endl;
	ASSERT(false);

	return false;
}

GUIDynamic::GUIDynamic(bool update_gui_scale_from_monitor) :
	update_gui_scale_from_monitor(update_gui_scale_from_monitor)
{
	if (update_gui_scale_from_monitor)
		set_gui_scale(Monitor::get_all_monitors()[0].get_content_scale().x);
}

///////////		WINDOW		////////////

void GUIDynamic::new_frame() {
	new_frame(gui);
}

void GUIDynamic::new_frame(GUI& gui)
{
	if (update_gui_scale_from_monitor)
		set_gui_scale(Monitor::get_all_monitors()[0].get_content_scale().x);
	
	if (layout_stack.size() != 0) {
		std::cout << "[GUI Error] GUIDynamic::new_frame() is called but previous gui hierarchy were invalid" << std::endl;
		ASSERT(false);
	}

	layout_states.clear();
	layout_stack.clear();

	io_state = gui.get_io_state();

	for (auto iterator = resolved_properties.begin(); iterator != resolved_properties.end();) {
		auto& [key, value] = *iterator;
		
		if (value.node_id == Node::invalid_node) {
			iterator = resolved_properties.erase(iterator);
		}
		else {
			value.node_id = Node::invalid_node;
			iterator++;
		}
	}

	current_frame_timepoint = std::chrono::system_clock::now();
}

GUIDynamic::WindowDesc& GUIDynamic::window_begin(size_t& id){

	if (id == invalid_id)
		id = generate_id();

	layout_states.emplace_back();
	layout_stack.push_back(layout_states.size() - 1);

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() != 0) {
		std::cout << "[GUI Error] GUIDynamic::window_begin() is called but windows must be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}
	
	WindowDesc desc;
	desc.id = id;
	size_t node_id = push_node(Node::invalid_node, desc);
	
	layout.node_stack.push_back(node_id);
	layout.last_window = node_id;
	
	resolved_properties[id].node_id		= layout.last_window;
	resolved_properties[id].layout_id	= layout_stack.back();

	return window_prop();
}

GUIDynamic::WindowDesc& GUIDynamic::window_prop(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::window_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}
	
	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.last_window == Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::window_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<WindowDesc>(layout.nodes[layout.last_window].desc);
}

void GUIDynamic::window_end(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::window_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::window_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) != NodeType::Window) {
		std::cout << "[GUI Error] GUIDynamic::window_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	layout.node_stack.pop_back();
	layout.last_window = Node::invalid_node;

	resolve();
	
	layout_stack.pop_back();
}

///////////		GRID		////////////

GUIDynamic::GridDesc& GUIDynamic::grid_begin(size_t& id){
	
	if (id == invalid_id)
		id = generate_id();

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	auto& desc = grid_begin();
	std::get<GridDesc>(layout.nodes[layout.last_grid].desc).id = id;
	
	resolved_properties[id].node_id		= layout.last_grid;
	resolved_properties[id].layout_id	= layout_stack.back();
	return desc;
}

GUIDynamic::GridDesc& GUIDynamic::grid_begin(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];


	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_begin() is called but grids cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) == Window && layout.nodes[layout.node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::grid_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUIDynamic::grid_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span  = glm::ivec2(0, 0);
	if (get_type(layout.nodes[layout.node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span  = desc.current_grid_span;
	}

	GridDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(layout.node_stack.back(), desc);

	layout.node_stack.push_back(id);
	layout.last_grid = id;

	return grid_prop();
}

GUIDynamic::GridDesc& GUIDynamic::grid_prop(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.last_grid == Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::grid_prop() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<GridDesc>(layout.nodes[layout.last_grid].desc);
}

void GUIDynamic::grid_add_column(float target_width, float min_width, float max_width){
	grid_prop().columns.push_back(glm::vec3(target_width, min_width, max_width));
}

void GUIDynamic::grid_add_row(float target_height, float min_height, float max_height){
	grid_prop().rows.push_back(glm::vec3(target_height, min_height, max_height));
}

void GUIDynamic::grid_region(glm::ivec2 grid_index, glm::ivec2 grid_span){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_region() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.last_grid == Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::grid_region() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (glm::any(glm::lessThan(grid_index, glm::ivec2(0))) || glm::any(glm::lessThanEqual(grid_span, glm::ivec2(0)))) {
		std::cout << "[GUI Error] GUIDynamic::grid_region() is called but values for index and span are invalid" << std::endl;
		ASSERT(false);
	}

	GUIDynamic::GridDesc& desc = grid_prop();

	//if (glm::any(glm::greaterThan(grid_index + grid_span, glm::ivec2(desc.columns.size(), desc.rows.size())))) {
	//	std::cout << "[GUI Error] GUIDynamic::grid_region() is called but values the given region exceeds the layout" << std::endl;
	//	ASSERT(false);
	//}

	desc.current_grid_index = grid_index;
	desc.current_grid_span	= grid_span;
}

void GUIDynamic::grid_end(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_end() is called but there are no grids in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) != NodeType::Grid) {
		std::cout << "[GUI Error] GUIDynamic::grid_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	auto& desc = grid_prop();

	if (desc.columns.size() == 0 || desc.rows.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::grid_end() is called without specifying at least one column and row" << std::endl;
		ASSERT(false);
	}

	//for (float column : desc.columns) {
	//	if (column == fit)
	//	{
	//		std::cout << "[GUI Error] GUIDynamic::grid_end() is called but (fit) is not a valid value for a column" << std::endl;
	//		ASSERT(false);
	//	}
	//}
	//
	//for (float row : desc.rows) {
	//	if (row == fit)
	//	{
	//		std::cout << "[GUI Error] GUIDynamic::grid_end() is called but (fit) is not a valid value for a row" << std::endl;
	//		ASSERT(false);
	//	}
	//}

	desc.current_grid_index	= glm::ivec2(0, 0);
	desc.current_grid_span	= glm::ivec2(0, 0);

	layout.node_stack.pop_back();
	layout.last_grid = find_last_of_type(NodeType::Grid);
}

///////////		STACK		////////////

GUIDynamic::StackDesc& GUIDynamic::stack_begin(size_t& id){
	
	if (id == invalid_id)
		id = generate_id();

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	auto& desc = stack_begin();
	std::get<StackDesc>(layout.nodes[layout.last_stack].desc).id = id;
	
	resolved_properties[id].node_id		= layout.last_stack;
	resolved_properties[id].layout_id	= layout_stack.back();

	return desc;
}

GUIDynamic::StackDesc& GUIDynamic::stack_begin(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_begin() is called but stacks cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) == Window && layout.nodes[layout.node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::stack_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUIDynamic::stack_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span  = glm::ivec2(0, 0);
	if (get_type(layout.nodes[layout.node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span  = desc.current_grid_span;
	}

	StackDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(layout.node_stack.back(), desc);

	layout.node_stack.push_back(id);
	layout.last_stack = id;

	return stack_prop();
}

GUIDynamic::StackDesc& GUIDynamic::stack_prop(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.last_stack == Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::stack_prop() is called but there are no stacks in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<StackDesc>(layout.nodes[layout.last_stack].desc);
}

void GUIDynamic::stack_end(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::stack_end() is called but there are no stacks in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) != NodeType::Stack) {
		std::cout << "[GUI Error] GUIDynamic::stack_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	layout.node_stack.pop_back();
	layout.last_stack = find_last_of_type(NodeType::Stack);

}

///////////		BOX		////////////


GUIDynamic::BoxDesc& GUIDynamic::box_begin(size_t& id){
	
	if (id == invalid_id)
		id = generate_id();

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	auto& desc = box_begin();
	
	std::get<BoxDesc>(layout.nodes[layout.last_box].desc).id = id;
	resolved_properties[id].node_id		= layout.last_box;
	resolved_properties[id].layout_id	= layout_stack.back();

	return desc;
}

GUIDynamic::BoxDesc& GUIDynamic::box_begin(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_begin() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_begin() is called but boxs cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box)
		box_end();

	if (get_type(layout.nodes[layout.node_stack.back()]) == Window && layout.nodes[layout.node_stack.back()].child != Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::box_begin() is called under a window but windows can have only one child" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) == Box) {
		std::cout << "[GUI Error] GUIDynamic::box_begin() is called under a box but boxes cannot have any children" << std::endl;
		ASSERT(false);
	}

	glm::ivec2 grid_index = glm::ivec2(0, 0);
	glm::ivec2 grid_span = glm::ivec2(0, 0);
	if (get_type(layout.nodes[layout.node_stack.back()]) == NodeType::Grid) {
		auto& desc = grid_prop();
		grid_index = desc.current_grid_index;
		grid_span = desc.current_grid_span;
	}

	BoxDesc desc;
	desc.grid_slot = grid_index;
	desc.grid_span = grid_span;

	size_t id = push_node(layout.node_stack.back(), desc);

	layout.node_stack.push_back(id);
	layout.last_box = id;

	return box_prop();
}

GUIDynamic::BoxDesc& GUIDynamic::box_prop(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_prop() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_prop() is called but boxs cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (layout.last_box == Node::invalid_node) {
		std::cout << "[GUI Error] GUIDynamic::box_prop() is called but there are no boxes in the hierarchy" << std::endl;
		ASSERT(false);
	}

	return std::get<BoxDesc>(layout.nodes[layout.last_box].desc);

}

void GUIDynamic::box_end(){

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_end() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_end() is called but boxs cannot be a root node in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (layout.node_stack.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::box_end() is called but there are no boxes in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (get_type(layout.nodes[layout.node_stack.back()]) != NodeType::Box) {
		std::cout << "[GUI Error] GUIDynamic::box_end() is called without finishing definition of an another object" << std::endl;
		ASSERT(false);
	}

	layout.node_stack.pop_back();
	layout.last_box = find_last_of_type(NodeType::Box);

}

///////////		TRAVERSAL		////////////

//																	  level	   self
void GUIDynamic::_traverse_nodes(std::function<void(int32_t, size_t)> lambda) {

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::_traverse_nodes() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.nodes.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_down() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	struct node_info {
		int32_t level;
		size_t node;
	};
	std::vector<node_info> stack;

	stack.push_back({0, 0});

	while (stack.size() != 0) {

		node_info info	= stack.back();
		stack.pop_back();

		Node& node = layout.nodes[info.node];

		std::vector<size_t> children;
		size_t iterating_child = node.child;
		while (iterating_child != Node::invalid_node) {

			children.push_back(iterating_child);
			iterating_child = layout.nodes[iterating_child].sibling;

		}

		for (size_t i = children.size() - 1; i != -1; i--)
			stack.push_back({ info.level+1, children[i] });

		lambda(info.level, info.node);

	}

}

void GUIDynamic::traverse_nodes_down(std::function<void(int32_t, size_t)> lambda) {
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_down() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.nodes.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_down() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}
	
	size_t	node_index = 0;
	int32_t level = 0;

	do {
		
		Node& node = layout.nodes[node_index];
		
		lambda(level, node_index);
		
		if (node.child != Node::invalid_node)
			level++;
		else if (node.sibling == Node::invalid_node)	
			level--;

		node_index++;

	} while (level != 0 && node_index < layout.nodes.size());
}

void GUIDynamic::traverse_nodes_up(std::function<void(int32_t, size_t)> lambda) {

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_up() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (layout.nodes.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_up() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	size_t	node_index = layout.nodes.size() - 1;
	int32_t level = 0;

	do {

		Node& node = layout.nodes[node_index];

		if (node.child != Node::invalid_node)
			level--;
		else if (node.sibling == Node::invalid_node)
			level++;

		lambda(level, node_index);
		
		node_index--;

	} while (node_index >= 0 && node_index != -1);
}

void GUIDynamic::traverse_nodes_children(size_t parent_node, std::function<void(size_t)> lambda) {

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_children() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	if (parent_node >= layout.nodes.size()) {
		std::cout << "[GUI Error] GUIDynamic::traverse_nodes_children() is called but given root_node doesn't exist" << std::endl;
		ASSERT(false);
	}

	size_t child = layout.nodes[parent_node].child;

	while (child != Node::invalid_node) {
		lambda(child);
		child = layout.nodes[child].sibling;
	}
}

void GUIDynamic::print_nodes() {

	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::print_nodes() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	for (size_t i = 0; i < layout.nodes.size(); i++) {
		
		Node& node = layout.nodes[i];

		NodeType type		= get_type(node);
		std::string name	= 
			type == NodeType::Window	? "Window"	:
			type == NodeType::Box		? "Box"		:
			type == NodeType::Grid		? "Grid"	:
			type == NodeType::Stack		? "Stack"	: "Error";

		std::cout << i << "\t\t" << name << "\tchild: " << node.child << " sib:" << node.sibling << " parent:" << node.parent << std::endl;
	}
		
}

void GUIDynamic::print_layout()
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::print_nodes() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	int32_t prev_current_layout = layout_stack.back();

	for (int32_t i = 0; i < layout_states.size(); i++) {
		
		LayoutState& layout = layout_states[i];
		layout_stack.back() = i;

		traverse_nodes_down([&](int32_t level, size_t node) {

			NodeType type = get_type(layout.nodes[node]);
			std::string name =
				type == NodeType::Window ? "Window" :
				type == NodeType::Box ? "Box" :
				type == NodeType::Grid ? "Grid" :
				type == NodeType::Stack ? "Stack" : "Error";

			for (int32_t i = 0; i < level; i++)
				std::cout << " | ";
			std::cout << name;
			std::cout << " size: " << node_size(node);
			std::cout << " position: " << node_position(node);
			std::cout << " target_size: " << node_target_size(node);
			std::cout << " min_size: " << node_min_size(node);
			std::cout << " max_size: " << node_max_size(node);
			if (type != NodeType::Window)	std::cout << " margin: " << node_margin(node);
			if (type != NodeType::Box)		std::cout << " padding: " << node_padding(node);
			std::cout << std::endl;

			});
	}
	
	layout_stack.back() = prev_current_layout;
}

///////////		RESOLVE		////////////

void GUIDynamic::resolve(bool verbose)
{
	if (verbose) std::cout << "resolve" << std::endl;
	if (verbose) print_layout();
	
	resolve_phase0_fit();

	if (verbose) std::cout << "phase 0 " << std::endl;
	if (verbose) print_layout();

	resolve_phase1_avail_and_position();
	
	if (verbose) std::cout << "phase 1" << std::endl;
	if (verbose) print_layout();

	resolve_phase2_gui_scale();

	if (verbose) std::cout << "phase 2" << std::endl;
	if (verbose) print_layout();

	resolve_phase3_mouse_event();
}

GUIDynamic::ResolvedProperties GUIDynamic::get_resolved_properties(size_t id)
{
	if (resolved_properties.find(id) == resolved_properties.end())
		return ResolvedProperties();

	return resolved_properties.at(id);
}

glm::vec2 GUIDynamic::get_mouse_position_scale_independent()
{
	return (io_state.mouse_position - window_prop().position) / gui_scale;
}

GUI::IOState& GUIDynamic::get_io_state() {
	return io_state;
}

::Window* GUIDynamic::window_prop_handle()
{
	if (layout_states.size() == 0)
		return nullptr;

	return get_window_handle(std::get<WindowDesc>(layout_states[layout_stack.back()].nodes[0].desc).id);
}

ImmediateRenderer* GUIDynamic::window_prop_immediate_renderer()
{
	if (layout_states.size() == 0)
		return nullptr;

	return get_immediate_renderer(std::get<WindowDesc>(layout_states[layout_stack.back()].nodes[0].desc).id);
}

::Window* GUIDynamic::get_window_handle(size_t window_id)
{
	if (resolved_properties.find(window_id) == resolved_properties.end())
		return nullptr;

	ResolvedProperties props = resolved_properties.at(window_id);

	if (layout_states.size() <= props.layout_id)
		return nullptr;

	if (get_type(layout_states[props.layout_id].nodes[0]) != Window)
		return nullptr;

	
	if (gui.get_window_state(compute_window_string(window_id)) == nullptr)
		return nullptr;

	return gui.get_window_state(compute_window_string(window_id))->window.get();
}

ImmediateRenderer* GUIDynamic::get_immediate_renderer(size_t window_id)
{
	if (resolved_properties.find(window_id) == resolved_properties.end())
		return nullptr;

	ResolvedProperties props = resolved_properties.at(window_id);

	if (layout_states.size() <= props.layout_id || layout_states[props.layout_id].nodes.size() <= props.node_id)
		return nullptr;

	if (get_type(layout_states[props.layout_id].nodes[props.node_id]) != Window)
		return nullptr;

	if (gui.get_window_state(compute_window_string(window_id)) == nullptr)
		return nullptr;

	return gui.get_window_state(compute_window_string(window_id))->renderer.get();
}

size_t GUIDynamic::generate_id()
{
	size_t id = next_id_to_generate;
	next_id_to_generate++;
	return id;
}

void GUIDynamic::resolve_phase0_fit(){
	
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::resolve_phase0_fit() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	traverse_nodes_up([&](int32_t level, size_t node_id) {

		Node& node = layout.nodes[node_id];
		NodeType node_type = get_type(node);

		switch (node_type) {
		case Window:
		{
			auto& desc = std::get<WindowDesc>(node.desc);

			glm::vec2 self_padding = non_avail(desc.padding);

			if (node.child == Node::invalid_node) {

				if (desc.min_size.x == fit)		desc.min_size.x		= 0		+ self_padding.x;
				if (desc.min_size.y == fit)		desc.min_size.y		= 0		+ self_padding.y;
				if (desc.target_size.x == fit)  desc.target_size.x	= 0		+ self_padding.x;
				if (desc.target_size.y == fit)	desc.target_size.y	= 0		+ self_padding.y;
				if (desc.max_size.x == fit)		desc.max_size.x		= 0		+ self_padding.x;
				if (desc.max_size.y == fit)		desc.max_size.y		= 0		+ self_padding.y;
				if (desc.max_size.x == avail)	desc.max_size.x		= 10240	+ self_padding.x;
				if (desc.max_size.y == avail)	desc.max_size.y		= 10240	+ self_padding.y;

				break;
			}

			glm::vec2 child_target_size	= node_target_size(node.child);
			glm::vec2 child_min_size	= node_min_size(node.child);
			glm::vec2 child_max_size	= node_max_size(node.child);
			glm::vec2 child_margin		= non_avail(node_margin(node.child));

			if (desc.min_size.x		== fit)		desc.min_size.x		= child_min_size.x	+ child_margin.x + self_padding.x;
			if (desc.min_size.y		== fit)		desc.min_size.y		= child_min_size.y	+ child_margin.y + self_padding.y;
			if (desc.target_size.x	== fit)		desc.target_size.x	= child_min_size.x	+ child_margin.x + self_padding.x;
			if (desc.target_size.y	== fit)		desc.target_size.y	= child_min_size.y	+ child_margin.y + self_padding.y;
			if (desc.max_size.x		== fit)		desc.max_size.x		= child_min_size.x	+ child_margin.x + self_padding.x;
			if (desc.max_size.y		== fit)		desc.max_size.y		= child_min_size.y	+ child_margin.y + self_padding.y;
			if (desc.max_size.x		== avail)	desc.max_size.x		= 10240				+ child_margin.x + self_padding.x;
			if (desc.max_size.y		== avail)	desc.max_size.y		= 10240				+ child_margin.y + self_padding.y;

			break;
		}
		case Box:
			{ 
			auto& desc = std::get<BoxDesc>(node.desc);

			if (desc.min_size.x == fit)		desc.min_size.x		= 0;
			if (desc.min_size.y == fit)		desc.min_size.y		= 0;
			if (desc.target_size.x == fit)  desc.target_size.x	= 0;
			if (desc.target_size.y == fit)	desc.target_size.y	= 0;
			if (desc.max_size.x == fit)		desc.max_size.x		= 0;
			if (desc.max_size.y == fit)		desc.max_size.y		= 0;
			if (desc.max_size.x == avail)	desc.max_size.x		= 10240;
			if (desc.max_size.y == avail)	desc.max_size.y		= 10240;

			break;
		}
		case Grid:
		{
			auto& desc = std::get<GridDesc>(node.desc);
			
			glm::vec2 self_padding = desc.padding;

			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				
				if (desc.columns[column_id].z == avail) desc.columns[column_id].z = 10240;

				if (glm::any(glm::equal(desc.columns[column_id], glm::vec3(fit)))) {

					float max_value = 0;
					traverse_nodes_children(node_id, [&](size_t child) {
						if (node_grid_index(child).x == column_id) {
							if (node_grid_span(child).x != 1) {
								return;
								//std::cout << "[GUI Error] grid slot cannot be \"fit\" while containing multi-slot children" << std::endl;
								//ASSERT(false);
							}
							max_value = std::max(max_value, glm::clamp(non_avail(node_target_size(child).x), node_min_size(child).x, node_max_size(child).x)) + non_avail(node_margin(child).x);
						}
						});

					if (desc.columns[column_id].x == fit) desc.columns[column_id].x = max_value;
					if (desc.columns[column_id].y == fit) desc.columns[column_id].y = max_value;
					if (desc.columns[column_id].z == fit) desc.columns[column_id].z = max_value;
				}
			}
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {

				if (desc.rows[row_id].z == avail) desc.rows[row_id].z = 10240;

				if (glm::any(glm::equal(desc.rows[row_id], glm::vec3(fit)))) {
					float max_value = 0;
					traverse_nodes_children(node_id, [&](size_t child) {
						if (node_grid_index(child).y == row_id) {
							if (node_grid_span(child).y != 1) {
								return;
								//std::cout << "[GUI Error] grid slot cannot be \"fit\" while containing multi-slot children" << std::endl;
								//ASSERT(false);
							}
							
							max_value = std::max(max_value, glm::clamp(non_avail(node_target_size(child).y), node_min_size(child).y, node_max_size(child).y)) + non_avail(node_margin(child).y);
						}
						});

					if (desc.rows[row_id].x == fit) desc.rows[row_id].x = max_value;
					if (desc.rows[row_id].y == fit) desc.rows[row_id].y = max_value;
					if (desc.rows[row_id].z == fit) desc.rows[row_id].z = max_value;
				}
			}

			glm::vec2 grid_min_size = glm::vec2(0);

			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++)
				grid_min_size.x += glm::clamp(non_avail(desc.columns[column_id].y), desc.columns[column_id].x, desc.columns[column_id].z);

			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++)
				grid_min_size.y += glm::clamp(non_avail(desc.rows[row_id].y), desc.rows[row_id].x, desc.rows[row_id].z);

			if (desc.min_size.x == fit)		desc.min_size.x		= grid_min_size.x	+ self_padding.x;
			if (desc.min_size.y == fit)		desc.min_size.y		= grid_min_size.y	+ self_padding.y;
			if (desc.target_size.x == fit)  desc.target_size.x	= grid_min_size.x	+ self_padding.x;
			if (desc.target_size.y == fit)	desc.target_size.y	= grid_min_size.y	+ self_padding.y;
			if (desc.max_size.x == fit)		desc.max_size.x		= grid_min_size.x	+ self_padding.x;
			if (desc.max_size.y == fit)		desc.max_size.y		= grid_min_size.y	+ self_padding.y;
			if (desc.max_size.x == avail)	desc.max_size.x		= 10240				+ self_padding.x;
			if (desc.max_size.y == avail)	desc.max_size.y		= 10240				+ self_padding.y;

			break;
		}
		case Stack:
		{
			auto& desc = std::get<StackDesc>(node.desc);

			glm::vec2 self_padding = non_avail(desc.padding);

			bool there_is_fit = 
				glm::any(glm::equal(desc.target_size, glm::vec2(fit)))	|| 
				glm::any(glm::equal(desc.min_size, glm::vec2(fit)))		|| 
				glm::any(glm::equal(desc.max_size, glm::vec2(fit)));

			if (there_is_fit && node.child == Node::invalid_node) {
				
				if (desc.min_size.x == fit)		desc.min_size.x		= 0		+ self_padding.x + 0;
				if (desc.min_size.y == fit)		desc.min_size.y		= 0		+ self_padding.y + 0;
				if (desc.target_size.x == fit)  desc.target_size.x	= 0		+ self_padding.x + 0;
				if (desc.target_size.y == fit)	desc.target_size.y	= 0		+ self_padding.y + 0;
				if (desc.max_size.x == fit)		desc.max_size.x		= 0		+ self_padding.x + 0;
				if (desc.max_size.y == fit)		desc.max_size.y		= 0		+ self_padding.y + 0;
				if (desc.max_size.x == avail)	desc.max_size.x		= 10240	+ self_padding.x + 0;
				if (desc.max_size.y == avail)	desc.max_size.y		= 10240	+ self_padding.y + 0;

			}
			else if (there_is_fit) {

				glm::vec2 stack_min_size = glm::vec2(0);
				int32_t children_count = 0;
				traverse_nodes_children(node_id, [&](size_t child) {

					children_count++;
					glm::vec2 child_min_size = glm::clamp(non_avail(node_target_size(child)), node_min_size(child), node_max_size(child)) + non_avail(node_margin(child));

					if (desc.is_vertical) {
						stack_min_size.x = glm::max(stack_min_size.x, child_min_size.x);
						stack_min_size.y += child_min_size.y;
					}
					else {
						stack_min_size.x += child_min_size.x;
						stack_min_size.y = glm::max(stack_min_size.y, child_min_size.y);
					}

					});

				if (desc.is_vertical)	stack_min_size.y += desc.spacing * glm::max((children_count - 1), 0);
				else					stack_min_size.x += desc.spacing * glm::max((children_count - 1), 0);

				if (desc.target_size.x == fit)  desc.target_size.x	= stack_min_size.x	+ self_padding.x;
				if (desc.target_size.y == fit)	desc.target_size.y	= stack_min_size.y	+ self_padding.y;
				if (desc.min_size.x == fit)		desc.min_size.x		= stack_min_size.x	+ self_padding.x;
				if (desc.min_size.y == fit)		desc.min_size.y		= stack_min_size.y	+ self_padding.y;
				if (desc.max_size.x == fit)		desc.max_size.x		= stack_min_size.x	+ self_padding.x;
				if (desc.max_size.y == fit)		desc.max_size.y		= stack_min_size.y	+ self_padding.y;
				if (desc.max_size.x == avail)	desc.max_size.x		= 10240				+ self_padding.x;
				if (desc.max_size.y == avail)	desc.max_size.y		= 10240				+ self_padding.y;
			}

			break;
		}
		}
		});
}

void GUIDynamic::resolve_phase1_avail_and_position()
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::resolve_phase1_avail_and_position() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	LayoutState& layout = layout_states[layout_stack.back()];

	traverse_nodes_down([&](int32_t level, size_t node_id) {

		Node& node = layout.nodes[node_id];
		NodeType node_type = get_type(node);

		switch (node_type) {
		case Window:
		{
			auto& desc = std::get<WindowDesc>(node.desc);

			// handle maximize
			if (is_avail(desc.target_size.x))	desc.target_size.x	= 10240;
			if (is_avail(desc.target_size.y))	desc.target_size.y	= 10240;

			desc.size = glm::clamp(desc.target_size, desc.min_size, desc.max_size);

			if (node.child == Node::invalid_node)
				break;

			glm::vec2 available_content_size = 
				glm::max(
					desc.size - 
					glm::vec2(desc.padding.x + desc.padding.z, desc.padding.y + desc.padding.w),
					glm::vec2(0)
				);

			if (is_avail(node_target_size(node.child).x))	node_target_size(node.child).x	= available_content_size.x;
			if (is_avail(node_target_size(node.child).y))	node_target_size(node.child).y	= available_content_size.y;
			if (is_avail(node_min_size(node.child).x))		node_min_size(node.child).x		= available_content_size.x;
			if (is_avail(node_min_size(node.child).y))		node_min_size(node.child).y		= available_content_size.y;
			if (is_avail(node_max_size(node.child).x))		node_max_size(node.child).x		= available_content_size.x;
			if (is_avail(node_max_size(node.child).y))		node_max_size(node.child).y		= available_content_size.y;

			node_position(node.child) = glm::vec2(desc.padding.x, desc.padding.y);

			break;
		}
		case Box:
		{
			auto& desc = std::get<BoxDesc>(node.desc);
			desc.size = glm::clamp(desc.target_size, desc.min_size, desc.max_size);

			break;
		}
		case Grid:
		{
			auto& desc = std::get<GridDesc>(node.desc);
			
			desc.size = glm::clamp(desc.target_size, desc.min_size, desc.max_size);

			if (node.child == Node::invalid_node)
				return;


			glm::vec2 cells_min_size = glm::vec2(0);
			glm::ivec2 cells_avails_total = glm::ivec2(0);

			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				cells_avails_total.x += avail_ratio(desc.columns[column_id].x);
				cells_min_size.x += non_avail(desc.columns[column_id].x);
			}
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {
				cells_avails_total.y += avail_ratio(desc.rows[row_id].x);
				cells_min_size.y += non_avail(desc.rows[row_id].x);
			}

			glm::vec2 self_size = desc.size - glm::vec2(desc.padding.x + desc.padding.z, desc.padding.y + desc.padding.w);
			glm::vec2 size_per_avail = compute_size_per_avail(self_size - cells_min_size, cells_avails_total);

			float current_offset = 0;
			for (int32_t column_id = 0; column_id < desc.columns.size(); column_id++) {
				current_offset += compute_physical_size(desc.columns[column_id].x, size_per_avail.x);
				desc.columns[column_id].x = current_offset;
			}

			current_offset = 0;
			for (int32_t row_id = 0; row_id < desc.rows.size(); row_id++) {
				current_offset += compute_physical_size(desc.rows[row_id].x, size_per_avail.y);
				desc.rows[row_id].x = current_offset;
			}

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2& child_target_size = node_target_size(child_id);
				glm::vec4& child_margin_ref = node_margin(child_id);

				glm::ivec2 child_total_avail = avail_ratio(child_margin_ref) + avail_ratio(child_target_size);
				glm::vec2 child_non_avail_size = non_avail(child_margin_ref) + non_avail(child_target_size);

				//if (glm::all(glm::equal(child_total_avail, glm::ivec2(0))))
				//	return;

				glm::ivec2 child_index = node_grid_index(child_id);
				glm::ivec2 child_span = node_grid_span(child_id);

				glm::vec2 cell_size = glm::vec2(
					desc.columns[child_index.x + child_span.x - 1].x - (child_index.x > 0 ? desc.columns[child_index.x - 1].x : 0),
					desc.rows[child_index.y + child_span.y - 1].x - (child_index.y > 0 ? desc.rows[child_index.y - 1].x : 0)
				);

				glm::vec2 child_size_per_avail = compute_size_per_avail(cell_size - child_non_avail_size, child_total_avail);

				if (is_avail(child_target_size.x)) child_target_size.x = avail_ratio(child_target_size.x) * child_size_per_avail.x;
				if (is_avail(child_target_size.y)) child_target_size.y = avail_ratio(child_target_size.y) * child_size_per_avail.y;

				glm::vec2 cell_position =
					glm::vec2(
						child_index.x > 0 ? desc.columns[child_index.x - 1].x : 0,
						child_index.y > 0 ? desc.rows[child_index.y - 1].x : 0);

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
			auto& desc	= std::get<StackDesc>(node.desc);
			
			desc.size	= glm::clamp(desc.target_size, desc.min_size, desc.max_size);

			if (node.child == Node::invalid_node)
				return;

			glm::vec2	children_min_size		= desc.min_size;
			int32_t		children_total_avails	= 0;

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2 child_target_size = node_target_size(child_id);
				glm::vec4 child_margin		= node_margin(child_id);

				children_total_avails +=
					desc.is_vertical ?
					avail_ratio(child_target_size.y) + avail_ratio(child_margin).y :
					avail_ratio(child_target_size.x) + avail_ratio(child_margin).x;

				});

			glm::vec2 available_content_size = 
				desc.size - 
				glm::vec2(desc.padding.x + desc.padding.z, desc.padding.y + desc.padding.w);

			float size_per_avail = 0;

			bool total_avail_changed = true;

			while (total_avail_changed) {
				
				total_avail_changed = false;

				size_per_avail =
					desc.is_vertical ?
					compute_size_per_avail(available_content_size.y - children_min_size.y, children_total_avails) :
					compute_size_per_avail(available_content_size.x - children_min_size.x, children_total_avails);

				traverse_nodes_children(node_id, [&](size_t child_id) {
					
					float child_on_axis_target_size = desc.is_vertical ? node_target_size(child_id).y	: node_target_size(child_id).x;
					float child_on_axis_min_size	= desc.is_vertical ? node_min_size(child_id).y		: node_min_size(child_id).x;
					float child_on_axis_max_size	= desc.is_vertical ? node_max_size(child_id).y		: node_max_size(child_id).x;

					float clamped_size = glm::clamp(compute_physical_size(child_on_axis_target_size, size_per_avail), child_on_axis_min_size, child_on_axis_max_size);

					if (clamped_size != child_on_axis_target_size) {
						
						children_total_avails -= avail_ratio(child_on_axis_target_size);
						
						if (child_id != layout.nodes[node_id].child) 
							total_avail_changed = true;
					}

					if (desc.is_vertical)	node_target_size(child_id).y = clamped_size;
					if (!desc.is_vertical)	node_target_size(child_id).x = clamped_size;


					});
			}

			float current_position = 0;

			traverse_nodes_children(node_id, [&](size_t child_id) {

				glm::vec2& child_target_size_ref	= node_target_size(child_id);
				glm::vec2& child_min_size_ref		= node_min_size(child_id);
				glm::vec2& child_max_size_ref		= node_max_size(child_id);
				glm::vec4& child_margin_ref			= node_margin(child_id);

				glm::vec2& child_position_ref		= node_position(child_id);

				int32_t child_avail_off_side =
					desc.is_vertical ?
					avail_ratio(child_margin_ref.x) +
					avail_ratio(child_margin_ref.z) +
					avail_ratio(child_target_size_ref.x)
					:
					avail_ratio(child_margin_ref.y) +
					avail_ratio(child_margin_ref.w) +
					avail_ratio(child_target_size_ref.y);

				float child_physical_off_side =
					desc.is_vertical ?
					non_avail(child_margin_ref.x) +
					non_avail(child_margin_ref.z) +
					non_avail(child_target_size_ref.x)
					:
					non_avail(child_margin_ref.y) +
					non_avail(child_margin_ref.w) +
					non_avail(child_target_size_ref.y);

				float child_avail_per_size_off_side
					= compute_size_per_avail(
						(desc.is_vertical ? available_content_size.x : available_content_size.y) - child_physical_off_side,
						child_avail_off_side
					);

				float clamped_size = desc.is_vertical ?
					glm::clamp(compute_physical_size(child_target_size_ref.x, child_avail_per_size_off_side), child_min_size_ref.x, child_max_size_ref.x) :
					glm::clamp(compute_physical_size(child_target_size_ref.y, child_avail_per_size_off_side), child_min_size_ref.y, child_max_size_ref.y);

				if (
					( desc.is_vertical && clamped_size != child_target_size_ref.x && is_avail(child_target_size_ref.x)) || 
					(!desc.is_vertical && clamped_size != child_target_size_ref.y && is_avail(child_target_size_ref.y))
					) 
				{

					if (desc.is_vertical)	child_avail_off_side -= avail_ratio(child_target_size_ref.x);
					if (!desc.is_vertical)	child_avail_off_side -= avail_ratio(child_target_size_ref.y);

					child_physical_off_side += clamped_size;

					child_avail_per_size_off_side
						= compute_size_per_avail(
							(desc.is_vertical ? available_content_size.x : available_content_size.y) - child_physical_off_side,
							child_avail_off_side
						);
				}
				
				glm::vec2 effective_margin = glm::vec2(0);

				if (desc.is_vertical) {
					child_target_size_ref.x = clamped_size;
					effective_margin.x = compute_physical_size(child_margin_ref.x, child_avail_per_size_off_side);
					effective_margin.y = compute_physical_size(child_margin_ref.y, size_per_avail);
				}
				else {
					child_target_size_ref.y = clamped_size;
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

				current_position += desc.is_vertical ? child_target_size_ref.y : child_target_size_ref.x;
				current_position += desc.spacing;

				});

			break;
		}
		}

		});
}

void GUIDynamic::resolve_phase2_gui_scale()
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::resolve_phase2_gui_scale() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}
	
	LayoutState& layout = layout_states[layout_stack.back()];

	traverse_nodes_down([&](int32_t level, size_t node_id) {

		if (is_any_avail(node_size(node_id)))
			return;
		if (glm::any(glm::equal(node_size(node_id), glm::vec2(fit))))
			return;

		if (node_id != 0)
			node_position(node_id)	*= gui_scale;
		node_size(node_id)			*= gui_scale;
		
		if (get_type(layout.nodes[node_id]) == Box) {
			auto& desc = std::get<BoxDesc>(layout.nodes[node_id].desc);
			desc.border_rounding	*= gui_scale;
			desc.border_thickness	*= gui_scale;
			desc.shadow_thickness	*= gui_scale;
		}
		});
}

void GUIDynamic::resolve_phase3_mouse_event()
{
	if (layout_states.size() == 0) {
		std::cout << "[GUI Error] GUIDynamic::resolve_phase3_mouse_event() is called but there are no windows in the hierarchy" << std::endl;
		ASSERT(false);
	}

	if (resolved_properties.size() == 0)
		return;

	for (auto& entry : resolved_properties) {
		auto& [resolved_id, resolved_property] = entry;
		
		if (resolved_property.layout_id != layout_stack.back())
			continue;

		resolved_property.event = GUI::None;
	}

	glm::vec2&			mouse_position_ref	= io_state.mouse_position;
	GUI::MouseEvent		mouse_event_ref		= io_state.mouse_state;
	glm::vec2			root_position		= node_position(0);

	struct stack_info {
		size_t widget = 0;
		int32_t level = 0;
	};

	std::vector<size_t> captured_nodes;

	std::vector<stack_info> stack;
	stack.push_back({ .widget = 0, .level = 0 });

	while (stack.size() != 0) {

		auto [node_id, node_level] = stack.back();
		stack.pop_back();

		GUI::MouseEvent& event_ref = node_mouse_event(node_id);

		glm::vec2 position = node_id != 0 ? root_position + node_position(node_id) : root_position;
		glm::vec2 size = node_size(node_id);

		AABB2 node_area = AABB2(position, position + size);

		if (!node_area.does_contain(mouse_position_ref)) {
			event_ref = GUI::MouseEvent::None;
			continue;
		}

		if (!node_pass_through_events_non_ref(node_id)) {
			
			for (int32_t i = 0; i < captured_nodes.size(); i++) {
				node_mouse_event(captured_nodes[i]) = GUI::MouseEvent::None;

				size_t captured_resolved_id = get_node_id(captured_nodes[i]);

				if (resolved_properties.find(captured_resolved_id) != resolved_properties.end()) {
					if (resolved_properties[captured_resolved_id].layout_id != layout_stack.back())
						continue;
					resolved_properties[captured_resolved_id].event = GUI::MouseEvent::None;
				}
			}

			captured_nodes.clear();
		}

		captured_nodes.push_back(node_id);

		event_ref = io_state.mouse_state;
		size_t resolved_id = get_node_id(node_id);

		if (resolved_properties.find(resolved_id) != resolved_properties.end()) {
			
			if (resolved_properties[resolved_id].layout_id != layout_stack.back())
				continue;

			resolved_properties[resolved_id].event = event_ref;
			resolved_properties[resolved_id].level = node_level;
		}

		traverse_nodes_children(node_id, [&](size_t child) {
			stack.push_back({ child, node_level + 1 });
			});
	}

	for (auto& entry : resolved_properties) {
		auto& [resolved_id, resolved_property] = entry;
		size_t node_id = resolved_property.node_id;
		int32_t layout_id = resolved_property.layout_id;

		if (resolved_property.layout_id != layout_stack.back())
			continue;

		resolved_property.position =	node_id == Node::invalid_node ? glm::vec2(0) : node_position(node_id) / gui_scale;
		resolved_property.size =		node_id == Node::invalid_node ? glm::vec2(0) : node_size(node_id) / gui_scale;
	}
}

void GUIDynamic::publish() {
	publish(gui);
	gui.render();
}

void GUIDynamic::publish(GUI& gui)
{
	if (layout_states.size() == 0)
		return;

	int32_t prev_current_layout = layout_stack.back();

	for (int32_t layout_index = 0; layout_index < layout_states.size(); layout_index++) {
		layout_stack.back() = layout_index;

		LayoutState& layout = layout_states[layout_stack.back()];

		if (layout.node_stack.size() != 0) {
			std::cout << "GUIDynamic::publish() is called but hierarchy isn't completed yet" << std::endl;
			ASSERT(false);
		}

		if (get_type(layout.nodes[0]) != NodeType::Window) {
			std::cout << "GUIDynamic::publish() is called with invalid hierarchy" << std::endl;
			ASSERT(false);
		}

		{
			auto& desc = std::get<WindowDesc>(layout.nodes[0].desc);

			gui.window_begin(compute_window_string(desc.id),
				desc.size,
				desc.position
			);

			gui.window_prop().position = desc.position;
			gui.window_prop().size = desc.size;
			gui.window_prop().is_resizable = desc.is_resizable;
			gui.window_prop().is_decorated = desc.is_decorated;

			gui.window_prop().color = desc.color;
		}

		traverse_nodes_down([&](int32_t level, size_t child) {

			switch (get_type(layout.nodes[child])) {
			case Box:
			{
				auto& desc = std::get<BoxDesc>(layout.nodes[child].desc);
				gui.box_begin(desc.position, desc.size);
				gui.box_prop().color = desc.color;
				gui.box_prop().border_thickness = desc.border_thickness;
				gui.box_prop().border_rounding = desc.border_rounding;
				gui.box_prop().border_color0 = desc.border_color0;
				gui.box_prop().border_color1 = desc.border_color1;
				gui.box_prop().border_color2 = desc.border_color2;
				gui.box_prop().border_color3 = desc.border_color3;
				gui.box_prop().shadow_thickness = desc.shadow_thickness;
				gui.box_prop().shadow_color = desc.shadow_color;
				gui.box_prop().uv00 = desc.uv00;
				gui.box_prop().uv11 = desc.uv11;
				gui.box_prop().texture_handle = desc.texture_handle;
				gui.box_prop().z = (float)child;
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

	layout_stack.back() = prev_current_layout;
}

std::chrono::system_clock::time_point GUIDynamic::get_current_frame_timepoint()
{
	return current_frame_timepoint;
}

float GUIDynamic::get_gui_scale(){
	return gui_scale;
}

void  GUIDynamic::set_gui_scale(float gui_scale){
	this->gui_scale = gui_scale;
}

bool GUIDynamic::is_avail(float value) {
	return avail_ratio(value) != 0;
}

bool GUIDynamic::is_any_avail(glm::vec2 value)
{
	return is_avail(value.x) || is_avail(value.y);
}

bool GUIDynamic::is_any_avail(glm::vec4 value)
{
	return is_avail(value.x) || is_avail(value.y) || is_avail(value.z) || is_avail(value.w);
}

int32_t GUIDynamic::avail_ratio(float value) {
	return value <= avail ? std::round(value / avail) : 0;
}

glm::ivec2 GUIDynamic::avail_ratio(glm::vec2 value)
{
	return glm::ivec2(avail_ratio(value.x), avail_ratio(value.y));
}

glm::ivec2 GUIDynamic::avail_ratio(glm::vec4 value)
{
	return glm::ivec2(avail_ratio(value.x) + avail_ratio(value.z), avail_ratio(value.y) + avail_ratio(value.w));
}

float GUIDynamic::non_avail(float value)
{
	return is_avail(value) ? 0 : value;
}

glm::vec2 GUIDynamic::non_avail(glm::vec2 value)
{
	return glm::vec2(non_avail(value.x), non_avail(value.y));
}

glm::vec2 GUIDynamic::non_avail(glm::vec4 value)
{
	return glm::vec2(non_avail(value.x) + non_avail(value.z), non_avail(value.y) + non_avail(value.w));
}

float GUIDynamic::compute_size_per_avail(float remaining_size, int32_t avail_total)
{
	return glm::max(avail_total != 0 ? remaining_size / avail_total : 0, 0.0f);
}

glm::vec2 GUIDynamic::compute_size_per_avail(glm::vec2 remaining_size, glm::ivec2 avail_total)
{
	return glm::vec2(
		compute_size_per_avail(remaining_size.x, avail_total.x),
		compute_size_per_avail(remaining_size.y, avail_total.y)
	);
}

float GUIDynamic::compute_physical_size(float value, float size_per_avail)
{
	return is_avail(value) ? avail_ratio(value) * size_per_avail : value;
}

glm::vec2 GUIDynamic::compute_physical_size(glm::vec2 value, glm::vec2 size_per_avail)
{
	return glm::vec2(compute_physical_size(value.x, size_per_avail.x), compute_physical_size(value.y, size_per_avail.y));
}

glm::vec2 GUIDynamic::compute_physical_size(glm::vec4 value, glm::vec2 size_per_avail)
{
	return glm::vec2(
		compute_physical_size(value.x, size_per_avail.x) + compute_physical_size(value.z, size_per_avail.x),
		compute_physical_size(value.y, size_per_avail.y) + compute_physical_size(value.w, size_per_avail.y));
}

std::string GUIDynamic::compute_window_string(size_t id)
{
	return gui_idstr_prefix + "window" + std::to_string(id);
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_padding(glm::vec4 value)
{
	padding = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_name(std::string value)
{
	name = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_color(glm::vec4 value)
{
	color = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_thickness(glm::vec4 value)
{
	border_thickness = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_rounding(glm::vec4 value)
{
	border_rounding = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_color0(glm::vec4 value)
{
	border_color0 = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_color1(glm::vec4 value)
{
	border_color1 = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_color2(glm::vec4 value)
{
	border_color2 = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_border_color3(glm::vec4 value)
{
	border_color3 = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_shadow_thickness(glm::vec4 value)
{
	shadow_thickness = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_shadow_color(glm::vec4 value)
{
	shadow_color = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_is_decorated(bool value)
{
	is_decorated = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_is_resizable(bool value)
{
	is_resizable = value;
	return *this;
}

GUIDynamic::WindowDesc& GUIDynamic::WindowDesc::set_position(glm::vec2 value)
{
	position = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_uv00(glm::vec2 value)
{
	uv00 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_uv11(glm::vec2 value)
{
	uv11 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_texture_handle(size_t value)
{
	texture_handle = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_pass_through_events(bool value)
{
	 pass_through_events = value;
	 return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_margin(glm::vec4 value)
{
	margin = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_color(glm::vec4 value)
{
	color = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_thickness(glm::vec4 value)
{
	border_thickness = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_rounding(glm::vec4 value)
{
	border_rounding = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_color0(glm::vec4 value)
{
	border_color0 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_color1(glm::vec4 value)
{
	border_color1 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_color2(glm::vec4 value)
{
	border_color2 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_border_color3(glm::vec4 value)
{
	border_color3 = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_shadow_thickness(glm::vec4 value)
{
	shadow_thickness = value;
	return *this;
}

GUIDynamic::BoxDesc& GUIDynamic::BoxDesc::set_shadow_color(glm::vec4 value)
{
	shadow_color = value;
	return *this;
}


GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_margin(glm::vec4 value)
{
	margin = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_padding(glm::vec4 value)
{
	padding = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_target_size(glm::vec2 value)
{
	target_size = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_min_size(glm::vec2 value)
{
	min_size = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_max_size(glm::vec2 value)
{
	max_size = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::add_column(float target_width, float min_width, float max_width) {
	columns.push_back(glm::vec3(target_width, min_width, max_width));
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::add_row(float target_height, float min_height, float max_height) {
	rows.push_back(glm::vec3(target_height, min_height, max_height));
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_permeable_event(bool value)
{
	permeable_events = value;
	return *this;
}

GUIDynamic::GridDesc& GUIDynamic::GridDesc::set_pass_through_events(bool value)
{
	pass_through_events = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_margin(glm::vec4 value) {
	margin = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_padding(glm::vec4 value) {
	padding = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_target_size(glm::vec2 value) {
	target_size = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_min_size(glm::vec2 value) {
	min_size = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_max_size(glm::vec2 value) {
	max_size = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_spacing(float value) {
	spacing = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_is_vertical(bool value) {
	is_vertical = value;
	return *this;
}

GUIDynamic::StackDesc& GUIDynamic::StackDesc::set_pass_through_events(bool value)
{
	pass_through_events = value;
	return *this;
}

bool GUIDynamic::ResolvedProperties::does_exists()
{
	return node_id != Node::invalid_node;
}
