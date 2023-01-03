#pragma once

#include <vector>
#include "upgrade_bar.hpp"

struct ShopScene {
	std::vector<UpgradeBar> bars;
	void init();

	void update();

	void draw();
};