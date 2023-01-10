#pragma once

#include "base.hpp"
#include "physics.hpp"
#include "global_state.hpp"
#include "random.hpp"
#include <numbers>

inline Vector2D randomVelocity(Float norm) {
	Float angle = RD::randFloat() * std::numbers::pi * 2;
	return {norm * std::sinf(angle), norm * std::cosf(angle)};
}

inline void spawnRainbowParticle(Vector2D position, D2D1_COLOR_F color) {
	// @TODO: rainbow
	Entity* particle = new RectangleEntity(position, randomVelocity(300), 5, 5);
	particle->drawable = &DT::rect_drawable;
	particle->base_color = color;
	global_state.handler.addGravityParticle(particle, 100);
}

inline void spawnTransitionParticle(Vector2D position) {
	// @TODO: rainbow
	Entity* particle = new CircleEntity(position, {0, 10}, 10);
	particle->drawable = &DT::ellipse_drawable;
	particle->base_color = DT::randomColor(0.78);
	global_state.handler.addGravityParticle(particle, 400, 300);
}

inline void addWallRect(Vector2D pos, Float dx, Float dy, D2D1_COLOR_F color) {
	auto rect = new RectangleEntity(pos, {0, 0}, dx, dy);
	rect->drawable = &DT::rect_drawable;
	rect->immoveable = true;
	rect->base_color = color;
	global_state.handler.addWall(rect);
}

inline void addCircObject(Vector2D pos, Vector2D v) {
	auto circ = new CircleEntity(pos, v, 20);
	circ->drawable = &DT::outline_ellipse_drawable;
	circ->base_color = DT::randomColor(0.78);
	global_state.handler.addObject(circ);
}

inline void addExplodeObject(Vector2D pos, Vector2D v, D2D1_COLOR_F color) {
	auto circ = new CircleEntity(pos, v, 10);
	circ->drawable = &DT::ellipse_drawable;
	circ->base_color = color;
	global_state.handler.addExplosion(circ);
}

inline void addRectObject(Vector2D pos, Vector2D v) {
	auto rect = new RectangleEntity(pos, v, 20, 20);
	rect->drawable = &DT::outline_rect_drawable;
	rect->base_color = DT::randomColor(0.78);
	global_state.handler.addObject(rect);
}

inline void setUpWalls(Vector2D top_left, Vector2D bottom_right, Float width, D2D1_COLOR_F color = {0, 0, 0, 1}) {
	width /= 2;

	Float center_x = (top_left.x + bottom_right.x) / 2;
	Float center_y = (top_left.y + bottom_right.y) / 2;
	Float dx = (bottom_right.x - top_left.x ) / 2;
	Float dy = (bottom_right.y - top_left.y ) / 2;

	addWallRect({top_left.x + width, center_y}, width, dy, color);
	addWallRect({bottom_right.x - width, center_y}, width, dy, color);

	addWallRect({center_x, top_left.y + width}, dx, width, color);
	addWallRect({center_x, bottom_right.y  - width}, dx, width, color);
	
}
