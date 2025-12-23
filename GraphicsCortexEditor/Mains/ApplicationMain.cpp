#include "GraphicsCortex.h"

#include "Editor.h"
#include "Panels/Panel_Inspector.h"


void main() {

	Editor editor;
	
	editor.push_panel<Panel_Inspector>();
	
	editor.run();
}