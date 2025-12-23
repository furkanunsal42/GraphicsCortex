#pragma once

#include "Editor.h"
#include "CortexGUIWidgets/Container/GUIWidget_Grid.h"
#include "CortexGUIWidgets/Container/GUIWidget_Stack.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Rectangle.h"
#include "CortexGUIWidgets/Basic/GUIWidget_Label.h"


class _ComponentBlock_Field_Scalar : public widget::Grid {

};

class _ComponentBlock_Field_Vector2 : public widget::Grid {

};

class _ComponentBlock_Field_Vector3 : public widget::Grid {

};

class _ComponentBlock_Field_Vector4 : public widget::Grid {

};

class _Inspector_ComponentBlock : public widget::Grid {
public:
	WidgetHandle<widget::Rectangle> header;
	WidgetHandle<widget::Label> component_name;

	WidgetHandle<widget::Stack> properties_stack;
};

class Panel_Inspector : public EditorPanel {
public:

	glm::vec2 position = glm::vec2(0, 0);

	WidgetHandle<widget::Grid> grid;
	WidgetHandle<widget::Label> header_label;

	void on_init() override 
	{ 
		grid = widget::create<widget::Grid>();
		grid->target_size = glm::vec2(300, -1);
		grid->color = glm::vec4(0.12, 0.12, 0.12, 1);
		grid->padding = glm::vec4(10);

		header_label = widget::create<widget::Label>();
		header_label->text = U"Inspector";
		header_label->font = 1;
		header_label->text_height = 16;
		header_label->text_color = glm::vec4(1, 1, 1, 1);
		
		grid->add_column(0);
		grid->add_row(0);
		grid->add(header_label, 0, 0);
	}
	
	void on_render() override 
	{ 
		GUI::get().render(grid, position);
	}

private:
	
};