#include "physics.hpp"
#include <algorithm>

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
	walls.push_back(ent);
}

void EntityHandler::addObject(Entity* ent) {
	objects.push_back(ent);
}

void EntityHandler::simulateTick() {
	for (auto e: objects) {
		e->simulateTick();
	}

	for (auto e: free_particles) {
		e->simulateTick();
	}

	this->collideAll(1);
}
void EntityHandler::collideAll(Float elasticity) {

	for (size_t i = 0; i < objects.size(); i++)
	for (size_t j = i + 1; j < objects.size(); j++) {
		if (objects[i]->collides(*objects[j])) {
			objects[i]->collide(*objects[j], elasticity);
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
	for (auto e: walls) {
		e->draw();
	}
	for (auto e: objects) {
		e->draw();
	}
	for (auto e: free_particles) {
		e->draw();
	}
}

bool CircleEntity::collides(const Entity& oth) const {
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
	if (oth.getType() == EntityType::Circle) {
		return oth.collides(*this);
	}
	else if (oth.getType() == EntityType::Rectangle) {

		throw "AA";
		// @TODO This might be wrong:
		// const RectangleEntity& rect = dynamic_cast<const RectangleEntity&>(oth);

		// if (position.x + dx < oth.position.x) return false;
		// if (oth.position.x + rect.dx < position.x) return false;
		// if (position.y + dy < oth.position.y) return false;
		// if (oth.position.y + rect.dy < position.y) return false;
		// return true;
	}
};

void RectangleEntity::collide(Entity& oth, Float elasticity) {
	if (oth.getType() == EntityType::Circle) {
		oth.collide(*this, elasticity);
	}
	else {
		throw "Rect Rect collision not yet implemented";
	}
}

bool RectangleEntity::isInside(const Vector2D& pos) const {
	return position.x - dx <= pos.x && position.x + dx >= pos.x &&
		position.y - dy <= pos.y && position.y + dy >= pos.y; 
}