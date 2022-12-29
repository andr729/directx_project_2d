#pragma once

#include <cmath>

constexpr Float TICK_TIME = 1;

typedef float Float;
struct Vector2D {
	Float x, y;

	[[no_discard]]
	constexpr Vector2D operator+(const Vector2D& oth) {
		return {x + oth.x, y + oth.y};
	}
	[[no_discard]]
	constexpr Vector2D operator-(const Vector2D& oth) {
		return {x - oth.x, y - oth.y};
	}
	constexpr void operator+=(const Vector2D& oth) {
		x += oth.x;
		y += oth.y;
	}
	constexpr void operator-=(const Vector2D& oth) {
		x -= oth.x;
		y -= oth.y;
	}

	[[no_discard]]
	constexpr Vector2D operator*(const Float v) {
		return {x * v, y * v};
	}
	[[no_discard]]
	constexpr Vector2D operator/(const Float v) {
		return {x / v, y / v};
	}
	constexpr void operator*=(const Float v) {
		x *= v;
		y *= v;
	}

	[[no_discard]]
	constexpr Float abs2() {
		return x * x + y * y;
	}

	[[no_discard]]
	Float abs() {
		return std::sqrt(abs2());
	}
};

// We have it to be able to quickly deterine entity type
enum class EntityType {
	Circle, Rectangle
};

struct Entity {
protected:
	Vector2D position = {0, 0};
	Vector2D velocity = {0, 0};
	EntityType type;

	Entity(Vector2D position, Vector2D velocity, EntityType type):
		position(position),
		velocity(velocity),
		type(type) {}

public:
	void simulateTick() {
		position += velocity * TICK_TIME;	
	}

	void simulateTick(Vector2D force) {
		auto delta_v = force * TICK_TIME;
		position += (velocity + delta_v/2) * TICK_TIME;
		velocity += delta_v;
	}

	virtual bool collides(const Entity& oth) = 0;
	virtual bool isInside(const Vector2D& pos) = 0;
};

struct CircleEntity: public Entity {
private:
	Float radius = 0;
public:
	CircleEntity(Vector2D position, Vector2D velocity, Float radius):
		Entity(position, velocity, EntityType::Circle), radius(radius) {}
	
	virtual bool collides(const Entity& oth) {
		if (oth.type == EntityType::Circle) {
			
		}
	};
};

struct RectangeEntity: public Entity {
	Float dx = 0;
	Float dy = 0;
	RectangeEntity(Vector2D position, Vector2D velocity, Float dx, Float dy):
		Entity(position, velocity, EntityType::Rectangle), dx(dx), dy(dy) {}
};