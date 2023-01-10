#include "physics.hpp"
#include "entity_utils.hpp"
#include <algorithm>
#include <cassert>
#include <numbers>
#include <limits>

// XD
#undef min

bool Entity::collideAlongAxis(Entity& ent1, Entity& ent2, Vector2D axis, Float elasticity) {
		if (ent1.immoveable and ent2.immoveable) {
			return false;
		}

		// split velocities into component parallel and perpendicular components:
		const Float org_v1 = projectionScalar(ent1.velocity, axis);
		const Float org_v2 = projectionScalar(ent2.velocity, axis);
		Float v1 = org_v1;
		Float v2 = org_v2;
		
		if (v1 - v2 < 0) {
			return false;
		}

		// change reference frame to center of momentum
		Float v_cm;
		if (ent1.immoveable) {
			v_cm = v1;
		}
		else if (ent2.immoveable) {
			v_cm = v2;
		}
		else {
			v_cm = (v1 * ent1.mass + v2 * ent2.mass) / (ent1.mass + ent2.mass);
		}

		v1 -= v_cm;
		v2 -= v_cm;

		// collide:
		v1 = -v1 * elasticity;
		v2 = -v2 * elasticity;

		// return to original reference frame:
		v1 += v_cm;
		v2 += v_cm;

		// update velocities:
		if (!ent1.immoveable) {
			ent1.velocity += axis.normUnit() * (v1 - org_v1);
		}
		if (!ent2.immoveable) {
			ent2.velocity += axis.normUnit() * (v2 - org_v2);;
		}

		return true;
}

void EntityHandler::clear() {
	walls.clear();
	objects.clear();
	free_particles.clear();
	explosions.clear();
}

void EntityHandler::addWall(Entity* ent) {
	walls.emplace_back(ent);
}

void EntityHandler::addObject(Entity* ent) {
	objects.emplace_back(ent);
}

void EntityHandler::addExplosion(Entity* ent) {
	assert(ent != nullptr);
	explosions.emplace_back(ent);
}

void EntityHandler::addGravityParticle(Entity* ent, size_t life_time) {
	ent->max_life_time = life_time;
	ent->force = {0, 1500};
	free_particles.emplace_back(ent);
}

bool isEntDead(std::unique_ptr<Entity>& ent) {
	return !ent->alive;
}

void clearDead(std::vector<std::unique_ptr<Entity> >& vec) {
	auto it = std::remove_if(vec.begin(), vec.end(), isEntDead);
	auto size = it - vec.begin();
	vec.resize(size);
}

void EntityHandler::clearDead() {
	::clearDead(explosions);
	::clearDead(objects);
	::clearDead(free_particles);
}

void EntityHandler::simulateTick() {
	for (auto& e: objects) {
		e->simulateTick();
	}

	for (auto& e: free_particles) {
		e->simulateTick();
		if (e->life_time > e->max_life_time) {
			e->alive = false;
		}
	}

	for (auto& e: explosions) {
		e->simulateTick();
		if (e->life_time > e->max_life_time) {
			e->alive = false;
		}
	}

	clearDead();

	this->collideAll(1);
}

void EntityHandler::explode(Vector2D position, D2D1_COLOR_F color) {
	for (int i = 0; i < 8; i++) {
		auto angle = (i / 8.f) * 2 * std::numbers::pi;
		Vector2D v = Vector2D{std::sinf(angle), std::cosf(angle)} * global_state.game_scene.explosion_speed;

		auto circ = new CircleEntity(position, v, 10);
		circ->drawable = &DT::ellipse_drawable;
		circ->base_color = color;
		circ->max_life_time = global_state.game_scene.explosion_life_time * FPS;
		addExplosion(circ);
	}
}

