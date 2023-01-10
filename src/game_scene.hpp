#pragma once

#include "base.hpp"
#include "draw_tools.hpp"
#include "physics.hpp"

enum class GameSceneState {
	WaitToExplode,
	Exploding,
	LastExplosionGone,
};
 
class GameScene {
	constexpr static Vector2D top_left_simulation = {100, 200};
	constexpr static Vector2D bottom_right_simulation = {1600 - 100, 900 - 100};
	constexpr static size_t explosion_object_count = 8;

	GameSceneState state;
	size_t state_switch_tick;
	
	double earned_money = 0;
	double next_value = 1;
	double multiplier;

	void explode(Vector2D position, D2D1_COLOR_F color);
	void switchToShop();
	void update();

public:
	constexpr static double explosion_life_time = 0.75;
	size_t explosion_speed;

	void newLevel();
	void draw();
	void onClick();

	void explosionWas();
};

void addWallRect(Vector2D pos, Vector2D v, Float dx, Float dy, Float mass);
void addCircObject(Vector2D pos, Vector2D v, Float r, Float mass);
