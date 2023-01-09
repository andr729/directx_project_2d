#pragma once

#include "base.hpp"
#include "draw_tools.hpp"
#include "physics.hpp"

struct GameScene {
	constexpr static Vector2D top_left_simulation = {100, 200};
	constexpr static Vector2D bottom_right_simulation = {1600 - 100, 900 - 100};

	bool explosion_was;

	void newLevel();
	void draw();
};

void addWallRect(Vector2D pos, Vector2D v, Float dx, Float dy, Float mass);
void addCircObject(Vector2D pos, Vector2D v, Float r, Float mass);
