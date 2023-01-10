#include "menu_scene.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"
#include "entity_utils.hpp"

void MenuScene::init() {
	start_button = Button(L"Start", { 600, 300, 1000, 450 });

	setUpWalls({10, 10}, {1600 - 10, 900 - 10}, 40);

	for (size_t i = 0; i < 10; i++) {
		addCircObject({100.f + i, 250.f + i}, {10.f * i, 10.f * i + i});
		addRectObject({200.f + i, 350.f + i}, {10.f * i, 10.f * i - i});
	}
	
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

	global_state.handler.drawAll();

	DT::drawBitmap(DT::title_bitmap, { 800 - bitmap_width / 2, 50, 800 + bitmap_width / 2, 50 + bitmap_height });
	DT::drawText(L"Made by:\nPiotr \"Piols\" Kępczyński\nAndrzej \"andr729\" Radzimiński", { 10, 700, 610, 850 }, DT::black_brush);

}

void MenuScene::onClick() {
	if (start_button.state == ButtonState::Hovered) {
		global_state.scene = Scene::GameScene;
		global_state.handler.clear();
		global_state.game_scene.newLevel();

	}
}