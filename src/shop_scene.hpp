#pragma once

#include <vector>
#include "upgrade_bar.hpp"

class ShopScene {
	std::vector<UpgradeBar> bars;
	Button continue_button;
	HRESULT update();
public:
	void init();
	HRESULT draw();
	void onClick();
};
