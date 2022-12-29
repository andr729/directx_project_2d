#include "physics.hpp"

bool CircleEntity::collides(const Entity& oth) const {
	if (oth.getType() == EntityType::Circle) {
		return (position - oth.position).abs2() < (radius + static_cast<const CircleEntity&>(oth).radius);
	}
	else if (oth.getType() == EntityType::Rectangle) {
		Vector2D circle_borders[4] = {position + Vector2D{radius, 0}, position + Vector2D{0, radius},
		                              position + Vector2D{-radius, 0}, position + Vector2D{0, -radius}};
		
		for (auto pos: circle_borders) {
			if (oth.isInside(pos)) {
				return true;
			}
		}

		const RectangeEntity& rect = static_cast<const RectangeEntity&>(oth);
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


bool RectangeEntity::collides(const Entity& oth) const {
	if (oth.getType() == EntityType::Circle) {
		return oth.collides(*this);
	}
	else if (oth.getType() == EntityType::Rectangle) {
		const RectangeEntity& rect = static_cast<const RectangeEntity&>(oth);

		if (position.x + dx < oth.position.x) return false;
		if (oth.position.x + rect.dx < position.x) return false;
		if (position.y + dy < oth.position.y) return false;
		if (oth.position.y + rect.dy < position.y) return false;
		return true;
	}
};