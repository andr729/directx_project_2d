#include "shop_scene.hpp"
#include "draw_tools.hpp"
#include "global_state.hpp"

void ShopScene::init() {

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
		if (bars[i].cost > global_state.game_state.money || bars[i].level == UPGRADES_PER_BAR) {
			state = ButtonState::Unclickable;
		}
		button.state = state;
	}
}

void ShopScene::draw() {
	update();

}