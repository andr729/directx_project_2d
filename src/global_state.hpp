#pragma once

#include "base.hpp"
#include <d2d1_3.h>

struct GlobalState {
	ID2D1Factory7* factory = nullptr;
	ID2D1HwndRenderTarget* d2d_render_target = nullptr;
	int tick = 0;

	// d2d1 stuff, and our stuff

	Vector2D mouse_position;

	// std::queue<Event> ??

	void onMouseMove();
	// ...
};

extern GlobalState global_state;
