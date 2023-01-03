#pragma once

#include <cmath>
#include "base.hpp"
#include "draw_tools.hpp"

// We have it to be able to quickly determine entity type
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
	DT::Drawable* drawable = nullptr;

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
	virtual void collide(Entity& oth, Float elasticity) = 0;
	virtual bool isInside(const Vector2D& pos) const = 0;

	// axis is from ent1 to ent2
	static void collideAlongAxis(Entity& ent1, Entity& ent2, Vector2D axis, Float elasticity);

	virtual void draw(ID2D1RenderTarget* rt) = 0;
};

struct CircleEntity: public Entity {
	Float radius = 0;
	CircleEntity(Vector2D position, Vector2D velocity, Float radius):
		Entity(position, velocity, EntityType::Circle), radius(radius) {}
	
	bool collides(const Entity& oth) const final;
	void collide(Entity& oth, Float elasticity) final;
	bool isInside(const Vector2D& pos) const final;

	void draw(ID2D1RenderTarget* rt) override {
		if (drawable)
			drawable->draw(rt, position, {radius, radius});
	}
};

struct RectangleEntity: public Entity {
	Float dx = 0;
	Float dy = 0;
	RectangleEntity(Vector2D position, Vector2D velocity, Float dx, Float dy):
		Entity(position, velocity, EntityType::Rectangle), dx(dx), dy(dy) {}

	bool collides(const Entity& oth) const final;
	void collide(Entity& oth, Float elasticity) final;
	bool isInside(const Vector2D& pos) const final;

	void draw(ID2D1RenderTarget* rt) override {
		if (drawable)
			drawable->draw(rt, position, {dx, dy});
	}
};