void EntityHandler::collideAll(Float elasticity) {

	for (size_t i = 0; i < objects.size(); i++)
	for (size_t j = i + 1; j < objects.size(); j++) {
		if (objects[i]->collides(*objects[j])) {
			objects[i]->collide(*objects[j], elasticity);
		}
	}

	for (size_t i = 0; i < explosions.size(); i++)
	for (size_t j = 0; j < objects.size(); j++) {
		if (explosions[i]->collides(*objects[j])) {
			explosions[i]->alive = false;
			objects[j]->alive = false;
			auto color = objects[j]->base_color;
			color.r *= 0.7;
			color.g *= 0.7;
			color.b *= 0.7;
			explode(objects[j]->position, color);
		}
	}

	for (size_t i = 0; i < walls.size(); i++)
	for (size_t j = 0; j < objects.size(); j++) {
		if (walls[i]->collides(*objects[j])) {
			walls[i]->collide(*objects[j], elasticity);
		}
	}
}

void EntityHandler::drawAll() {
	for (auto& e: walls) {
		e->draw();
	}
	for (auto& e: objects) {
		e->draw();
	}
	for (auto& e: free_particles) {
		e->draw();
	}
	for (auto& e: explosions) {
		e->draw();
	}
}

bool CircleEntity::collides(const Entity& oth) const {
	if (!alive || !oth.alive) {
		return false;
	}
	if (oth.getType() == EntityType::Circle) {
		auto radius_sum = (radius + dynamic_cast<const CircleEntity&>(oth).radius);
		return (position - oth.position).abs2() < radius_sum * radius_sum;
	}
	else if (oth.getType() == EntityType::Rectangle) {
		Vector2D circle_borders[4] = {position + Vector2D{radius, 0}, position + Vector2D{0, radius},
		                              position + Vector2D{-radius, 0}, position + Vector2D{0, -radius}};
		
		for (auto pos: circle_borders) {
			if (oth.isInside(pos)) {
				return true;
			}
		}

		const RectangleEntity& rect = dynamic_cast<const RectangleEntity&>(oth);
		Vector2D rect_borders[4] = {rect.position + Vector2D{rect.dx, rect.dy}, rect.position + Vector2D{rect.dx, -rect.dy},
		                            rect.position + Vector2D{-rect.dx, rect.dy}, rect.position + Vector2D{-rect.dx, -rect.dy},};

		for (auto pos: rect_borders) {
			if (isInside(pos)) {
				return true;
			}
		}

		return false;
	}
};

void CircleEntity::collide(Entity& oth, Float elasticity) {
	if (oth.getType() == EntityType::Circle) {
		collideAlongAxis(*this, oth, oth.position - position, elasticity);
	}
	else {
		const RectangleEntity& rect = dynamic_cast<const RectangleEntity&>(oth);
		auto top_left_corner = rect.position + Vector2D(-rect.dx, -rect.dy);
		auto top_right_corner = rect.position + Vector2D(rect.dx, -rect.dy);
		auto bottom_left_corner = rect.position + Vector2D(-rect.dx, rect.dy);
		auto bottom_right_corner = rect.position + Vector2D(rect.dx, rect.dy);

		if (position.x >= rect.position.x - rect.dx and position.x <= rect.position.x + rect.dx) {
			if (position.y < rect.position.y) {
				// top collision
				auto aux = collideAlongAxis(*this, oth, {0, 1}, elasticity);
				if (aux and oth.immoveable) {
					position.y = oth.position.y - rect.dy - radius;
				}
			}
			else {
				// bottom collision
				auto aux = collideAlongAxis(*this, oth, {0, -1}, elasticity);
				if (aux and oth.immoveable) {
					position.y = oth.position.y + rect.dy + radius;
				}
			}
		}
		else if (position.y >= rect.position.y - rect.dy and position.y <= rect.position.y + rect.dy) {
			if (position.x < rect.position.x) {
				// left collision
				auto aux = collideAlongAxis(*this, oth, {1, 0}, elasticity);
				if (aux and oth.immoveable) {
					position.x = oth.position.x - rect.dx - radius;
				}
			}
			else {
				// right collision
				auto aux = collideAlongAxis(*this, oth, {-1, 0}, elasticity);
				if (aux and oth.immoveable) {
					position.x = oth.position.x + rect.dx + radius;
				}
			}
		}
		else if (position.x < rect.position.x) {
			if (position.y < rect.position.y) {
				// top left collision
				collideAlongAxis(*this, oth, top_left_corner - position, elasticity);
			}
			else {
				// bottom left collision
				collideAlongAxis(*this, oth, bottom_left_corner - position, elasticity);
			}
		}
		else {
			if (position.y < rect.position.y) {
				// top right collision
				collideAlongAxis(*this, oth, top_right_corner - position, elasticity);
			}
			else {
				// bottom right collision
				collideAlongAxis(*this, oth, bottom_right_corner - position, elasticity);
			}
		}
	}
}

