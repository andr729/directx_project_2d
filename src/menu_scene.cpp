#include "menu_scene.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"

void MenuScene::init() {
	start_button = Button(L"Start", { 600, 300, 1000, 450 });
}

void MenuScene::update() {
	DT::updateRadialBrush();
	Vector2D mouse_position = global_state.mouse_position;
	Button& button = start_button;
	ButtonState state = ButtonState::Clickable;
	if (mouse_position.x < button.position.right && mouse_position.x > button.position.left &&
		mouse_position.y < button.position.bottom && mouse_position.y > button.position.top) {
		state = ButtonState::Hovered;
	}
	button.state = state;
}

void MenuScene::draw() {
	update();

	DT::drawTitleGeometry();

	start_button.draw();
	
	const int bitmap_width = 500;
	const int bitmap_height = 164;
	DT::drawBitmap(DT::title_bitmap, { 800 - bitmap_width / 2, 50, 800 + bitmap_width / 2, 50 + bitmap_height });

	DT::drawText(L"Made by:\nPiotr \"Piols\" Kêpczyñski\nAndrzej \"andr729\" Radzimiñski", { 10, 700, 610, 850 }, DT::black_brush);
}

void MenuScene::onClick() {
	if (start_button.state == ButtonState::Hovered) {
		global_state.scene = Scene::GameScene;
		global_state.game_scene.newLevel();
	}
}