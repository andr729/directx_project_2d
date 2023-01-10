#include "game_scene.hpp"
#include "global_state.hpp"
#include "entity_utils.hpp"
#include "random.hpp"
#include <numbers>

void addWallRect(Vector2D pos, Float dx, Float dy) {
	auto rect = new RectangleEntity(pos, {0, 0}, dx, dy);
	rect->drawable = &DT::rect_drawable;
	rect->immoveable = true;
	global_state.handler.addWall(rect);
}

void addCircObject(Vector2D pos, Vector2D v, Float size) {
	auto circ = new CircleEntity(pos, v, size);
	circ->drawable = &DT::outline_ellipse_drawable;
	circ->base_color = DT::randomColor(0.78);
	global_state.handler.addObject(circ);
}

void addExplodeObject(Vector2D pos, Vector2D v, D2D1_COLOR_F color) {
	auto circ = new CircleEntity(pos, v, 10);
	circ->drawable = &DT::ellipse_drawable;
	circ->base_color = color;
	global_state.handler.addExplosion(circ);
}

void addRectObject(Vector2D pos, Vector2D v) {
	auto rect = new RectangleEntity(pos, v, 20, 20);
	rect->drawable = &DT::outline_rect_drawable;
	rect->base_color = DT::randomColor(0.78);
	global_state.handler.addObject(rect);
}


void setUpWalls(Vector2D top_left, Vector2D bottom_right, Float width) {
	width /= 2;

	Float center_x = (top_left.x + bottom_right.x) / 2;
	Float center_y = (top_left.y + bottom_right.y) / 2;
	Float dx = (bottom_right.x - top_left.x ) / 2;
	Float dy = (bottom_right.y - top_left.y ) / 2;

	addWallRect({top_left.x + width, center_y}, width, dy);
	addWallRect({bottom_right.x - width, center_y}, width, dy);

	addWallRect({center_x, top_left.y + width}, dx, width);
	addWallRect({center_x, bottom_right.y  - width}, dx, width);
	
}

void GameScene::newLevel() {
	global_state.handler.clear();

	explosion_was = false;

	setUpWalls(top_left_simulation, bottom_right_simulation, 30);

	auto circ_count = global_state.game_state.upgrades[L"Number of circles"];
	auto rect_count = global_state.game_state.upgrades[L"Number of squares"];
	
	auto circ_speed = global_state.game_state.upgrades[L"Circle speed"];
	auto rect_speed = global_state.game_state.upgrades[L"Square speed"];
	
	auto explosion_radius = global_state.game_state.upgrades[L"Explosion radius"];
	this->explosion_speed = explosion_radius / explosion_life_time + 1;

	for (int i = 0; i < circ_count; i++) {
		addCircObject(
			RD::randVector(top_left_simulation, bottom_right_simulation),
			randomVelocity(circ_speed),
			20
		);
	}

	for (int i = 0; i < rect_count; i++) {
		addRectObject(
			RD::randVector(top_left_simulation, bottom_right_simulation),
			randomVelocity(rect_speed)
		);
	}

}

void GameScene::draw() {
	DT::drawText(L"+???", {10, 10, 100, 100}, DT::black_brush);
	DT::drawText(
		(std::to_wstring(int64_t(global_state.game_state.money)) + L"$").c_str(),
		{75,  75 + DT::TEXT_FONT_STROKE, 400, 75 + 2 * DT::TEXT_FONT_STROKE},
		DT::black_brush);


	global_state.render_target->FillRectangle({ top_left_simulation.x,top_left_simulation.y,bottom_right_simulation.x,bottom_right_simulation.y}, DT::light_gray_brush);
	global_state.handler.drawAll();
}


void GameScene::explode(Vector2D position, D2D1_COLOR_F color) {
	global_state.handler.explode(position, color);
}


void GameScene::onClick() {
	// @TODO: check if is in game
	if (!explosion_was) {
		explosion_was = true;
		explode(global_state.mouse_position, DT::randomColor(0.75f));
	}
	else {
		global_state.handler.clear();
		global_state.scene = Scene::ShopScene;
	}
}


