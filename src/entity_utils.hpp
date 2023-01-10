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


inline void spawnRainbowParticle(Vector2D position) {
	// @TODO: rainbow
	Entity* particle = new CircleEntity(position, randomVelocity(100), 10);
	particle->drawable = &DT::ellipse_drawable;
	global_state.handler.addGravityParticle(particle, 100);
}







