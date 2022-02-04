#pragma once
#include "Menu.h"
#include "Text.h"
//Data structure definitions for useful UI controllers

struct MenuButton {
	MenuButton(){};

	MenuOption menuType;
	Text buttonText;
	std::string label;
	bool selected;
};
