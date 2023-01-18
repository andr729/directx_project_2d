#pragma once

#include "base.hpp"
#include "game_state.hpp"
#include "shop_scene.hpp"
#include "game_scene.hpp"
#include "menu_scene.hpp"
#include "win_scene.hpp"
#include "physics.hpp"
#include <d2d1_3.h>

enum class Scene {
	ShopScene,
	GameScene,
	MenuScene,
	WinScene,
};

struct GlobalState {
	ID2D1Factory7* factory = nullptr;
	ID2D1HwndRenderTarget* render_target = nullptr;

	uint64_t tick = 0;
	Scene scene;

	GameState game_state;
	ShopScene shop_scene;
	GameScene game_scene;
	MenuScene menu_scene;
	WinScene win_scene;

	Vector2D mouse_position;

	EntityHandler handler;

	void onMouseMove(FLOAT x, FLOAT y);
};

extern GlobalState global_state;
