#pragma once

#include <map>
#include <string>

struct GameState {
	double money;
	std::map<std::wstring, double> upgrades;
};