#pragma once

#include "button.hpp"

class WinScene {
	Button continue_button;
	HRESULT update();
public:
	void init();
	HRESULT draw();
	void onClick();
};
