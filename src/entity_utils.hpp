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







