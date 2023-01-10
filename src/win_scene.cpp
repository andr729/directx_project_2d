#include "win_scene.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"
#include "entity_utils.hpp"

void WinScene::init() {
	global_state.game_state.won = true;
	continue_button = Button(L"Continue playing", { 600, 600, 1000, 750 });
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
}

void WinScene::draw() {
	update();

	constexpr int bitmap_width = 500;
	constexpr int bitmap_height = 164;
	continue_button.draw();
	DT::drawBitmap(DT::title_bitmap, { 800 - bitmap_width / 2, 50, 800 + bitmap_width / 2, 50 + bitmap_height });
	DT::drawText(L"Made by:\nPiotr \"Piols\" Kêpczyñski\nAndrzej \"andr729\" Radzimiñski", { 10, 700, 610, 850 }, DT::black_brush);

}

void WinScene::onClick() {
	if (continue_button.state == ButtonState::Hovered) {
		global_state.scene = Scene::GameScene;
		global_state.handler.clear();
		global_state.game_scene.newLevel();

	}
}