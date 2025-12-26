#pragma once
#include "GUI2.h"
#include <variant>
#include <array>

// rules of dynamic positioning
// all objects have the following properties: 
//   * "target_size"
//   * "min_size"
//   * "max_size"
// in addition, container objects also have "padding" and containing objects have "margin"
// 
// any values specified for these properties can be either physical or relative.
// definitions of physical and relative values depends on the type of object.
// 
//   * any positive value is always a physical value.
//   * (fit) and (avail) are always relative sizes.
// 
//   * if target_size is either (fit) or a physical size then min_size and max_size is ignored.
// 
// if an object is a container:
//   
//   * (fit) can be used to mean minimum possible size that is enough to fit the contained objects.
//   * if a (fit) sized container doesn't have any containing objects, it will have 0 physical size.
// 
// if an object is being contained:
//   
//   * (avail) can be used to mean maximum possible region the container can provide.
//   * any positive intager multiple of (avail) can be used to specify a ratio.
//   * total region requested by (avail) values will be partitioned to containing objects according to the coefficients of (avail)
//   * if an (avail) sized object is being contained by a (fit) sized container, the object will have 0 physical size.
// 
// special case for margin:  
// 
//   * margin can be (avail) or physical size.
//   * physical size definition for margin includes all 0, positive and negative values.
//
// special case for grid cells:
// 
//   * cell's of grids must have either (avail) or physical size.
//
// special case for stacks:
//
//   * "spacing" must be (avail) or physical size.
//

class GUI2Dynamic {
public:

	static constexpr float		fit = -1;
	static constexpr float		avail = -1024 * 64;

	static constexpr float		fit1 = fit;
	static constexpr glm::vec2	fit2 = glm::vec2(fit);
	static constexpr glm::vec3	fit3 = glm::vec3(fit);
	static constexpr glm::vec4	fit4 = glm::vec4(fit);

	static constexpr float		avail1 = avail;
	static constexpr glm::vec2	avail2 = glm::vec2(avail);
	static constexpr glm::vec3	avail3 = glm::vec3(avail);
	static constexpr glm::vec4	avail4 = glm::vec4(avail);

	struct WindowDesc;
	struct BoxDesc;
	struct GridDesc;
	struct StackDesc;

	void				window_begin(const std::string& idstr);
	WindowDesc&			window_prop();
	void				window_end();

	void				grid_begin(const std::string& idstr);
	void				grid_begin();
	GridDesc&			grid_prop();
	void				grid_add_column(float width);
	void				grid_add_row(float height);
	void				grid_region(glm::ivec2 grid_index, glm::ivec2 grid_span);
	void				grid_end();

	void				stack_begin(const std::string& idstr);
	void				stack_begin();
	StackDesc&			stack_prop();
	void				stack_end();

	void				box_begin(const std::string& idstr);
	void				box_begin();
	BoxDesc&			box_prop();
	void				box_end();

	void				print_nodes();
	void				print_layout();
	void				resolve();
	glm::vec2			get_size		(const std::string& idstr);
	glm::vec2			get_position	(const std::string& idstr);
	GUI2::MouseEvent	get_mouse_event	(const std::string& idstr);

	void				publish(GUI2& gui);

	struct WindowDesc {
		glm::vec4	padding				= glm::vec4(10);
		glm::vec2	target_size			= glm::vec2(fit);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);

		std::string name				= "CortexGUI2 Window";
		glm::vec4	color				= glm::vec4(1, 1, 1, 1);
		glm::vec4	border_thickness	= glm::vec4(0);
		glm::vec4	border_rounding		= glm::vec4(0);
		glm::vec4	border_color0		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color1		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color2		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color3		= glm::vec4(0, 0, 0, 1);
		glm::vec4	shadow_thickness	= glm::vec4(0);
		glm::vec4	shadow_color		= glm::vec4(0, 0, 0, 1);
		
		bool		is_decorated		= false;
		bool		is_resizable		= true;

		glm::vec2	position			= glm::vec2(100);

