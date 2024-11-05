#pragma once

#include <iostream>

class Component {
public:

	enum UpdateType {
		Activated,
		Deactivated,
		Initialized,
		Destructed,

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

	virtual void on_activated() {};
	virtual void on_deactivated() {};
	virtual void on_initialized() {};
	virtual void on_destructed() {};

	virtual void on_begin() {};
	virtual void on_update() {};

	virtual void on_fixed_update() {};
	virtual void on_pre_render() {};
	virtual void on_post_render() {};

	virtual void on_pre_physics() {};
	virtual void on_post_physics() {};
};