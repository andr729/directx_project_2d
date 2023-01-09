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
	std::wstring text;
	D2D1_RECT_F position;
	D2D1_COLOR_F color;
	ButtonState state;

	Button(std::wstring text = L"", D2D1_RECT_F position = {0.f, 0.f, 0.f, 0.f}, D2D1_COLOR_F color = D2D1_COLOR_F(1.0f, 1.0f, 1.0f, 1.0f), ButtonState state = ButtonState::Unclickable) :
		text(text), position(position), color(color), state(state) {}

	void draw();
};
