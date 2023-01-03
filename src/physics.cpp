#include "physics.hpp"
#include <algorithm>

// XD
#undef min

void Entity::collideAlongAxis(Entity& ent1, Entity& ent2, Vector2D axis, Float elasticity) {
		// split velocities into component parallel and perpendicular components:
		Vector2D v1_c = projection(ent1.velocity, axis);
		Vector2D v2_c = projection(ent2.velocity, -axis);

		// @todo:
		ent1.position -= v1_c * 0.1; 
		ent2.position -= v2_c * 0.1;

		ent1.velocity -= v1_c;
		ent2.velocity -= v2_c;

		// change reference frame to center of momentum
		Vector2D v_cm = (v1_c * ent1.mass + v2_c * ent2.mass) / (ent1.mass + ent2.mass);
		v1_c -= v_cm;
		v2_c -= v_cm;

		// collide:
		v1_c = -v1_c * elasticity;
		v2_c = -v2_c * elasticity;

		// return to original reference frame:
		v1_c += v_cm;
		v2_c += v_cm;

		// update velocities:
		ent1.velocity += v1_c;
		ent2.velocity += v2_c;
}

bool CircleEntity::collides(const Entity& oth) const {
	if (oth.getType() == EntityType::Circle) {
		auto radius_sum = (radius + static_cast<const CircleEntity&>(oth).radius);
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

		const RectangleEntity& rect = static_cast<const RectangleEntity&>(oth);
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
		const RectangleEntity& rect = static_cast<const RectangleEntity&>(oth);
	
		// Approximate the collision type:
		Float top = std::abs( position.y - (rect.position.y - rect.dy - radius));
		Float bottom = std::abs( position.y - (rect.position.y + rect.dy + radius));
		Float left = std::abs( position.x - (rect.position.x - rect.dx - radius));
		Float right = std::abs( position.x - (rect.position.x + rect.dx + radius));

		Float top_left = std::abs((position - (rect.position + Vector2D(-rect.dx, -rect.dy))).abs() - radius);
		Float top_right = std::abs((position - (rect.position + Vector2D(rect.dx, -rect.dy))).abs() - radius);
		Float bottom_left = std::abs((position - (rect.position + Vector2D(-rect.dx, rect.dy))).abs() - radius);
		Float bottom_right = std::abs((position - (rect.position + Vector2D(rect.dx, rect.dy))).abs() - radius);

		Float min_dist = std::min({top, bottom, left, right, top_left, top_right, bottom_left, bottom_right});

		if (top == min_dist) {
			collideAlongAxis(*this, oth, {0, 1}, elasticity);
		}
		else if (bottom == min_dist) {
			collideAlongAxis(*this, oth, {0, -1}, elasticity);
		}
		else if (left == min_dist) {
			collideAlongAxis(*this, oth, {1, 0}, elasticity);
		}
		else if (right == min_dist) {
			collideAlongAxis(*this, oth, {-1, 0}, elasticity);
		}
		else if (top_left == min_dist) {
			collideAlongAxis(*this, oth, {1, 1}, elasticity);
		}
		else if (top_right == min_dist) {
			collideAlongAxis(*this, oth, {-1, 1}, elasticity);
		}
		else if (bottom_left == min_dist) {
			collideAlongAxis(*this, oth, {1, -1}, elasticity);
		}
		else if (bottom_right == min_dist) {
			collideAlongAxis(*this, oth, {-1, -1}, elasticity);
		}
	}
}

bool CircleEntity::isInside(const Vector2D& pos) const {
	return (pos - position).abs2() < radius*radius;
}

bool RectangleEntity::collides(const Entity& oth) const {
	if (oth.getType() == EntityType::Circle) {
		return oth.collides(*this);
	}
	else if (oth.getType() == EntityType::Rectangle) {
		// @TODO This might be wrong:
		const RectangleEntity& rect = static_cast<const RectangleEntity&>(oth);

		if (position.x + dx < oth.position.x) return false;
		if (oth.position.x + rect.dx < position.x) return false;
		if (position.y + dy < oth.position.y) return false;
		if (oth.position.y + rect.dy < position.y) return false;
		return true;
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