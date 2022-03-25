#pragma once

#include <cmath>

class vec3 {
	double e[3];

public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	double length() {
		return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	double operator[](int i) const { return e[i]; } //TODO is this slow?

	inline vec3 unit_vector();
};

using color = vec3;
using point3 = vec3;

inline vec3 operator*(const vec3& v, double s) {
	return vec3(v[0] * s, v[1] * s, v[2] * s);
}

inline vec3 operator*(double s, const vec3& v) {
	return v * s;
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 operator/(const vec3& v, double s) {
	return v * (1 / s);
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 vec3::unit_vector() {
	return *this / this->length();
}