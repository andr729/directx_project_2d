#include "menu_scene.hpp"
#include "global_state.hpp"

MenuScene::init() {
	start_button = Button(L"Start", { 600, 300, 100, 450 });
}

MenuScene::update() {
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

MenuScene::draw() {

}