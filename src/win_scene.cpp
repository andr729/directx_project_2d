#include "win_scene.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"
#include "entity_utils.hpp"

void WinScene::init() {
	global_state.game_state.won = true;
	continue_button = Button(L"Continue playing", { 450, 700, 1150, 850 });
}

void WinScene::update() {
	DT::updateRadialBrush();
	Vector2D mouse_position = global_state.mouse_position;
	Button& button = continue_button;
	ButtonState state = ButtonState::Clickable;
	if (mouse_position.x < button.position.right && mouse_position.x > button.position.left &&
		mouse_position.y < button.position.bottom && mouse_position.y > button.position.top) {
		state = ButtonState::Hovered;
	}
	button.state = state;


	spawnTransitionParticle({Float(RD::randRange(0, 1600)), 0});
}

void WinScene::draw() {
	update();

	DT::drawText(L"CONGRATS,", { 0, -50, 1600, 150 }, DT::lin_brush, DT::shop_text_format);
	DT::drawText(L"YOU WON!", { 0, -50 + DT::SHOP_FONT_STROKE, 1600, 150 + DT::SHOP_FONT_STROKE}, DT::lin_brush, DT::shop_text_format);
	DT::drawText(L"Thank you for playing,\nHope you enjoyed your playthrough!\n\nPiols & andr729", {0, 50, 1600, 700}, DT::black_brush, DT::button_text_format);
	continue_button.draw();
	//DT::drawText(L"Made by:\nPiotr \"Piols\" K\x0119pczy\x0144ski\nAndrzej \"andr729\" Radzimi\x0144ski", { 10, 700, 610, 850 }, DT::black_brush);

	global_state.handler.drawAll();
}

void WinScene::onClick() {
	if (continue_button.state == ButtonState::Hovered) {
		global_state.scene = Scene::GameScene;
		global_state.handler.clear();
		global_state.game_scene.newLevel();

	}
}