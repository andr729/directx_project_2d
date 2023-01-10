#include "shop_scene.hpp"
#include "draw_tools.hpp"
#include "global_state.hpp"
#include "entity_utils.hpp"

// TODO: hr here

D2D1_RECT_F rect(Vector2D top_left, Float dx, Float dy) {
	return D2D1_RECT_F { 
		.left = top_left.x,
		.top = top_left.y, 
		.right = top_left.x + dx, 
		.bottom = top_left.y + dy 
	};
}

UpgradeBar defaultUpgradeBar(std::wstring&& name, D2D1_COLOR_F color, D2D1_RECT_F rect) {
	UpgradeBar out;
	out.name = std::move(name);
	out.rect = rect;
	out.max_level = 10;
	out.levelup_value = 1;
	out.cost = 100;
	out.cost_multiplier = 2;
	out.color = color;
	return out;
}

void ShopScene::init() {
	constexpr Float bars_start_height = 225;
	constexpr Float bars_start_width = 50;
	constexpr Float bar_width = 1500;
	constexpr Float bar_height = 60;
	constexpr Float horizontal_gap = 20;

	Vector2D top_left = { bars_start_width, bars_start_height };

	auto addDefaultBar = [&](std::wstring&& name, D2D1_COLOR_F color) {
		bars.push_back(defaultUpgradeBar(
			std::move(name), color,
			rect(top_left, bar_width, bar_height)));
		top_left.y += bar_height + horizontal_gap;
	};
	
	addDefaultBar(L"Multiplier", D2D1_COLOR_F(0.8f, 0.0f, 0.0f, 1.0f));
	bars[bars.size() - 1].levelup_value = 0.01;

	addDefaultBar(L"Number of circles", D2D1_COLOR_F(0.98f, 0.58f, 0.04f, 1.0f));

	addDefaultBar(L"Circle speed", D2D1_COLOR_F(1.f, 1.f, 0.f, 1.0f));

	addDefaultBar(L"Explosion radius", D2D1_COLOR_F(0.f, 0.8f, 0.f, 1.0f));
	bars[bars.size() - 1].levelup_value = 5.f;
	addDefaultBar(L"Number of squares", D2D1_COLOR_F(0.01f, 0.75f, 0.78f, 1.0f));
	addDefaultBar(L"Square speed", D2D1_COLOR_F(0.0f, 0.0f, 1.f, 1.0f));

	UpgradeBar bar;
	bar.name = L"Number of bullets";
	bar.rect = rect(top_left, bar_width, bar_height);	
	bar.max_level = 5;
	bar.levelup_value = 1;
	bar.cost = 100;
	bar.cost_multiplier = 2;
	bar.color = D2D1_COLOR_F(0.48f, 0.17f, 0.65f, 1.0f);
	bars.push_back(bar);
	top_left.y += bar_height + horizontal_gap;
	
	addDefaultBar(L"Bullet duration", D2D1_COLOR_F(1.f, 0.59f, 0.75f, 1.0f));
	bars[bars.size() - 1].levelup_value = 0.1f;
	
	for (auto& bar : bars) {
		bar.init();
	}

	continue_button = Button(L"Continue", { 1125, 75, 1525, 200}, { 1.0f, 1.0f, 1.0f, 1.0f });

	DT::initLinearBrush();
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
	Button& button = continue_button;
	ButtonState state = ButtonState::Clickable;
	if (mouse_position.x < button.position.right && mouse_position.x > button.position.left &&
		mouse_position.y < button.position.bottom && mouse_position.y > button.position.top) {
		state = ButtonState::Hovered;
	}
	button.state = state;
}

void ShopScene::draw() {
	update();

	ID2D1HwndRenderTarget* render_target = global_state.render_target;
	render_target->FillRectangle({ 20,20,1580,880 }, DT::light_gray_brush);
	render_target->DrawRectangle({ 20,20,1580,880 }, DT::black_brush, 5);

	for (auto& bar : bars) {
		bar.draw();
	}
	continue_button.draw();

	DT::drawText(L"Money:", { 75, 75, 400, 75 + DT::TEXT_FONT_STROKE }, DT::black_brush);
	DT::drawText((std::to_wstring(int64_t(global_state.game_state.money)) + L"$").c_str(), {75,  75 + DT::TEXT_FONT_STROKE, 400, 75 + 2 * DT::TEXT_FONT_STROKE}, DT::black_brush);
	DT::drawText(L"SHOP", { 500, 0, 1100, DT::SHOP_FONT_STROKE}, DT::black_brush, DT::shop_text_format);
	DT::drawText(L"SHOP", { 500, 0, 1100, DT::SHOP_FONT_STROKE}, DT::lin_brush, DT::shop_text_format);

	global_state.handler.drawAll();
}

void ShopScene::onClick() {
	update();
	Vector2D mouse_position = global_state.mouse_position;

	for (auto &bar : bars) {
		Button& button = bar.button;
		if (button.state == ButtonState::Hovered) {
			bar.level++;
			global_state.game_state.upgrades[bar.name] += bar.levelup_value;
			global_state.game_state.money -= bar.cost;
			bar.cost *= bar.cost_multiplier;
			DT::updateLinearBrush();

			for (size_t i = 0; i < 5; i++) {
				spawnRainbowParticle({mouse_position.x, mouse_position.y}, bar.color);
			}
			
		}
	}

	if (continue_button.state == ButtonState::Hovered) {
		// TODO: better handling of scene switching.
		global_state.scene = Scene::GameScene;
		global_state.game_scene.newLevel();
	}
}
