#pragma once

#include <windows.h>

enum class ButtonState {
	Unclickable,
	Clickable,
	Hovered,
};

struct Button {
	std::string text;
	RECT position;
	ButtonState state;

	Button(std::string text, RECT position, ButtonState state = ButtonState::Unclickable) : text(text), position(position), state(state) {}
};