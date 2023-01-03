#pragma once

#include <string>
#include <d2d1_3.h>
#include "button.hpp"
#include "base.hpp"

const int UPGRADES_PER_BAR = 10;

struct UpgradeBar {
	const WCHAR* name;
	D2D1_RECT_F rect;
	double cost;
	double cost_multiplier;
	int level;
	double levelup_value;
	Button button;
	D2D1_COLOR_F color;

	void draw();
};