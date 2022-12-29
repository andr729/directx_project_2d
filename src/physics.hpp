#pragma once

#include <cmath>
#include "base.hpp"

// We have it to be able to quickly deterine entity type
enum class EntityType {
	Circle, Rectangle
};

struct Entity {
protected:
	EntityType type;
	Entity(Vector2D position, Vector2D velocity, EntityType type):
		position(position),
		velocity(velocity),
		type(type) {}

public:
	Vector2D position = {0, 0};
	Vector2D velocity = {0, 0};
	Float mass = 1;
	bool immoveable = false;

	constexpr EntityType getType() const { return type; }

	void simulateTick() {
		position += velocity * TICK_TIME;	
	}

	void simulateTick(Vector2D force) {
		auto delta_v = force * TICK_TIME;
		position += (velocity + delta_v/2) * TICK_TIME;
		velocity += delta_v;
	}

	virtual bool collides(const Entity& oth) const = 0;
	virtual bool collide(Entity& oth, Float elasticity) = 0;
	virtual bool isInside(const Vector2D& pos) const = 0;
};

struct CircleEntity: public Entity {
	Float radius = 0;
	CircleEntity(Vector2D position, Vector2D velocity, Float radius):
		Entity(position, velocity, EntityType::Circle), radius(radius) {}
	
	bool collides(const Entity& oth) const final;
	bool isInside(const Vector2D& pos) const final;
};

struct RectangeEntity: public Entity {
	Float dx = 0;
	Float dy = 0;
	RectangeEntity(Vector2D position, Vector2D velocity, Float dx, Float dy):
		Entity(position, velocity, EntityType::Rectangle), dx(dx), dy(dy) {}

	bool collides(const Entity& oth) const;
	bool isInside(const Vector2D& pos) const;
};