#pragma once
#include "GUI2.h"

class GUI2Widgets {
public:

	void window_begin();
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

private:

	GUI2 gui;

};