#pragma once

#include "base.hpp"
#include <random>
#include <cassert>
#include <limits>

// XD
#undef max

namespace RD {
    extern std::mt19937_64 rand;

	inline int64_t randRange(int64_t from, int64_t to) {
		assert(from <= to);
		int64_t value = static_cast<int64_t>(rand());
		return std::abs(value % (to - from + 1)) + from;
	}

	inline Vector2D randVector(Vector2D top_left, Vector2D bottom_right) {
		auto x = randRange(top_left.x, bottom_right.x);
		auto y = randRange(top_left.y, bottom_right.y);
		return {Float(x), Float(y)};
	}

	inline Float randFloat() {
		return Float(rand()) / std::numeric_limits<uint64_t>::max();
	}

}
