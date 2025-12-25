#pragma once
#include "GUI2Dynamic.h"

//void				rect();
//void				text();
//void				image();
//
//void				textbox();
//void				slider();
//void				dragfloat();
//
//void				checkbox();
//void				button();
//void				imagebutton();
//
//void				combobox_begin();
//void				combobox_item();
//void				combobox_end();
//
//void				menu_begin();
//void				menu_item();
//void				menu_end();
//
//void				tab_begin();
//void				tab_item();
//void				tab_end();

struct Rect {


	Rect& set_color(glm::vec4);

	void publish(GUI2Dynamic& gui_dynamic);
};

class Widget {
public:
	virtual void publish(GUI2Dynamic& gui_dynaimc) = 0;
};

class Label : public Widget {
public:
	std::string text;
	void publish(GUI2Dynamic& gui_dynaimc) override {};
};

