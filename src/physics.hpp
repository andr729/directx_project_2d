#pragma once

#include "base.hpp"
#include "draw_tools.hpp"
#include <cmath>
#include <vector>
#include <memory>

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
	Vector2D force = {0, 0};
	Float mass = 1;
	size_t life_time = 0;
	size_t max_life_time = -1;
	
	DT::Drawable* drawable = nullptr;
	bool immoveable = false;
	bool alive = true;

	constexpr EntityType getType() const { return type; }

	void simulateTick() {
		auto delta_v = force * TICK_TIME;
		position += (velocity + delta_v/2) * TICK_TIME;
		velocity += delta_v;
		life_time++;
	}

	virtual bool collides(const Entity& oth) const = 0;
	virtual void collide(Entity& oth, Float elasticity) = 0;
	virtual bool isInside(const Vector2D& pos) const = 0;

	// axis is from ent1 to ent2
	static bool collideAlongAxis(Entity& ent1, Entity& ent2, Vector2D axis, Float elasticity);

	virtual void draw() = 0;
};

class EntityHandler {
private:
	constexpr static size_t explosion_lifetime = 10;

	std::vector<std::unique_ptr<Entity> > objects;
	std::vector<std::unique_ptr<Entity> > walls;
	std::vector<std::unique_ptr<Entity> > free_particles;
	std::vector<std::unique_ptr<Entity> > explosions;

	void collideAll(Float elasticity);
	void clearDead();
public:
	void clear();
	void addWall(Entity*);
	void addObject(Entity*);
	void addExplosion(Entity*);
	void addGravityParticle(Entity*, size_t life_time);
	void simulateTick();
	void explode(Vector2D position);
	void drawAll();
};

struct CircleEntity: public Entity {
	Float radius = 0;
	CircleEntity(Vector2D position, Vector2D velocity, Float radius):
		Entity(position, velocity, EntityType::Circle), radius(radius) {}
	
	bool collides(const Entity& oth) const final;
	void collide(Entity& oth, Float elasticity) final;
	bool isInside(const Vector2D& pos) const final;

	void draw() override {
		if (drawable)
			drawable->draw(position, {radius, radius});
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

	void draw() override {
		if (drawable)
			drawable->draw(position, {dx, dy});
	}
};