	private:
		friend GUI2Dynamic;
		std::string idstr;
		glm::vec2	size				= glm::vec2(0);
	};

	struct BoxDesc {
		glm::vec4	margin				= glm::vec4(0);
		glm::vec2	target_size			= glm::vec2(128);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);

		glm::vec4	color				= glm::vec4(1, 1, 1, 1);
		glm::vec4	border_thickness	= glm::vec4(0);
		glm::vec4	border_rounding		= glm::vec4(0);
		glm::vec4	border_color0		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color1		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color2		= glm::vec4(0, 0, 0, 1);
		glm::vec4	border_color3		= glm::vec4(0, 0, 0, 1);
		glm::vec4	shadow_thickness	= glm::vec4(0);
		glm::vec4	shadow_color		= glm::vec4(0, 0, 0, 1);
		
		glm::vec2	uv00				= glm::vec2(0);
		glm::vec2	uv11				= glm::vec2(1);
		uint32_t	texture_id			= -1;

	private:
		friend GUI2Dynamic;
		std::string idstr;
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);
	};

	struct GridDesc {
		glm::vec4	margin				= glm::vec4(0);
		glm::vec4	padding				= glm::vec4(10);
		glm::vec2	target_size			= glm::vec2(fit);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);
		
		std::vector<float> columns;
		std::vector<float> rows;

	private:
		friend GUI2Dynamic;
		std::string idstr;
		glm::ivec2	current_grid_index	= glm::ivec2(0, 0);
		glm::ivec2	current_grid_span	= glm::ivec2(0, 0);
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);
	};

	struct StackDesc {
		glm::vec4	margin				= glm::vec4(0);
		glm::vec4	padding				= glm::vec4(10);
		glm::vec2	target_size			= glm::vec2(fit);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);

		float		spacing				= 10;
		bool		is_vertical			= true;

	private:
		friend GUI2Dynamic;
		std::string idstr;
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);

	};

	struct ResolvedProperties {
		glm::vec2 position				= glm::vec2(0);
		glm::vec2 size					= glm::vec2(0);
		GUI2::MouseEvent event			= GUI2::None;
	};

private:

	enum NodeType {
		Window,
		Box,
		Grid,
		Stack,
	};

	struct Node {
		static constexpr size_t invalid_node = -1;
		
		std::variant<
			WindowDesc, 
			BoxDesc, 
			GridDesc, 
			StackDesc
		> desc			= BoxDesc();
		
		size_t parent	= invalid_node;
		size_t sibling	= invalid_node;
		size_t child	= invalid_node;
	};

	std::vector<size_t> node_stack;
	size_t last_window	= Node::invalid_node;
	size_t last_box		= Node::invalid_node;
	size_t last_grid	= Node::invalid_node;
	size_t last_stack	= Node::invalid_node;
	
	std::vector<size_t>	root_nodes;
	std::vector<Node> nodes;

	size_t find_last_of_type(NodeType type);
	NodeType get_type(const Node& node);
	size_t push_node(size_t parent = Node::invalid_node);
	size_t push_node(size_t parent, WindowDesc desc);
	size_t push_node(size_t parent, BoxDesc desc);
	size_t push_node(size_t parent, GridDesc desc);
	size_t push_node(size_t parent, StackDesc desc);

	glm::vec2&	node_position(size_t node);
	glm::vec2&	node_size(size_t node);
	glm::vec4&	node_margin(size_t node);
	glm::vec4&	node_padding(size_t node);
	glm::vec4	get_node_padding_non_ref(size_t node);
	glm::vec2&	node_target_size(size_t node);
	glm::vec2&	node_min_size(size_t node);
	glm::vec2&	node_max_size(size_t node);
	glm::ivec2&	node_grid_index(size_t node);
	glm::ivec2&	node_grid_span(size_t node);

	//														level	  self
	void _traverse_nodes(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void traverse_nodes_down(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void traverse_nodes_up(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void traverse_nodes_children(size_t parent_node, std::function<void(size_t)> lambda_given_self);

	void resolve_phase0_fit(size_t root_node);
	void resolve_phase1_avail_and_position(size_t root_node);
	void resolve_phase2_mouse_event(size_t root_node);

	//template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
	bool		is_avail(float value);
	bool		is_any_avail(glm::vec2 value);
	bool		is_any_avail(glm::vec4 value);
	int32_t		avail_ratio(float value);
	glm::ivec2	avail_ratio(glm::vec2 value);
	glm::ivec2	avail_ratio(glm::vec4 value);
	float		non_avail(float value);
	glm::vec2	non_avail(glm::vec2 value);
	glm::vec2	non_avail(glm::vec4 value);
	float 		compute_size_per_avail(float remaining_size, int32_t avail_total);
	glm::vec2  	compute_size_per_avail(glm::vec2 remaining_size, glm::ivec2 avail_total);

	float		compute_physical_size(float value, float size_per_avail);
	glm::vec2	compute_physical_size(glm::vec2 value, glm::vec2 size_per_avail);
	glm::vec2	compute_physical_size(glm::vec4 value, glm::vec2 size_per_avail);

	std::unordered_map<std::string, ResolvedProperties> resolved_properties;

	static constexpr std::string gui_idstr_prefix = "::#";
};