#pragma once
#include <iostream>

typedef size_t component_type_id;
class Entity;
class Scene;

class Component {
public:

	enum UpdateType {
		AddedToScene,
		RemovedFromScene,

		Begin,

		Update,
		FixedUpdate,

		PreRender,
		PostRender,
		PrePhysics,
		PostPhysics,
	};

	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;
	Component() = default;
	virtual ~Component() = default;

	virtual void on_added_to_scene(Scene& scene) {};
	virtual void on_removed_from_scene(Scene& scene) {};

	virtual void on_begin() {};
	virtual void on_update() {};

	virtual void on_fixed_update() {};
	virtual void on_pre_render() {};
	virtual void on_post_render() {};

	virtual void on_pre_physics() {};
	virtual void on_post_physics() {};

	Entity* get_entity();

private:
	friend Entity;
	Entity* entity = nullptr;
};