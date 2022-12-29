#pragma once

#include <d2d1_3.h>
#include <cmath>

#define hr(code) {HRESULT hresult = code; if(FAILED(hresult)) return hresult;}

typedef float Float;
constexpr Float TICK_TIME = 1;

struct Vector2D {
	Float x, y;

	[[no_discard]]
	constexpr Vector2D operator+(const Vector2D& oth) const {
		return {x + oth.x, y + oth.y};
	}

	[[no_discard]]
	constexpr Vector2D operator-(const Vector2D& oth) const {
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
	constexpr Vector2D operator*(const Float v) const {
		return {x * v, y * v};
	}
	[[no_discard]]
	constexpr Vector2D operator/(const Float v) const {
		return {x / v, y / v};
	}
	constexpr void operator*=(const Float v) {
		x *= v;
		y *= v;
	}

	[[no_discard]]
	Vector2D operator-() {
		return {-x, -y};
	}

	[[no_discard]]
	constexpr Float abs2() const {
		return x * x + y * y;
	}

	[[no_discard]]
	Float abs() const {
		return std::sqrt(abs2());
	}

	Vector2D normUnit() const {
		return *this / abs();
	}
};

Float dot(Vector2D v1, Vector2D v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

Vector2D projection(Vector2D base, Vector2D onto) {
	return onto.normUnit() * dot(base, onto.normUnit());
}

class TransformationMatrix {
	D2D1::Matrix3x2F inner;
public:

	TransformationMatrix() {}
	TransformationMatrix(const D2D1::Matrix3x2F m) : inner(m) {}

	const D2D1::Matrix3x2F& getInner() {
		return inner;
	}

	TransformationMatrix& operator=(const TransformationMatrix& oth) {
		inner = oth.inner;
		return *this;
	};

	TransformationMatrix operator*(const TransformationMatrix& oth) {
		TransformationMatrix out;
		out.inner = inner;
		out.inner.SetProduct(out.inner, oth.inner);
	};

	TransformationMatrix& operator*=(const TransformationMatrix& oth) {
		inner.SetProduct(inner, oth.inner);
		return *this;
	};
};


