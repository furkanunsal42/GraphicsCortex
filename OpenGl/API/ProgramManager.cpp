#include "ProgramManager.h"

#include <iostream>

void ProgramManager::update_uniforms(bool init) {
	std::cout << "[Opengl Error] virtual function ProgramManager.update_uniforms(init) have been called.\n";
}

ProgramManager& ProgramManager::get() {
	static ProgramManager instance;
	return instance;
}