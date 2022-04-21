#pragma once

#include <cmath>
#include <limits>
#include <memory>

// Constants

const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358932385;

// Functions

inline double deg_to_rad(double deg) {
	return deg * pi / 180.0;
}
