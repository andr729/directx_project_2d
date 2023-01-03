#pragma once

#include <string>
#include <d2d1_3.h>
#include "button.hpp"
#include "base.hpp"

struct UpgradeBar {
	std::wstring name;
	D2D1_RECT_F rect;
	double cost;
	double cost_multiplier;
	int level = 0;
	int max_level;
	double levelup_value;
	Button button;
	D2D1_COLOR_F color;

	void init();

	void draw();
};