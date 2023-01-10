#include "game_scene.hpp"
#include "global_state.hpp"
#include "random.hpp"
#include <numbers>

void add_wall_rect(Vector2D pos, Float dx, Float dy) {
	auto rect = new RectangleEntity(pos, {0, 0}, dx, dy);
	rect->drawable = &DT::rect_drawable;
	rect->immoveable = true;
	global_state.handler.addWall(rect);
}

void add_circ_object(Vector2D pos, Vector2D v, Float size) {
	auto circ = new CircleEntity(pos, v, size);
	circ->drawable = &DT::ellipse_drawable;
	circ->base_color = DT::randomColor();
	global_state.handler.addObject(circ);
}

void add_explode_object(Vector2D pos, Vector2D v) {
	auto circ = new CircleEntity(pos, v, 10);
	circ->drawable = &DT::ellipse_drawable;
	global_state.handler.addExplosion(circ);
}


void set_up_walls(Vector2D top_left, Vector2D bottom_right, Float width) {
	width /= 2;

	Float center_x = (top_left.x + bottom_right.x) / 2;
	Float center_y = (top_left.y + bottom_right.y) / 2;
	Float dx = (bottom_right.x - top_left.x ) / 2;
	Float dy = (bottom_right.y - top_left.y ) / 2;

	add_wall_rect({top_left.x + width, center_y}, width, dy);
	add_wall_rect({bottom_right.x - width, center_y}, width, dy);

	add_wall_rect({center_x, top_left.y + width}, dx, width);
	add_wall_rect({center_x, bottom_right.y  - width}, dx, width);
	
}

void GameScene::newLevel() {
	global_state.handler.clear();

	explosion_was = false;

	set_up_walls(top_left_simulation, bottom_right_simulation, 30);

	// for (int i = 0; i < 10; i++) {
	// 	add_circ_object(
	// 		RD::randVector({500, 500}, {510, 510}),
	// 		{120, 10}
	// 	);
	// }

	for (int i = 0; i < 10; i++) {
		add_circ_object(
			RD::randVector(top_left_simulation, bottom_right_simulation),
			{50, 50},
			20
		);
	}

}

void GameScene::draw() {
	DT::drawText(L"+???", {10, 10, 100, 100}, DT::black_brush);
	DT::drawText(
		(std::to_wstring(int64_t(global_state.game_state.money)) + L"$").c_str(),
		{75,  75 + DT::TEXT_FONT_STROKE, 400, 75 + 2 * DT::TEXT_FONT_STROKE},
		DT::black_brush);
	global_state.handler.drawAll();
}


void GameScene::explode(Vector2D position) {
	for (int i = 0; i < explosion_object_count; i++) {
		Float angle = 2*std::numbers::pi * i / explosion_object_count;
		Vector2D norm_v = {std::sinf(angle), std::cosf(angle)};
		add_explode_object(
			position + norm_v * 10,
			norm_v * 20
		);
	}
}


void GameScene::onClick() {
	// @TODO: check if is in game
	explode(global_state.mouse_position);
	global_state.handler.clear();
	global_state.scene = Scene::ShopScene;
}


