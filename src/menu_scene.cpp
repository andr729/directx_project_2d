#include "menu_scene.hpp"
#include "global_state.hpp"
#include "draw_tools.hpp"
#include "entity_utils.hpp"

void MenuScene::init() {
	constexpr static Vector2D top_left_simulation = {10, 10};
	constexpr static Vector2D bottom_right_simulation = {1600 - 10, 900 - 10};

	start_button = Button(L"Start", { 600, 300, 1000, 450 });

	setUpWalls(top_left_simulation, bottom_right_simulation, 30, {0, 0, 0, 0});

	for (size_t i = 0; i < 25; i++) {
		addCircObject(
			RD::randVector(top_left_simulation + Vector2D{80, 80}, bottom_right_simulation - Vector2D{80, 80}),
			RD::randVelocity(450)
		);
		addRectObject(
			RD::randVector(top_left_simulation + Vector2D{80, 80}, bottom_right_simulation - Vector2D{80, 80}),
			RD::randVelocity(450)
		);

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

	global_state.render_target->FillRectangle({ 0,0,1600,900}, DT::light_gray_brush);
	global_state.handler.drawAll();

	DT::drawTitleGeometry();
	
	constexpr int bitmap_width = 500;
	constexpr int bitmap_height = 164;

	start_button.draw();
	DT::drawBitmap(DT::title_bitmap, { 800 - bitmap_width / 2, 50, 800 + bitmap_width / 2, 50 + bitmap_height });
	DT::drawText(L"Made by:\nPiotr \"Piols\" K\x0119pczy\x0144ski\nAndrzej \"andr729\" Radzimi\x0144ski", { 10, 700, 610, 850 }, DT::black_brush);

}

void MenuScene::onClick() {
	if (start_button.state == ButtonState::Hovered) {
		global_state.scene = Scene::GameScene;
		global_state.handler.clear();
		global_state.game_scene.newLevel();

	}
}