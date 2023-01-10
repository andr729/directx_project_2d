#pragma once

#include "base.hpp"
#include "draw_tools.hpp"
#include "physics.hpp"

class GameScene {
	constexpr static Vector2D top_left_simulation = {100, 200};
	constexpr static Vector2D bottom_right_simulation = {1600 - 100, 900 - 100};
	constexpr static size_t explosion_object_count = 8;

	bool explosion_was;

	void explode(Vector2D position, D2D1_COLOR_F color);

public:
	constexpr static size_t explosion_speed = 10;
	size_t explosion_life_time;

	void newLevel();
	void draw();
	void onClick();
};

void addWallRect(Vector2D pos, Vector2D v, Float dx, Float dy, Float mass);
void addCircObject(Vector2D pos, Vector2D v, Float r, Float mass);
