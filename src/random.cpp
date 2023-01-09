#include "random.hpp"

namespace RD {
	namespace {
    	std::random_device rd;
	}

	extern std::mt19937_64 rand(rd());

}
