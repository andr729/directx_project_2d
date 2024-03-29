#pragma once

#include <d2d1_3.h>
#include <cmath>

#define hr(code) {HRESULT hresult = (code); if(FAILED(hresult)) return hresult;}

typedef float Float;
constexpr size_t FPS = 50;
constexpr Float TICK_TIME = 1.f / FPS;

class TransformationMatrix {
	D2D1::Matrix3x2F inner;
public:

	TransformationMatrix() {}
	TransformationMatrix(const D2D1::Matrix3x2F m) : inner(m) {}

	D2D1::Matrix3x2F& getInner() { return inner; }
	const D2D1::Matrix3x2F& getInner() const { return inner; }

	TransformationMatrix& operator=(const TransformationMatrix& oth) {
		inner = oth.inner;
		return *this;
	};

	TransformationMatrix operator*(const TransformationMatrix& oth) {
		TransformationMatrix out;
		out.inner = inner;
		out.inner.SetProduct(out.inner, oth.inner);
		return out;
	};

	TransformationMatrix& operator*=(const TransformationMatrix& oth) {
		inner.SetProduct(inner, oth.inner);
		return *this;
	}

	TransformationMatrix inverted() const {
		auto out = *this;
		out.getInner().Invert();
		return out;
	}
};

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

	void operator*=(const TransformationMatrix& matrix) {
		auto& m = matrix.getInner();
		Float x2 = m.m11 * x + m.m21 * y + m.dx;
		Float y2 = m.m12 * x + m.m22 * y + m.dy;
		x = x2;
		y = y2;
	}

	[[no_discard]]
	Vector2D operator*(const TransformationMatrix& matrix) {
		Vector2D out = *this;
		out *= matrix;
		return out;
	}

	[[no_discard]]
	Vector2D operator-() const {
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

	[[no_discard]]
	Vector2D normUnit() const {
		return *this / abs();
	}

	D2D1_POINT_2F toD2D1point() {
		return D2D1_POINT_2F{x, y};
	}
};

inline Float dot(Vector2D v1, Vector2D v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline Float projectionScalar(Vector2D base, Vector2D onto) { 
	return dot(base, onto.normUnit());
}

template<typename T>
void SafeRelease(T **ppT) {
	if (*ppT) {
		(*ppT)->Release();
		*ppT = nullptr;
	}
}
