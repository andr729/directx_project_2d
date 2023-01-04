#pragma once

#include <vector>
#include "upgrade_bar.hpp"

struct ShopScene {
	std::vector<UpgradeBar> bars;
	Button continue_button;

	void init();

	// draw() calls update() so no need to manually call this one.
	void update();

	void draw();

	void onClick();
};