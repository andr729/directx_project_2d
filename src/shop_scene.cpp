#include "shop_scene.hpp"
#include "draw_tools.hpp"
#include "global_state.hpp"

void ShopScene::init() {
	float bars_start_height = 250;
	float bars_start_width = 50;
	float bar_width = 1500;
	float bar_height = 60;
	float horizontal_gap = 20;

	Vector2D topleft = { bars_start_width, bars_start_height };
	UpgradeBar bar;
	bar.name = L"Number of circles";
	bar.rect = { 
		.left = topleft.x,
		.top = topleft.y, 
		.right = topleft.x + bar_width, 
		.bottom = topleft.y + bar_height 
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.8f, 0.0f, 0.0f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Circle speed";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.98f, 0.58f, 0.04f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Explosion radius";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(1.f, 1.f, 0.f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Explosion duration";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.f, 0.8f, 0.f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Number of squares";
	bar.rect = { 
		.left = topleft.x,
		.top = topleft.y, 
		.right = topleft.x + bar_width, 
		.bottom = topleft.y + bar_height 
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.01f, 0.75f, 0.78f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Square speed";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.0f, 0.0f, 1.f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Number of bullets";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 5;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.48f, 0.17f, 0.65f, 1.0f);
	bars.push_back(bar);

	topleft.y += bar_height + horizontal_gap;
	bar.name = L"Bullet duration";
	bar.rect = {
		.left = topleft.x,
		.top = topleft.y,
		.right = topleft.x + bar_width,
		.bottom = topleft.y + bar_height
	};
	bar.max_level = 10;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(1.f, 0.59f, 0.75f, 1.0f);
	bars.push_back(bar);
	for (auto& bar : bars) {
		bar.init();
	}
}

void ShopScene::update() {
	DT::updateRadialBrush();
	Vector2D mouse_position = global_state.mouse_position;
	for (int i = 0; i < bars.size(); i++) {
		Button& button = bars[i].button;
		ButtonState state = ButtonState::Clickable;
		if (mouse_position.x < button.position.right && mouse_position.x > button.position.left &&
			mouse_position.y < button.position.bottom && mouse_position.y > button.position.top) {
			state = ButtonState::Hovered;
		}
		if (bars[i].cost > global_state.game_state.money || bars[i].level == bars[i].max_level) {
			state = ButtonState::Unclickable;
		}
		button.state = state;
	}
}

void ShopScene::draw() {
	update();

	ID2D1HwndRenderTarget* render_target = global_state.render_target;
	render_target->DrawRectangle({ 0,0,1600,900 }, DT::black_brush, 5);

	for (auto& bar : bars) {
		bar.draw();
	}
}