#include "upgrade_bar.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"

const float upgrades_start = 0.25;
const float upgrades_end = 0.6;
const float button_middle = 0.65;
const float value_start = 0.75;
const float increment_start = 0.85;


void UpgradeBar::init() {
	float height = rect.bottom - rect.top;
	float width = rect.right - rect.left;
	D2D1_RECT_F button_rect = {
		.left = rect.left + width * button_middle - height / 2,
		.top = rect.top,
		.right = rect.left + width * button_middle + height / 2,
		.bottom = rect.bottom
	};
	button = Button(L"+", button_rect, color);
}

void UpgradeBar::draw() {
	ID2D1HwndRenderTarget* render_target = global_state.render_target;
	Vector2D topleft = { rect.left, rect.top };
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;

	DT::drawText(name.c_str(), {.left = rect.left, .top = rect.top, .right = rect.left + width * upgrades_start, .bottom = rect.bottom}, DT::black_brush);

	float rect_spacing = (upgrades_end - upgrades_start) * width / (2 * max_level - 1);
	float rects_start = rect.left + width * upgrades_start;
	DT::changeBrushColor(color);
	for (int i = 0; i < max_level; i++) {
		D2D1_RECT_F current_rect = {
			.left = rects_start + 2 * i * rect_spacing,
			.top = rect.top,
			.right = rects_start + (2 * i + 1) * rect_spacing,
			.bottom = rect.bottom
		};
		render_target->FillRectangle(current_rect, (level > i ? DT::customizable_brush : DT::light_gray_brush));
		render_target->DrawRectangle(current_rect, DT::black_brush, 5);
	}

	button.draw();

	std::wstring value_text = std::to_wstring(global_state.game_state.upgrades[name]);
	DT::drawText(value_text.c_str(),
		{ 
			.left = rect.left + value_start * width, 
			.top = rect.top, 
			.right = rect.left + width * increment_start, 
			.bottom = rect.bottom 
		}, DT::black_brush);
	
	std::wstring increment_text = L"(+" + std::to_wstring(global_state.game_state.upgrades[name]) + L")";
	if (level == max_level) {
		increment_text = L"(max)";
	}
	DT::drawText(increment_text.c_str(),
		{ 
			.left = rect.left + increment_start * width, 
			.top = rect.top, 
			.right = rect.right, 
			.bottom = rect.bottom 
		}, DT::black_brush);
}