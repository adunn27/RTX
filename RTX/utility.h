#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Constants

const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358932385;

// Functions

inline double clamp(double x, double min, double max) {
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}

inline double deg_to_rad(double deg) {
	return deg * pi / 180.0;
}

inline double random_double() {
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	static std::mt19937 generator;
	return dist(generator);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

