#include "GraphicsCortex.h"

// each graphic object will know how to render itself
// pipeline will only manage rendering collective scene such as for shadowmap rendering for lighting

// requires multiple graphic objects for multiple shaders >-> not ideal

// programs will find uniform values from graphics and lights automatically
// programs will expose those uniforms that is user dependent

void Renderer(std::vector<Mesh_s>, UnorderedMaterial_s, std::vector<AmbiantLight_s>, Camera_s, Program_s);

class GenericPipeline {
	std::unordered_map<std::string, std::unordered_map<std::string, Graphic_s>> _graphic_collections;
	void create_graphic_group(const std::string& group_name);
	void delete_graphic_group(const std::string& group_name);
	void add_graphic(const std::string& target_group_name, Graphic_s graphic);
	void add_graphic(const std::string& target_group_name, Graphic_s graphic, const std::string& graphic_name);
	void remove_graphic(const std::string& target_group_name, const std::string& graphic_name);
	void render_group(const std::string& target_group_name);
	void render_graphic(const std::string& target_gruop_name, const std::string& graphic_name);


};

int main() {
	Graphic_s g;

}