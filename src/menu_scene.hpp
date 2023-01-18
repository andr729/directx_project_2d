#pragma once

#include "button.hpp"

class MenuScene {
	Button start_button;
	HRESULT update();
public:
	void init();
	HRESULT draw();
	void onClick();
};
