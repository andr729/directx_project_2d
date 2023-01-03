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

	Button(const WCHAR* text = L"", D2D1_RECT_F position = {0.f, 0.f, 0.f, 0.f}, D2D1_COLOR_F color = D2D1_COLOR_F(0.0f, 0.0f, 0.0f, 1.0f), ButtonState state = ButtonState::Unclickable) :
		text(text), position(position), color(color), state(state) {}

	void draw();
};