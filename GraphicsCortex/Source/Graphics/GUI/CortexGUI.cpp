#include "CortexGUI.h"

GUI& GUI::get()
{
	static GUI gui;
	return gui;
}
