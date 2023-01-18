#pragma once

#include <map>
#include <string>

struct GameState {
	double money;
	std::map<std::wstring, float> upgrades;
	bool won = false;
};