bool CircleEntity::isInside(const Vector2D& pos) const {
	return (pos - position).abs2() <= radius*radius;
}

bool RectangleEntity::collides(const Entity& oth) const {
	if (!alive || !oth.alive) {
		return false;
	}
	if (oth.getType() == EntityType::Circle) {
		return oth.collides(*this);
	}
	else if (oth.getType() == EntityType::Rectangle) {

		const RectangleEntity& rect = dynamic_cast<const RectangleEntity&>(oth);

		if (position.x + dx < oth.position.x - rect.dx) return false;
		if (position.x - dx > oth.position.x + rect.dx) return false;
		if (position.y + dy < oth.position.y - rect.dy) return false;
		if (position.y - dy > oth.position.y + rect.dy) return false;
		return true;
	}
};

void RectangleEntity::collide(Entity& oth, Float elasticity) {
	if (oth.getType() == EntityType::Circle) {
		oth.collide(*this, elasticity);
	}
	else {
		
		const RectangleEntity& rect = dynamic_cast<const RectangleEntity&>(oth);

		Float relative_x_v = oth.velocity.x - velocity.x;
		Float relative_y_v = oth.velocity.y - velocity.y;

		// todo
		//   when my_border oth_border
		Float when_right_left = -((position.x + dx) - (oth.position.x - rect.dx)) / (relative_x_v);
		Float when_left_right = ((oth.position.x + rect.dx) - (position.x - dx)) / (relative_x_v);

		Float when_bottom_top = -((position.y + dy) - (oth.position.y - rect.dy)) / (relative_y_v);
		Float when_top_bottom = ((oth.position.y + rect.dy) - (position.y - dy)) / (relative_y_v);

		Float max_lr = -std::numeric_limits<Float>::infinity();
		Float max_tb = -std::numeric_limits<Float>::infinity();

		Float eps = 1e-2;
		if (std::abs(relative_x_v) > eps) {
			if (relative_x_v > 0) {
				max_lr = when_right_left;
			}
			else {
				max_lr = when_left_right;
			}
		}
		
		if (std::abs(relative_y_v) > eps) {
			if (relative_y_v > 0) {
				max_tb = when_bottom_top;
			}
			else {
				max_tb = when_top_bottom;
			}
		}

		if (max_lr == -std::numeric_limits<Float>::infinity())
		if (max_tb == -std::numeric_limits<Float>::infinity()) {
			return;
		}

		Float maximum = std::max(max_lr, max_tb);

		if (maximum == max_tb and relative_y_v > 0 and when_top_bottom > 0) {
			collideAlongAxis(*this, oth, {0, -1}, elasticity);
		}
		else if (maximum == max_tb and relative_y_v < 0 and when_bottom_top > 0) {
			collideAlongAxis(*this, oth, {0, 1}, elasticity);
		}
		else if (maximum == max_lr and relative_x_v > 0 and when_left_right > 0) {
			collideAlongAxis(*this, oth, {-1, 0}, elasticity);
		}
		else if (maximum == max_lr and relative_x_v < 0 and when_right_left > 0) {
			// throw 1;
			collideAlongAxis(*this, oth, {1, 0}, elasticity);
		}
	}
}

bool RectangleEntity::isInside(const Vector2D& pos) const {
	return position.x - dx <= pos.x && position.x + dx >= pos.x &&
		position.y - dy <= pos.y && position.y + dy >= pos.y; 
}