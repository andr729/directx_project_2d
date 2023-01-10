#pragma once

#include "button.hpp"

struct WinScene {
	Button continue_button;

	void init();

	// draw() calls update() so no need to manually call this one.
	void update();

	void draw();

	void onClick();
};