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

	double length_squared() {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	
	double length() {
		return std::sqrt(length_squared());
	}

	inline double operator[](int i) const { return e[i]; } //TODO is this slow?

	inline vec3 unit_vector();

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	bool near_zero() const {
		const auto s = 1e-18;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2] < s));
	}

	inline vec3& operator+=(const vec3& v) {
		e[0] += v[0];
		e[1] += v[1];
		e[2] += v[2];
		return *this;
	}
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

inline vec3 operator-(const vec3& v) {
	return vec3(-v[0], -v[1], -v[2]);
}

inline double vector_dot(const vec3& v1, const vec3& v2) {
	return v1[0] * v2[0]
		+ v1[1] * v2[1]
		+ v1[2] * v2[2];
}

inline vec3 vector_cross(const vec3& v1, const vec3& v2) {
	return vec3(v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]);
}

inline vec3 vec3::unit_vector() {
	return *this / this->length();
}

inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

inline vec3 random_unit_vector() {
	return random_in_unit_sphere().unit_vector();
}

vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (vector_dot(in_unit_sphere, normal) > 0)
		return in_unit_sphere;
	return -in_unit_sphere;
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * vector_dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(vector_dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

// For depth of field
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}
