#include <cstddef>
#include <vector>

typedef float Float;
struct Vector2D {
	Float x, y;

	Vector2D operator+(const Vector2D&);
	// ...
};

// static drawable
struct Drawable {
	virtual void drawOn(Vector2D position /*???*/);
};

struct Texture: public Drawable {
	// ...
};

// animated by tick drawable
struct Animation {
	virtual void tick(size_t ticks = 1);
	virtual void drawOn(Vector2D position /*???*/);
};

struct Entity {
	Vector2D position;
	Vector2D velocity;
	Drawable* draw_obj; // optionally = nullptr

	virtual bool collides(const Entity& oth);
	virtual bool drawOn(/*...*/);
};

// generic shapes
struct CircleShape: public Entity {
	Float radius;
};

// generic shapes
struct RectangleShape: public Entity {

};

struct Particle: public CircleShape {};

// struct ExplodableObject {
// 	// ??
// 	virtual void explode(/*...*/);
// };

struct Circle: public CircleShape {
	
};

struct Square: public RectangleShape {};

struct Bullet: public RectangleShape {};
struct Wall: public RectangleShape {};


struct GameEntityHandler {
	std::vector<Entity*> explodable;
	std::vector<Entity*> explosions;
	
	void detectCollisions();
};

struct GameScene {
	GameEntityHandler ent_handler;

	std::vector<Drawable*> drawables;
};

struct ShopScene {};

// optional
// struct MenuScene {};






