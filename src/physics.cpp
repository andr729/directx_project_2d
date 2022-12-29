#include "physics.hpp"

void Entity::collideAlongAxis(Entity& ent1, Entity& ent2, Vector2D axis, Float elasticity) {
		// split velocities into component parallel and perpendicular components:
		Vector2D v1_c = projection(ent1.velocity, axis);
		Vector2D v2_c = projection(ent2.velocity, -axis);

		// @todo:
		ent1.position -= v1_c * 0.1; 
		ent1.position -= v2_c * 0.1;

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
		Vector2D rect_borders[4] = {rect.position + Vector2D{rect.dx, 0}, rect.position + Vector2D{0, rect.dy},
		                            rect.position + Vector2D{-rect.dx, 0}, rect.position + Vector2D{0, -rect.dy}};

		for (auto pos: rect_borders) {
			if (isInside(pos)) {
				return true;
			}
		}

		return false;
	}
};

bool CircleEntity::collide(Entity& oth, Float elasticity) {
	if (oth.getType() == EntityType::Circle) {
		collideAlongAxis(*this, oth, oth.position - position, elasticity);
	}
	else {
		throw "Aaaa";
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
		const RectangleEntity& rect = static_cast<const RectangleEntity&>(oth);

		if (position.x + dx < oth.position.x) return false;
		if (oth.position.x + rect.dx < position.x) return false;
		if (position.y + dy < oth.position.y) return false;
		if (oth.position.y + rect.dy < position.y) return false;
		return true;
	}
};

bool RectangleEntity::isInside(const Vector2D& pos) const {
	return position.x <= pos.x && position.x + dx >= pos.x &&
		position.y <= pos.y && position.y + dy >= pos.y; 
}