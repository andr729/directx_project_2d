#pragma once

struct MenuScene {
	Button start_button;

	void init();

	// draw() calls update() so no need to manually call this one.
	void update();

	void draw();

	void onClick();
};