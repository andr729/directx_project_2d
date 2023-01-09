#include "game_scene.hpp"
#include "global_state.hpp"
#include "random.hpp"

DT::RectDrawable rect_drawable;
DT::EllipseDrawable ellipse_drawable;

void add_wall_rect(Vector2D pos, Float dx, Float dy) {
	auto rect = new RectangleEntity(pos, {0, 0}, dx, dy);
	rect->drawable = &rect_drawable;
	rect->immoveable = true;
	global_state.handler.addWall(rect);
}

void add_circ_object(Vector2D pos, Vector2D v, Float r, Float mass) {
	auto circ = new CircleEntity(pos, v, r);
	circ->mass = mass;
	circ->drawable = &ellipse_drawable;
	global_state.handler.addObject(circ);
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

	set_up_walls(top_left_simulation, bottom_right_simulation, 30);

	for (int i = 0; i < 10; i++) {
		add_circ_object(
			RD::randVector({300, 300}, {500, 500}),
			{120, 10},
			10,
			1
		);
	}

}

void GameScene::draw() {
	global_state.handler.drawAll();
}