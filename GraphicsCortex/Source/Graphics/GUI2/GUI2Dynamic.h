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

	static constexpr float		fit   = -1;
	static constexpr float		avail = -1024 * 64;
	static constexpr size_t		invalid_id = 0;

	struct WindowDesc;
	struct BoxDesc;
	struct GridDesc;
	struct StackDesc;
	struct ResolvedProperties;

	void				new_frame(GUI2& gui);

	WindowDesc&			window_begin(size_t& id);
	WindowDesc&			window_begin(const std::string& idstr);
	WindowDesc&			window_prop();
	void				window_end();

	GridDesc&			grid_begin(size_t& id);
	GridDesc&			grid_begin();
	GridDesc&			grid_prop();
	void				grid_add_column(float width);
	void				grid_add_row(float height);
	void				grid_region(glm::ivec2 grid_index, glm::ivec2 grid_span = glm::ivec2(1, 1));
	void				grid_end();

	StackDesc&			stack_begin(size_t& id);
	StackDesc&			stack_begin();
	StackDesc&			stack_prop();
	void				stack_end();

	BoxDesc&			box_begin(size_t& id);
	BoxDesc&			box_begin();
	BoxDesc&			box_prop();
	void				box_end();

	void				print_nodes();
	void				print_layout();
	void				resolve(size_t root_node, bool verbose = false);
	void				resolve();
	
	ResolvedProperties  get_resolved_properties(size_t id);
	ResolvedProperties  get_resolved_properties(const std::string& idstr);
	GUI2::IOState&		get_io_state();
	int32_t				get_levels_under_cursor(); 

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
		
		WindowDesc& set_padding			(glm::vec4 value	);
		WindowDesc& set_target_size		(glm::vec2 value	);
		WindowDesc& set_min_size		(glm::vec2 value	);
		WindowDesc& set_max_size		(glm::vec2 value	);
		WindowDesc& set_name			(std::string value	);
		WindowDesc& set_color			(glm::vec4 value	);
		WindowDesc& set_border_thickness(glm::vec4 value	);
		WindowDesc& set_border_rounding	(glm::vec4 value	);
		WindowDesc& set_border_color0	(glm::vec4 value	);
		WindowDesc& set_border_color1	(glm::vec4 value	);
		WindowDesc& set_border_color2	(glm::vec4 value	);
		WindowDesc& set_border_color3	(glm::vec4 value	);
		WindowDesc& set_shadow_thickness(glm::vec4 value	);
		WindowDesc& set_shadow_color	(glm::vec4 value	);
		WindowDesc& set_is_decorated	(bool value			);
		WindowDesc& set_is_resizable	(bool value			);
		WindowDesc& set_position		(glm::vec2 value	);

	private:
		friend GUI2Dynamic;
		size_t		id					= invalid_id;
		glm::vec2	size				= glm::vec2(0);

		GUI2::MouseEvent	mouse_event	= GUI2::MouseEvent::None;
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
		uint64_t	texture_handle		= 0;

		BoxDesc&	set_margin			(glm::vec4	value );
		BoxDesc&	set_target_size		(glm::vec2	value );
		BoxDesc&	set_min_size		(glm::vec2	value );
		BoxDesc&	set_max_size		(glm::vec2	value );
		BoxDesc&	set_color			(glm::vec4	value );
		BoxDesc&	set_border_thickness(glm::vec4	value );
		BoxDesc&	set_border_rounding	(glm::vec4	value );
		BoxDesc&	set_border_color0	(glm::vec4	value );
		BoxDesc&	set_border_color1	(glm::vec4	value );
		BoxDesc&	set_border_color2	(glm::vec4	value );
		BoxDesc&	set_border_color3	(glm::vec4	value );
		BoxDesc&	set_shadow_thickness(glm::vec4	value );
		BoxDesc&	set_shadow_color	(glm::vec4	value );
		BoxDesc&	set_uv00			(glm::vec2	value );
		BoxDesc&	set_uv11			(glm::vec2	value );
		BoxDesc&	set_texture_handle	(uint64_t	value );

	private:
		friend GUI2Dynamic;
		size_t		id					= invalid_id;
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);

		GUI2::MouseEvent  mouse_event	= GUI2::MouseEvent::None;

	};

	struct GridDesc {

		glm::vec4	margin				= glm::vec4(0);
		glm::vec4	padding				= glm::vec4(10);
		glm::vec2	target_size			= glm::vec2(fit);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);
		
		std::vector<float> columns;
		std::vector<float> rows;

		bool		permeable_events	= false;

		GridDesc&	set_margin			(glm::vec4	value);
		GridDesc&	set_padding			(glm::vec4	value);
		GridDesc&	set_target_size		(glm::vec2	value);
		GridDesc&	set_min_size		(glm::vec2	value);
		GridDesc&	set_max_size		(glm::vec2	value);
		GridDesc&	add_column			(float width);
		GridDesc&	add_row				(float height);
		GridDesc&	set_permeable_event (bool value);

	private:
		friend GUI2Dynamic;
		size_t		id					= invalid_id;
		glm::ivec2	current_grid_index	= glm::ivec2(0, 0);
		glm::ivec2	current_grid_span	= glm::ivec2(0, 0);
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);

		GUI2::MouseEvent  mouse_event	= GUI2::MouseEvent::None;

	};

	struct StackDesc {

		glm::vec4	margin				= glm::vec4(0);
		glm::vec4	padding				= glm::vec4(10);
		glm::vec2	target_size			= glm::vec2(fit);
		glm::vec2	min_size			= glm::vec2(fit);
		glm::vec2	max_size			= glm::vec2(avail);

		float		spacing				= 10;
		bool		is_vertical			= true;

		StackDesc&	set_margin 		(glm::vec4	value);
		StackDesc&	set_padding 	(glm::vec4	value);
		StackDesc&	set_target_size (glm::vec2	value);
		StackDesc&	set_min_size 	(glm::vec2	value);
		StackDesc&	set_max_size 	(glm::vec2	value);
		StackDesc&	set_spacing 	(float		value);
		StackDesc&	set_is_vertical (bool		value);

	private:
		friend GUI2Dynamic;
		size_t		id					= invalid_id;
		glm::ivec2	grid_slot			= glm::ivec2(0, 0);
		glm::ivec2	grid_span			= glm::ivec2(0, 0);
		glm::vec2	position			= glm::vec2(0);
		glm::vec2	size				= glm::vec2(0);

		GUI2::MouseEvent  mouse_event	= GUI2::MouseEvent::None;

	};

	struct Node;
	struct ResolvedProperties {

		size_t				node_id		= Node::invalid_node;
		glm::vec2			position	= glm::vec2(0);
		glm::vec2			size		= glm::vec2(0);
		int32_t				level		= 0;
		GUI2::MouseEvent	event		= GUI2::None;
		bool does_exists();
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

	size_t			find_last_of_type(NodeType type);
	NodeType		get_type(const Node& node);
	size_t			push_node(size_t parent = Node::invalid_node);
	size_t			push_node(size_t parent, WindowDesc desc);
	size_t			push_node(size_t parent, BoxDesc desc);
	size_t			push_node(size_t parent, GridDesc desc);
	size_t			push_node(size_t parent, StackDesc desc);

	glm::vec2&		node_position(size_t node);
	glm::vec2&		node_size(size_t node);
	glm::vec4&		node_margin(size_t node);
	glm::vec4&		node_padding(size_t node);
	glm::vec4		get_node_padding_non_ref(size_t node);
	glm::vec2&		node_target_size(size_t node);
	glm::vec2&		node_min_size(size_t node);
	glm::vec2&		node_max_size(size_t node);
	glm::ivec2&		node_grid_index(size_t node);
	glm::ivec2&		node_grid_span(size_t node);
	size_t			get_node_id(size_t node);
	GUI2::MouseEvent&  node_mouse_event(size_t node);

	void			_traverse_nodes(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void			traverse_nodes_down(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void			traverse_nodes_up(size_t root_node, std::function<void(int32_t, size_t)> lambda_given_level_self);
	void			traverse_nodes_children(size_t parent_node, std::function<void(size_t)> lambda_given_self);

	void			resolve_phase0_fit(size_t root_node);
	void			resolve_phase1_avail_and_position(size_t root_node);
	void			resolve_phase2_mouse_event(size_t root_node);

	bool			is_avail(float value);
	bool			is_any_avail(glm::vec2 value);
	bool			is_any_avail(glm::vec4 value);
	int32_t			avail_ratio(float value);
	glm::ivec2		avail_ratio(glm::vec2 value);
	glm::ivec2		avail_ratio(glm::vec4 value);
	float			non_avail(float value);
	glm::vec2		non_avail(glm::vec2 value);
	glm::vec2		non_avail(glm::vec4 value);
	float 			compute_size_per_avail(float remaining_size, int32_t avail_total);
	glm::vec2  		compute_size_per_avail(glm::vec2 remaining_size, glm::ivec2 avail_total);

	float			compute_physical_size(float value, float size_per_avail);
	glm::vec2		compute_physical_size(glm::vec2 value, glm::vec2 size_per_avail);
	glm::vec2		compute_physical_size(glm::vec4 value, glm::vec2 size_per_avail);

	std::unordered_map<std::string, size_t>			idstr_to_id;
	std::unordered_map<size_t, ResolvedProperties>	resolved_properties;
	int32_t			levels_under_cursor = 0;
	GUI2::IOState	io_state;

	size_t next_id_to_generate = 1;
	size_t generate_id();

	static constexpr std::string gui_idstr_prefix = "::#";
};