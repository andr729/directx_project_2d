#pragma once

#include "base.hpp"
#include "draw_tools.hpp"
#include "physics.hpp"

struct GameScene {
	void newLevel();
	void draw();
};

void addWallRect(Vector2D pos, Vector2D v, Float dx, Float dy, Float mass);
void addCircObject(Vector2D pos, Vector2D v, Float r, Float mass);
