#pragma once

#include <d2d1_3.h>

#define hr(code) {HRESULT hresult = code; if(FAILED(hresult)) return hresult;}

typedef float Float;
struct Vector2D {
	Float x, y;

	Vector2D operator+(const Vector2D&);
	// ...
};

class TransformationMatrix {
	D2D1::Matrix3x2F inner;
public:

	TransformationMatrix() {}
	TransformationMatrix(const D2D1::Matrix3x2F m) : inner(m) {}

	const D2D1::Matrix3x2F& getInner() {
		return inner;
	}

	// D2D1::Matrix3x2F

	TransformationMatrix& operator=(const TransformationMatrix& oth) {
		inner = oth.inner;
		return *this;
	};

	TransformationMatrix operator*(const TransformationMatrix& oth) {
		Matrix3 out;
		out.inner = inner;
		out.inner.SetProduct(out.inner, oth.inner);
	};

	TransformationMatrix& operator*=(const TransformationMatrix& oth) {
		inner.SetProduct(inner, oth.inner);
		return *this;
	};
};


