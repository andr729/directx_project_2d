#pragma once

#include <vector>
#include "upgrade_bar.hpp"

struct ShopScene {
	std::vector<UpgradeBar> bars;

	void update();

	void draw();
};