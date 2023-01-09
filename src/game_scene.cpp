#include "game_scene.hpp"
#include "global_state.hpp"

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

void GameScene::newLevel() {
	global_state.handler.clear();

	add_wall_rect({20, 450}, 20, 450);

}

void GameScene::draw() {
	global_state.handler.drawAll();
}