#pragma once

#include "glm.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class GUI2 {
public:

	void init();
	void release();

	static constexpr float		fit		= 0;
	static constexpr float		avail	= -1024*1024;

	static constexpr float		fit1	= fit;
	static constexpr glm::vec2	fit2	= glm::vec2(fit);
	static constexpr glm::vec3	fit3	= glm::vec3(fit);
	static constexpr glm::vec4	fit4	= glm::vec4(fit);

	static constexpr float		avail1	= avail;
	static constexpr glm::vec2	avail2	= glm::vec2(avail);
	static constexpr glm::vec3	avail3	= glm::vec3(avail);
	static constexpr glm::vec4	avail4	= glm::vec4(avail);

	struct WindowDesc;
	struct BoxDesc;
	struct TextDesc;
	struct ImageDesc;
	struct GridDesc;
	struct StackDesc;

	struct WindowResult;
	struct BoxResult;
	struct TextResult;
	struct ImageResult;
	struct GridResult;
	struct StackResult;
	
	struct Layout;
	struct IOState;

	WindowResult	window(const std::string& idstr, WindowDesc& desc);
	void			window_end();

	BoxResult		box(BoxDesc& desc);

	Layout			generate_layout(IOState& io_state);

	struct WindowDesc {
		const char* name;
		
		glm::vec2 position;
		glm::vec2 size;

		glm::vec4 color;
		glm::vec4 border_thickness;
		glm::vec4 border_rounding;
		glm::vec4 border_color0;
		glm::vec4 border_color1;
		glm::vec4 border_color2;
		glm::vec4 border_color3;
		glm::vec4 shadow_thickness;
		glm::vec4 shadow_color;

		bool is_grapable;
		bool is_resizable;
		//bool is_dockable;
		//bool is_a_dock_surface;
	};

	struct WindowResult {

	};

	struct DrawCommand {
		enum Type {
			Clear,
			DrawTriangle,
			BufferLoadData,
			BufferResize,
		};

		union {
			glm::vec4 color;
			struct {
				void* data;
				size_t begin;
				size_t size;
			};
			struct {
				size_t program_id;
				size_t begin;
				size_t size;
			};
		};
	};

	struct WindowCommand {
		enum Type {
			Create,
			Rename,
			Resize,
			Reposition,
			Destroy
		};

		union {
			size_t id;
			const char* name;
			glm::ivec2 size;
			glm::ivec2 position;
		};
	};

	struct IOState {

	};

	struct Layout {

		int32_t window_count;
		size_t* draw_command_count_per_window;
		DrawCommand** commands;

	};

private:

	struct Node {
		
		enum Type {
			Window,
			Box,
		};

		Type type;
		void* data;

		size_t parent;
		size_t sibling;
		size_t child;
	};

	std::vector<Node> nodes;
	
	std::vector<size_t> window_stack;

	std::unordered_map<std::string, WindowDesc> windows_desc;
	std::unordered_map<std::string, WindowDesc> windows_state;
};