#pragma once
#include "GUI2.h"

class GUI2Widgets {
public:

	static constexpr float		fit = 0;
	static constexpr float		avail = -1024 * 1024;

	static constexpr float		fit1 = fit;
	static constexpr glm::vec2	fit2 = glm::vec2(fit);
	static constexpr glm::vec3	fit3 = glm::vec3(fit);
	static constexpr glm::vec4	fit4 = glm::vec4(fit);

	static constexpr float		avail1 = avail;
	static constexpr glm::vec2	avail2 = glm::vec2(avail);
	static constexpr glm::vec3	avail3 = glm::vec3(avail);
	static constexpr glm::vec4	avail4 = glm::vec4(avail);

	void window_begin(const std::string& idstr);
	void window_end();

	void grid_begin();
	void grid_end();

	void stack_begin();
	void stack_end();

	void text();
	void image();
	void box();

	void textbox();
	void slider();
	void dragfloat();

	void checkbox();
	void button();
	void imagebutton();

	void combobox_begin();
	void combobox_item();
	void combobox_end();

	void menu_begin();
	void menu_item();
	void menu_end();

	void tab_begin();
	void tab_item();
	void tab_end();

private:

	GUI2 gui;

};