#pragma once

#include <map>
#include <string>

struct GameState {
	double money;
	std::map<std::string, double> upgrades;
};