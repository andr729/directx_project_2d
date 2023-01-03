#pragma once

#include <windows.h>
#include <d2d1_3.h>
#include <string>

enum class ButtonState {
	Unclickable,
	Clickable,
	Hovered,
};

struct Button {
	const WCHAR* text;
	D2D1_RECT_F position;
	D2D1_COLOR_F color;
	ButtonState state;

	Button(const WCHAR* text, D2D1_RECT_F position, D2D1_COLOR_F color, ButtonState state = ButtonState::Unclickable) : 
		text(text), position(position), color(color), state(state) {}

	void draw(ID2D1HwndRenderTarget*);
};