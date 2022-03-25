#pragma once

#include "vec3.h"

class ray {
	point3 m_origin;
	vec3 m_dir;

public:
	ray() {}
	ray(const point3& origin_point, const vec3& direction)
		: m_origin{origin_point}, m_dir{direction} {}

	point3 origin() const { return m_origin; }
	vec3 direction() const { return m_dir; }
};
