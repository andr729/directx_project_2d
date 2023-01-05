#pragma once

#include "base.hpp"
#include "game_state.hpp"
#include "shop_scene.hpp"
#include <d2d1_3.h>

enum class Scene {
	ShopScene,
	GameScene,
	Menu,
};

struct GlobalState {
	ID2D1Factory7* factory = nullptr;
	ID2D1HwndRenderTarget* render_target = nullptr;
	int tick = 0;
	GameState game_state;
	ShopScene shop_scene;
	Scene scene;
	// d2d1 stuff, and our stuff

	Vector2D mouse_position;
	// std::queue<Event> ??

	void onMouseMove(FLOAT x, FLOAT y);
	// ...
};

extern GlobalState global_state;
